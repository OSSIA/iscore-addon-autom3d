#pragma once
#include <ossia/editor/scenario/time_process.hpp>
#include <Engine/Executor/ProcessElement.hpp>
#include <Engine/Executor/ExecutorContext.hpp>
#include <iscore/document/DocumentContext.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <Autom3D/Point.hpp>

class vtkParametricSpline;

namespace Device
{
class DeviceList;
}
namespace State
{
struct Address;
}
namespace Engine { namespace Execution
{
class ConstraintElement;
} }
namespace ossia {
class state;
namespace net {
class address_base;
}
}  // namespace OSSIA

namespace Autom3D
{
class ProcessModel;
namespace Executor
{
class ProcessExecutor final :
        public ossia::time_process
{
    public:
        ProcessExecutor(
                const State::Address& addr,
                const std::vector<Point>& spline,
                const Device::DeviceList& devices,
                Point scale,
                Point origin,
                bool deriv);
        ~ProcessExecutor();

        ossia::state_element state(double);
        ossia::state_element state() override;
        ossia::state_element offset(ossia::time_value) override;

    private:
        const Device::DeviceList& m_devices;

        ossia::state m_start;
        ossia::state m_end;

        ossia::net::address_base* m_addr{};

        vtkParametricSpline* m_spline{};
        Point m_scale;
        Point m_origin;

        double m_prev_t = 0;
        double m_prev_pt[3]{};
        bool m_use_deriv{};

};

}
}
