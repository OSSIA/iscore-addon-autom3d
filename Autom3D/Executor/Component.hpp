#pragma once

#include <Autom3D/Autom3DModel.hpp>
#include <OSSIA/Executor/ProcessElement.hpp>
namespace Autom3D
{
class ProcessModel;
namespace Executor
{

class Component final :
        public ::RecreateOnPlay::ProcessComponent_T<Autom3D::ProcessModel>
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
EXECUTOR_PROCESS_COMPONENT_FACTORY(ComponentFactory, "72416e08-5454-431a-a5cd-0c4a5c62e9e9", Component, Autom3D::ProcessModel)

}
}
