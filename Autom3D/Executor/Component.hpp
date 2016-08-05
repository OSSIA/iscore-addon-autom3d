#pragma once

#include <Autom3D/Autom3DModel.hpp>
#include <OSSIA/Executor/ProcessElement.hpp>
#include <Autom3D/Executor/Executor.hpp>
namespace Autom3D
{
class ProcessModel;
namespace Executor
{

class Component final :
        public ::RecreateOnPlay::ProcessComponent_T<Autom3D::ProcessModel, Autom3D::Executor::ProcessExecutor>
{
        COMPONENT_METADATA("fa6b32ac-8081-4273-8be8-7b83f31ba7f2")
    public:
        Component(
                RecreateOnPlay::ConstraintElement& parentConstraint,
                ProcessModel& element,
                const RecreateOnPlay::Context& ctx,
                const Id<iscore::Component>& id,
                QObject* parent);

};
using ComponentFactory = ::RecreateOnPlay::ProcessComponentFactory_T<Component>;
}
}
