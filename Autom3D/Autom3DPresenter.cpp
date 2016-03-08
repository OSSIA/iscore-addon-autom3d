#include "Autom3DPresenter.hpp"
#include <iscore/document/DocumentContext.hpp>
#include <Autom3D/Autom3DView.hpp>

namespace Autom3D
{
LayerPresenter::LayerPresenter(
        const LayerModel& model,
        LayerView* view,
        QObject* parent):
    Process::LayerPresenter{"AudioLayerPresenter", parent},
    m_layer{model},
    m_view{view},
    m_focusDispatcher{iscore::IDocument::documentContext(model).document}
{
    connect(view, &LayerView::pressed,
            this, [&] () {
        m_focusDispatcher.focus(this);
    });
}

void LayerPresenter::setWidth(qreal val)
{
  m_view->setWidth(val);
}

void LayerPresenter::setHeight(qreal val)
{
  m_view->setHeight(val);
}

void LayerPresenter::putToFront()
{
  m_view->show();
}

void LayerPresenter::putBehind()
{
  m_view->hide();
}

void LayerPresenter::on_zoomRatioChanged(ZoomRatio r)
{
}

void LayerPresenter::parentGeometryChanged()
{
}

const LayerModel& LayerPresenter::layerModel() const
{
    return m_layer;
}

const Id<Process::ProcessModel>& LayerPresenter::modelId() const
{
    return processModel().id();
}

void LayerPresenter::fillContextMenu(
        QMenu*,
        const QPoint&,
        const QPointF&) const
{
}
}
