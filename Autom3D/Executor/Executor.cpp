#include "Executor.hpp"
#include <vtkParametricSpline.h>
#include <vtkPoints.h>
#include <ossia/detail/algorithms.hpp>
#include <Device/Protocol/DeviceInterface.hpp>
#include <Engine/iscore2OSSIA.hpp>
#include <Engine/Protocols/OSSIADevice.hpp>
#include <ossia/network/base/node.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <ossia/editor/scenario/time_constraint.hpp>
#include <Device/Protocol/DeviceList.hpp>
namespace Autom3D
{
namespace Executor
{

static ossia::message make_message(
        const State::AddressAccessor& addr,
        const Device::DeviceList& devices)
{
    auto res = Engine::iscore_to_ossia::makeDestination(devices, addr);
    if(res)
    {
        return ossia::message{*res, {}};
    }
    else
    {
        throw std::runtime_error("Unable to find address: " + addr.toString().toStdString());
    }
}

ProcessExecutor::ProcessExecutor(
        const State::AddressAccessor& addr,
        const std::vector<State::vec3f>& spline,
        const Device::DeviceList& devices,
        State::vec3f scale,
        State::vec3f origin,
        bool deriv):
    m_spline{vtkParametricSpline::New()},
    m_scale{scale},
    m_origin{origin},
    m_message{make_message(addr, devices)},
    m_use_deriv{deriv}
{
    // Load the spline
    auto points = vtkPoints::New();
    for(const State::vec3f& pt : spline)
    {
        points->InsertNextPoint(pt[0], pt[1], pt[2]);
    }
    m_spline->SetPoints(points);
}

ProcessExecutor::~ProcessExecutor()
{
    m_spline->Delete();
}

ossia::state_element ProcessExecutor::state()
{
    return state(parent()->getPosition());
}

ossia::state_element ProcessExecutor::state(double t)
{
    double u[3]{t, 0, 0};
    double pt[3];
    double du[9];
    m_spline->Evaluate(u, pt, du);

    ossia::Vec3f vec;
    if(!m_use_deriv)
    {
        vec = {float(pt[0]) * m_scale[0] + m_origin[0],
                     float(pt[1]) * m_scale[1] + m_origin[1],
                     float(pt[2]) * m_scale[2] + m_origin[2]};
    }
    else
    {
        double dt = t - m_prev_t;
        vec = { float((pt[0] - m_prev_pt[0]) / dt) * m_scale[0],
                      float((pt[1] - m_prev_pt[1]) / dt) * m_scale[1],
                      float((pt[2] - m_prev_pt[2]) / dt) * m_scale[2]};
    }

    m_prev_pt[0] = pt[0];
    m_prev_pt[1] = pt[1];
    m_prev_pt[2] = pt[2];
    m_prev_t = t;

    m_message.message_value = vec;
    return m_message;
}

ossia::state_element ProcessExecutor::offset(ossia::time_value off)
{
    return state(off / parent()->getDurationNominal());
}

}
}
