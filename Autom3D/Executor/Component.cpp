#include "Component.hpp"
#include "Executor.hpp"
#include <Autom3D/Autom3DModel.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
namespace Autom3D
{
namespace Executor
{

Component::Component(
        RecreateOnPlay::ConstraintElement& parentConstraint,
        ProcessModel& element,
        const RecreateOnPlay::Context& ctx,
        const Id<iscore::Component>& id,
        QObject* parent):
    ::RecreateOnPlay::ProcessComponent_T<Autom3D::ProcessModel>{parentConstraint, element, ctx, id, "Autom3DComponent", parent}
{
    auto proc = std::make_shared<ProcessExecutor>(
                    element.address(),
                    element.handles(),
                    ctx.devices.list(),
                    element.scale(),
                    element.origin(),
                    element.useDerivative());
    m_ossia_process = proc;
}

}
}
