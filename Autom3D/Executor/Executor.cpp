#include "Executor.hpp"
#include <vtkParametricSpline.h>
#include <vtkPoints.h>
#include <iscore/tools/std/Algorithms.hpp>
#include <Device/Protocol/DeviceInterface.hpp>
#include <OSSIA/iscore2OSSIA.hpp>
#include <OSSIA/Protocols/OSSIADevice.hpp>
#include <ossia/network/base/node.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <ossia/editor/scenario/time_constraint.hpp>
#include <Device/Protocol/DeviceList.hpp>
namespace Autom3D
{
namespace Executor
{

ProcessExecutor::ProcessExecutor(
        const State::Address& addr,
        const std::vector<Point>& spline,
        const Device::DeviceList& devices,
        Point scale,
        Point origin,
        bool deriv):
    m_devices{devices},
    m_spline{vtkParametricSpline::New()},
    m_scale{scale},
    m_origin{origin},
    m_use_deriv{deriv}
{
    // Load the address
    // Look for the real node in the device
    auto dev_it = devices.find(addr.device);
    if(dev_it == devices.devices().end())
        return;

    auto dev = dynamic_cast<Ossia::OSSIADevice*>(*dev_it);
    if(!dev)
        return;

    auto node = iscore::convert::findNodeFromPath(addr.path, &dev->impl());
    if(!node)
        return;

    // Add the real address
    m_addr = node->getAddress();
    if(!m_addr)
        return;

    // Load the spline
    auto points = vtkPoints::New();
    for(const Point& pt : spline)
    {
        points->InsertNextPoint(pt.x(), pt.y(), pt.z());
    }
    m_spline->SetPoints(points);
}

ProcessExecutor::~ProcessExecutor()
{
    m_spline->Delete();
}

ossia::state_element ProcessExecutor::state()
{
    return state(parent->getPosition());
}

ossia::state_element ProcessExecutor::state(double t)
{
    if(m_addr)
    {
        double u[3]{t, 0, 0};
        double pt[3];
        double du[9];
        m_spline->Evaluate(u, pt, du);

        ossia::Tuple tuple;
        if(!m_use_deriv)
        {
          tuple.value = {
                ossia::Float{float(pt[0]) * m_scale.x() + m_origin.x()},
                ossia::Float{float(pt[1]) * m_scale.y() + m_origin.y()},
                ossia::Float{float(pt[2]) * m_scale.z() + m_origin.z()}};
        }
        else
        {
            double dt = t - m_prev_t;
            tuple.value = {
                  ossia::Float{float((pt[0] - m_prev_pt[0]) / dt) * m_scale.x()},
                  ossia::Float{float((pt[1] - m_prev_pt[1]) / dt) * m_scale.y()},
                  ossia::Float{float((pt[2] - m_prev_pt[2]) / dt) * m_scale.z()}};
        }

        m_prev_pt[0] = pt[0];
        m_prev_pt[1] = pt[1];
        m_prev_pt[2] = pt[2];
        m_prev_t = t;

        return ossia::Message{m_addr, tuple};
    }

    return {};
}

ossia::state_element ProcessExecutor::offset(ossia::time_value off)
{
    return state(off / parent->getDurationNominal());
}

}
}
