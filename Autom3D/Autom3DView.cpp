#include "Autom3DView.hpp"
#include <QPainter>
#include <Autom3D/Panel/AutomWidget.hpp>
#include <QGraphicsView>

#include <Autom3D/Panel/AutomImage.hpp>
namespace Autom3D
{
LayerView::LayerView(
        const ProcessModel& proc,
        QGraphicsItem* parent):
    Process::LayerView{parent},
    m_image{new AutomImage{proc}}
{
    connect(m_image, &AutomImage::changed,
            this, [&] { update(); });
}


void LayerView::paint_impl(QPainter* painter) const
{
    auto rect = boundingRect();
    auto img = m_image->getImage(rect.x(), rect.y());
    painter->drawImage(rect, img, img.rect());
}

void LayerView::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    emit pressed();
}
}
