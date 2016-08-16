#pragma once

#include <iscore/tools/Metadata.hpp>
#include <Process/LayerModel.hpp>

#include <iscore/serialization/VisitorInterface.hpp>
#include <iscore/tools/SettableIdentifier.hpp>

class QObject;

namespace Autom3D
{
class AutomWidget;
class ProcessModel;
class Layer final : public Process::LayerModel
{
    public:
        Layer(
                ProcessModel& model,
                const Id<Process::LayerModel>& id,
                QObject* parent);

        // Copy
        Layer(
                const Layer& source,
                ProcessModel& model,
                const Id<Process::LayerModel>& id,
                QObject* parent);

        // Load
        template<typename Impl>
        Layer(
                Deserializer<Impl>& vis,
                ProcessModel& model,
                QObject* parent) :
            Process::LayerModel {vis, model, parent}
        {
            vis.writeTo(*this);
            init(model);
        }

        Process::LayerModelPanelProxy* make_panelProxy(QObject* parent) const override;
        void serialize_impl(const VisitorVariant&) const override;

        const ProcessModel& model() const;

        AutomWidget* automationWidget() const;

  private:
        void init(ProcessModel& );
        AutomWidget* m_widget{};
};
}

DEFAULT_MODEL_METADATA(Autom3D::Layer, "3D Automation Layer")
