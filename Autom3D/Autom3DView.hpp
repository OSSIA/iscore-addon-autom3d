#pragma once
#include <Process/LayerView.hpp>

namespace Autom3D
{
class AutomImage;
class ProcessModel;
class LayerView final :
    public Process::LayerView
{
        Q_OBJECT
    public:
        explicit LayerView(
        const ProcessModel& ,
        QGraphicsItem* parent);

    signals:
        void pressed();

    private:
        void paint_impl(QPainter*) const override;
        void mousePressEvent(QGraphicsSceneMouseEvent*) override;

        AutomImage* m_image;
};
}
