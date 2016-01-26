#include <QString>

#include "Autom3DStateInspector.hpp"
#include "Autom3DStateInspectorFactory.hpp"
#include <Inspector/InspectorWidgetFactoryInterface.hpp>
#include <Autom3D/State/Autom3DState.hpp>
class QObject;
class QWidget;
namespace iscore {
class Document;
}  // namespace iscore

namespace Autom3D
{
StateInspectorFactory::StateInspectorFactory() :
    InspectorWidgetFactory {}
{

}

Inspector::InspectorWidgetBase* StateInspectorFactory::makeWidget(
        const QObject& sourceElement,
        const iscore::DocumentContext& doc,
        QWidget* parent) const
{
    return new StateInspectorWidget{
                safe_cast<const ProcessState&>(sourceElement),
                doc,
                parent};
}

const QList<QString>&StateInspectorFactory::key_impl() const
{
    static const QList<QString> lst{"Autom3DState"};
    return lst;
}

bool StateInspectorFactory::matches(const QObject& object) const
{
    return dynamic_cast<const Autom3D::ProcessState*>(&object);
}
}
