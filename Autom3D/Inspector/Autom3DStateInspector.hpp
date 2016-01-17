#pragma once

#include <Inspector/InspectorWidgetBase.hpp>

class QLabel;
class QWidget;
namespace iscore {
class Document;
}  // namespace iscore

namespace Autom3D
{
class ProcessState;
class StateInspectorWidget final : public Inspector::InspectorWidgetBase
{
    public:
        explicit StateInspectorWidget(
                const ProcessState& object,
                const iscore::DocumentContext& context,
                QWidget* parent = 0);

    private:
        void on_stateChanged();

        const ProcessState& m_state;
        QLabel* m_label{};
};
}
