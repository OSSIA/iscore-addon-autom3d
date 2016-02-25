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


#if defined(ISCORE_LIB_INSPECTOR)
#include <Autom3D/Inspector/Autom3DInspectorFactory.hpp>
#include <Autom3D/Inspector/Autom3DStateInspectorFactory.hpp>
#endif
#include <iscore_plugin_autom3d_commands_files.hpp>
#include <Autom3D/Autom3DFactory.hpp>
#include <Autom3D/Executor/ComponentFactory.hpp>
#include <OSSIA/Executor/DocumentPlugin.hpp>

iscore_plugin_autom3d::iscore_plugin_autom3d() :
    QObject {}
{
}

iscore_plugin_autom3d::~iscore_plugin_autom3d()
{

}


std::vector<std::unique_ptr<iscore::FactoryInterfaceBase>> iscore_plugin_autom3d::factories(
        const iscore::ApplicationContext& ctx,
        const iscore::AbstractFactoryKey& key) const
{
    return instantiate_factories<
            iscore::ApplicationContext,
    TL<
        FW<Process::ProcessFactory,
             Autom3D::ProcessFactory>,
#if defined(ISCORE_LIB_INSPECTOR)
        FW<Inspector::InspectorWidgetFactory,
             Autom3D::StateInspectorFactory>,
#endif
        FW<Process::InspectorWidgetDelegateFactory,
             Autom3D::InspectorFactory>,
        FW<RecreateOnPlay::ProcessComponentFactory,
             Autom3D::Executor::ProcessComponentFactory>
    >>(ctx, key);
}

std::pair<const CommandParentFactoryKey, CommandGeneratorMap> iscore_plugin_autom3d::make_commands()
{
    using namespace Autom3D;
    std::pair<const CommandParentFactoryKey, CommandGeneratorMap> cmds{CommandFactoryName(), CommandGeneratorMap{}};

    using Types = TypeList<
#include <iscore_plugin_autom3d_commands.hpp>
      >;
    for_each_type<Types>(iscore::commands::FactoryInserter{cmds.second});

    return cmds;
}

iscore::Version iscore_plugin_autom3d::version() const
{
    return iscore::Version{1};
}

UuidKey<iscore::Plugin> iscore_plugin_autom3d::key() const
{
    return "21f2eee9-5958-401d-a456-2e07b48063fb";
}

