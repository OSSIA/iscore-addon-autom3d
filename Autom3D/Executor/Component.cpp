#include "Component.hpp"
#include <Autom3D/Autom3DModel.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
namespace Autom3D
{
namespace Executor
{

Component::Component(
        Engine::Execution::ConstraintElement& parentConstraint,
        ProcessModel& element,
        const Engine::Execution::Context& ctx,
        const Id<iscore::Component>& id,
        QObject* parent):
    ::Engine::Execution::ProcessComponent_T<Autom3D::ProcessModel, ProcessExecutor>
        {parentConstraint, element, ctx, id, "Autom3DComponent", parent}
{
    auto proc = new ProcessExecutor(
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
