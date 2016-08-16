#include "Autom3DLayerModel.hpp"
#include "Autom3DModel.hpp"
#include "Autom3DPanelProxy.hpp"
#include <Process/LayerModel.hpp>
#include <iscore/document/DocumentContext.hpp>
#include <Autom3D/Panel/AutomWidget.hpp>
class QObject;
#include <iscore/tools/SettableIdentifier.hpp>

// TODO refactor with mapping ?
namespace Autom3D
{
Layer::Layer(
            ProcessModel& model,
            const Id<Process::LayerModel>& id,
            QObject* parent) :
    Process::LayerModel {id, Metadata<ObjectKey_k, Layer>::get(), model, parent}
{
  init(model);
}

Layer::Layer(
            const Layer& source,
            ProcessModel& model,
            const Id<Process::LayerModel>& id,
            QObject* parent) :
    Process::LayerModel {id, Metadata<ObjectKey_k, Layer>::get(), model, parent}
{
  init(model);
}

Process::LayerModelPanelProxy* Layer::make_panelProxy(
            QObject* parent) const
{
    return new PanelProxy{*this, parent};
}

void Layer::serialize_impl(
            const VisitorVariant&) const
{
    // Nothing to save
}

const ProcessModel& Layer::model() const
{
  return static_cast<const ProcessModel&>(processModel());
}

AutomWidget* Layer::automationWidget() const
{
  return m_widget;
}

void Layer::init(ProcessModel& model)
{
  m_widget = new AutomWidget{
                   model,
                  iscore::IDocument::documentContext(model).commandStack};

}
}
