/*
#pragma once
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <Autom3D/State/Autom3DState.hpp>

class QLabel;
namespace Autom3D
{
class ProcessState;
class StateInspectorWidget final :
        public Process::StateProcessInspectorWidgetDelegate_T<ProcessState>
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
*/
