#include "ComponentFactory.hpp"
#include "Component.hpp"
#include <Autom3D/Autom3DModel.hpp>
#include <OSSIA/Executor/DocumentPlugin.hpp>
namespace Autom3D
{
namespace Executor
{

ProcessComponentFactory::~ProcessComponentFactory()
{

}

RecreateOnPlay::ProcessComponent* ProcessComponentFactory::make(
        RecreateOnPlay::ConstraintElement& cst,
        Process::ProcessModel& proc,
        const RecreateOnPlay::Context& ctx,
        const Id<iscore::Component>& id,
        QObject* parent) const
{
    return new ProcessComponent{cst, static_cast<ProcessModel&>(proc), ctx, id, parent};
}

const ProcessComponentFactory::ConcreteFactoryKey&
ProcessComponentFactory::concreteFactoryKey() const
{
    static ConcreteFactoryKey k("4f97a846-3db4-458b-bf7a-19266ad84605");
    return k;
}

bool ProcessComponentFactory::matches(
        Process::ProcessModel& proc,
        const RecreateOnPlay::DocumentPlugin&) const
{
    return dynamic_cast<ProcessModel*>(&proc);
}

}
}
