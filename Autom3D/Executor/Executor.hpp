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
struct AddressAccessor;
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
                const State::AddressAccessor& addr,
                const std::vector<State::vec3f>& spline,
                const Device::DeviceList& devices,
                State::vec3f scale,
                State::vec3f origin,
                bool deriv);
        ~ProcessExecutor();

        ossia::state_element state(double);
        ossia::state_element state() override;
        ossia::state_element offset(ossia::time_value) override;

    private:
        ossia::net::address_base* m_addr{};

        vtkParametricSpline* m_spline{};
        State::vec3f m_scale;
        State::vec3f m_origin;
        ossia::message m_message;

        double m_prev_t = 0;
        double m_prev_pt[3]{};
        bool m_use_deriv{};

};

}
}
