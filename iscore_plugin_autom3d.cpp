#include <Autom3D/Autom3DLayerModel.hpp>
#include <Autom3D/Autom3DModel.hpp>
#include <unordered_map>

#include <Autom3D/Autom3DProcessMetadata.hpp>
#include <Autom3D/Commands/Autom3DCommandFactory.hpp>
#include <Inspector/InspectorWidgetFactoryInterface.hpp>
#include <Process/ProcessFactory.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <iscore/plugins/customfactory/StringFactoryKey.hpp>
#include <iscore/plugins/customfactory/FactorySetup.hpp>
#include "iscore_plugin_autom3d.hpp"

#include <Autom3D/Inspector/Autom3DInspectorFactory.hpp>
#include <Autom3D/Inspector/Autom3DStateInspectorFactory.hpp>

#include <iscore_plugin_autom3d_commands_files.hpp>
#include <Autom3D/Autom3DFactory.hpp>
#include <Autom3D/Executor/Component.hpp>
#include <Engine/Executor/DocumentPlugin.hpp>

iscore_plugin_autom3d::iscore_plugin_autom3d() :
    QObject {}
{
}

iscore_plugin_autom3d::~iscore_plugin_autom3d()
{

}


std::vector<std::unique_ptr<iscore::InterfaceBase>> iscore_plugin_autom3d::factories(
        const iscore::ApplicationContext& ctx,
        const iscore::InterfaceKey& key) const
{
    return instantiate_factories<
            iscore::ApplicationContext,
        FW<Process::ProcessModelFactory,
             Autom3D::ProcessFactory>,
        FW<Process::LayerFactory,
            Autom3D::LayerFactory>,
            /*,
#if defined(ISCORE_LIB_INSPECTOR)
        FW<Inspector::InspectorWidgetFactory,
             Autom3D::StateInspectorFactory>,
#endif*/
        FW<Process::InspectorWidgetDelegateFactory,
             Autom3D::InspectorFactory>,
        FW<Engine::Execution::ProcessComponentFactory,
             Autom3D::Executor::ComponentFactory>
    >(ctx, key);
}

std::pair<const CommandGroupKey, CommandGeneratorMap> iscore_plugin_autom3d::make_commands()
{
    using namespace Autom3D;
    std::pair<const CommandGroupKey, CommandGeneratorMap> cmds{CommandFactoryName(), CommandGeneratorMap{}};

    using Types = TypeList<
#include <iscore_plugin_autom3d_commands.hpp>
      >;
    for_each_type<Types>(iscore::commands::FactoryInserter{cmds.second});

    return cmds;
}
