

#include "Autom3DLayerModel.hpp"
#include "Autom3DModel.hpp"
#include "Autom3DPanelProxy.hpp"
#include <Process/LayerModel.hpp>

class QObject;
#include <iscore/tools/SettableIdentifier.hpp>

ISCORE_METADATA_IMPL(Autom3D::LayerModel)
// TODO refactor with mapping ?
namespace Autom3D
{
LayerModel::LayerModel(
            ProcessModel& model,
            const Id<Process::LayerModel>& id,
            QObject* parent) :
    Process::LayerModel {id, LayerModel::className.c_str(), model, parent}
{

}

LayerModel::LayerModel(
            const LayerModel& source,
            ProcessModel& model,
            const Id<Process::LayerModel>& id,
            QObject* parent) :
    Process::LayerModel {id, LayerModel::className.c_str(), model, parent}
{
    // Nothing to copy
}

Process::LayerModelPanelProxy* LayerModel::make_panelProxy(
            QObject* parent) const
{
    return new PanelProxy{*this, parent};
}

void LayerModel::serialize(
            const VisitorVariant&) const
{
    // Nothing to save
}

const ProcessModel& LayerModel::model() const
{
    return static_cast<const ProcessModel&>(processModel());
}
}
