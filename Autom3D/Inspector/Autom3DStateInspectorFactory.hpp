#pragma once
#include <Inspector/InspectorWidgetFactoryInterface.hpp>
#include <QList>

class QObject;
class QString;
class QWidget;
namespace iscore {
class Document;
}  // namespace iscore


namespace Autom3D
{
class StateInspectorFactory final :
        public Inspector::InspectorWidgetFactory
{
        ISCORE_CONCRETE_FACTORY_DECL("77a6b896-2a29-4014-8156-fdb410408fea")
    public:
        StateInspectorFactory();

        Inspector::InspectorWidgetBase* makeWidget(
                const QList<const QObject*>& sourceElement,
                const iscore::DocumentContext& doc,
                QWidget* parent) const override;

        bool matches(const QList<const QObject*>& object) const override;
};

}
