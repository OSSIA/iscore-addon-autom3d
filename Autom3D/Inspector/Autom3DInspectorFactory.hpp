#pragma once
#include <Process/Inspector/ProcessInspectorWidgetDelegateFactory.hpp>
#include <Autom3D/Autom3DModel.hpp>
#include <Autom3D/Inspector/Autom3DInspectorWidget.hpp>

namespace Autom3D
{
class InspectorFactory final :
        public Process::InspectorWidgetDelegateFactory_T<ProcessModel, InspectorWidget>
{
        ISCORE_CONCRETE_FACTORY("d312d001-b91f-42f0-8780-7e3a587c6ffc")
};
}
