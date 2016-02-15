#pragma once
#include <OSSIA/ProcessModel/TimeProcessWithConstraint.hpp>
#include <OSSIA/Executor/ProcessElement.hpp>
#include <OSSIA/Executor/ExecutorContext.hpp>
#include <iscore/document/DocumentContext.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <Autom3D/Point.hpp>

class vtkParametricSpline;
class DeviceDocumentPlugin;

namespace Device
{
class DeviceList;
}
namespace State
{
struct Address;
}
namespace RecreateOnPlay
{
class ConstraintElement;
}
namespace OSSIA {
class State;
class StateElement;
class Address;
}  // namespace OSSIA

namespace Autom3D
{
class ProcessModel;
namespace Executor
{
class ProcessExecutor final :
        public TimeProcessWithConstraint
{
    public:
        ProcessExecutor(
                const State::Address& addr,
                const std::vector<Point>& spline,
                const Device::DeviceList& devices,
                Point scale,
                Point origin);
        ~ProcessExecutor();

        std::shared_ptr<OSSIA::StateElement> state(
                const OSSIA::TimeValue&,
                const OSSIA::TimeValue&) override;

    private:
        const Device::DeviceList& m_devices;

        std::shared_ptr<OSSIA::State> m_start;
        std::shared_ptr<OSSIA::State> m_end;

        std::shared_ptr<OSSIA::Address> m_addr;

        vtkParametricSpline* m_spline{};
        Point m_scale;
        Point m_origin;

};

}
}
