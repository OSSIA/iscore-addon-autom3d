#pragma once
#include <iscore/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp>
#include <iscore/plugins/qt_interfaces/GUIApplicationContextPlugin_QtInterface.hpp>
#include <QObject>
#include <utility>
#include <vector>

#include <iscore/application/ApplicationContext.hpp>
#include <iscore/command/CommandGeneratorMap.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/plugins/customfactory/FactoryInterface.hpp>

class iscore_plugin_autom3d final :
        public QObject,
        public iscore::FactoryInterface_QtInterface,
        public iscore::CommandFactory_QtInterface
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID FactoryInterface_QtInterface_iid)
        Q_INTERFACES(
                iscore::FactoryInterface_QtInterface
                iscore::CommandFactory_QtInterface
                )

    public:
        iscore_plugin_autom3d();
        virtual ~iscore_plugin_autom3d() = default;

        // Process & inspector
        std::vector<std::unique_ptr<iscore::FactoryInterfaceBase>> factories(
                const iscore::ApplicationContext& ctx,
                const iscore::AbstractFactoryKey& factoryName) const override;

        // CommandFactory_QtInterface interface
        std::pair<const CommandParentFactoryKey, CommandGeneratorMap> make_commands() override;
};
