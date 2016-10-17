
#include <Explorer/Widgets/AddressAccessorEditWidget.hpp>
#include <iscore/widgets/SpinBoxes.hpp>
#include <QBoxLayout>
#include <QFormLayout>

#include <QPushButton>
#include <QSpinBox>
#include <QStringList>
#include <QWidget>
#include <algorithm>
#include <list>
#include <vector>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <Autom3D/Autom3DModel.hpp>
#include <Autom3D/Commands/ChangeAddress.hpp>
#include <Autom3D/Commands/SetAutom3DMax.hpp>
#include <Autom3D/Commands/SetAutom3DMin.hpp>
#include "Autom3DInspectorWidget.hpp"
#include <Inspector/InspectorWidgetBase.hpp>
#include <State/Address.hpp>
#include <iscore/widgets/SignalUtils.hpp>
#include <State/Widgets/Values/VecWidgets.hpp>

#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <iscore/tools/ModelPath.hpp>
#include <iscore/tools/Todo.hpp>
#include <QCheckBox>

namespace Autom3D
{
InspectorWidget::InspectorWidget(
        const Autom3D::ProcessModel& autom3DModel,
        const iscore::DocumentContext& doc,
        QWidget* parent) :
    InspectorWidgetDelegate_T {autom3DModel, parent},
    m_dispatcher{doc.commandStack}
{
    using namespace Explorer;
    setObjectName("Autom3DInspectorWidget");
    setParent(parent);

    auto vlay = new QFormLayout;
    vlay->setSpacing(0);
    vlay->setContentsMargins(0,0,0,0);

    // LineEdit
    // If there is a DeviceExplorer in the current document, use it
    // to make a widget.
    // TODO instead of doing this, just make an address line edit factory.
    auto plug = doc.findPlugin<Explorer::DeviceDocumentPlugin>();
    DeviceExplorerModel* explorer{};
    if(plug)
        explorer = &plug->explorer();
    m_lineEdit = new AddressAccessorEditWidget{explorer, this};

    m_lineEdit->setAddress(process().address());
    con(process(), &ProcessModel::addressChanged,
            m_lineEdit, &AddressAccessorEditWidget::setAddress);

    connect(m_lineEdit, &AddressAccessorEditWidget::addressChanged,
            this, &InspectorWidget::on_addressChange);

    vlay->addRow(tr("Address"), m_lineEdit);

    {
        auto scale = new State::Vec3DEdit{this};
        vlay->addRow(tr("Scale"), scale);
        scale->setValue(autom3DModel.scale());
        connect(scale, &State::Vec3DEdit::changed,
                this, [=] () {
            if(scale->value() != process().scale())
                m_dispatcher.submitCommand(new SetScale{process(), scale->value()});
        });
        con(autom3DModel, &ProcessModel::scaleChanged,
            this, [=] (State::vec3f s) {
            scale->setValue(s);
        });
    }

    {
        auto origin = new State::Vec3DEdit{this};
        vlay->addRow(tr("Origin"), origin);
        origin->setValue(autom3DModel.origin());
        connect(origin, &State::Vec3DEdit::changed,
                this, [=] () {
            if(origin->value() != process().origin())
            m_dispatcher.submitCommand(new SetOrigin{process(), origin->value()});
        });
        con(autom3DModel, &ProcessModel::originChanged,
            this, [=] (State::vec3f s) {
            origin->setValue(s);
        });
    }

    {
        auto min = new State::Vec3DEdit{this};
        vlay->addRow(tr("Min"), min);
        min->setValue(autom3DModel.min());
        connect(min, &State::Vec3DEdit::changed,
                this, [=] () {
            if(min->value() != process().min())
                m_dispatcher.submitCommand(new SetMin{process(), min->value()});
        });
        con(autom3DModel, &ProcessModel::minChanged,
            this, [=] (State::vec3f s) {
            min->setValue(s);
        });
    }

    {
        auto max = new State::Vec3DEdit{this};
        vlay->addRow(tr("Max"), max);
        max->setValue(autom3DModel.max());
        connect(max, &State::Vec3DEdit::changed,
                this, [=] () {
            if(max->value() != process().max())
            m_dispatcher.submitCommand(new SetMax{process(), max->value()});
        });
        con(autom3DModel, &ProcessModel::maxChanged,
            this, [=] (State::vec3f s) {
            max->setValue(s);
        });
    }

    {
        auto deriv = new QCheckBox{this};
        vlay->addRow(tr("Derivative"), deriv);
        deriv->setChecked(autom3DModel.useDerivative());
        connect(deriv, &QCheckBox::toggled,
                this, [=] (bool b) {
            if(b != process().useDerivative())
            m_dispatcher.submitCommand(new SetUseDerivative{process(), b});
        });
        con(autom3DModel, &ProcessModel::useDerivativeChanged,
            this, [=] (bool s) {
            deriv->setChecked(s);
        });
    }

    this->setLayout(vlay);
}

void InspectorWidget::on_addressChange(const ::State::AddressAccessor& newAddr)
{
    // Various checks
    if(newAddr == process().address())
        return;

    if(newAddr.address.path.isEmpty())
        return;

    m_dispatcher.submitCommand(new ChangeAddress{process(), newAddr});

}
}
