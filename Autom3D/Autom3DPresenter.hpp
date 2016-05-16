#pragma once
#include <Autom3D/Autom3DModel.hpp>
#include <Autom3D/Autom3DLayerModel.hpp>
#include <Process/LayerPresenter.hpp>
#include <Process/Focus/FocusDispatcher.hpp>

namespace Autom3D
{
class LayerModel;
class LayerView;
class LayerPresenter final :
        public Process::LayerPresenter
{
    public:
        explicit LayerPresenter(
                const LayerModel& model,
                LayerView* view,
                const Process::ProcessPresenterContext& ctx,
                QObject* parent);

        void setWidth(qreal width) override;
        void setHeight(qreal height) override;

        void putToFront() override;
        void putBehind() override;

        void on_zoomRatioChanged(ZoomRatio) override;

        void parentGeometryChanged() override;

        const LayerModel& layerModel() const override;
        const ProcessModel& processModel() const
        { return static_cast<ProcessModel&>(m_layer.processModel()); }
        const Id<Process::ProcessModel>& modelId() const override;

        void fillContextMenu(
                QMenu*,
                const QPoint& pos,
                const QPointF& scenepos) const override;

    private:
        const LayerModel& m_layer;
        LayerView* m_view{};
        ZoomRatio m_ratio{1};
};

}
