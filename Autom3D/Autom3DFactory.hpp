#pragma once
#include <Process/GenericProcessFactory.hpp>
#include <Autom3D/Autom3DModel.hpp>
#include <Autom3D/Autom3DProcessMetadata.hpp>
#include <Autom3D/Autom3DLayerModel.hpp>
#include <Autom3D/Autom3DPresenter.hpp>
#include <Autom3D/Autom3DView.hpp>
#include <Autom3D/Panel/AutomWidget.hpp>
#include <Process/WidgetLayer/WidgetLayerPanelProxy.hpp>
namespace Autom3D
{
using ProcessFactory = Process::GenericProcessFactory<
    Autom3D::ProcessModel,
    Autom3D::Layer,
    Autom3D::LayerPresenter,
    Autom3D::LayerView,
    WidgetLayer::LayerPanelProxy<Autom3D::ProcessModel, AutomWidget>>;
}


