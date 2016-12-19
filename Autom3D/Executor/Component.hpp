#pragma once

#include <Autom3D/Autom3DModel.hpp>
#include <Engine/Executor/ProcessComponent.hpp>
#include <Autom3D/Executor/Executor.hpp>
namespace Autom3D
{
class ProcessModel;
namespace Executor
{

class Component final :
        public ::Engine::Execution::ProcessComponent_T<Autom3D::ProcessModel, Autom3D::Executor::ProcessExecutor>
{
        COMPONENT_METADATA("fa6b32ac-8081-4273-8be8-7b83f31ba7f2")
    public:
        Component(
                Engine::Execution::ConstraintElement& parentConstraint,
                ProcessModel& element,
                const Engine::Execution::Context& ctx,
                const Id<iscore::Component>& id,
                QObject* parent);

};
using ComponentFactory = ::Engine::Execution::ProcessComponentFactory_T<Component>;
}
}
