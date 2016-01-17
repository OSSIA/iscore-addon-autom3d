#include <Explorer/PanelBase/DeviceExplorerPanelModel.hpp>
#include <Explorer/Widgets/AddressEditWidget.hpp>
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
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <iscore/tools/ModelPath.hpp>
#include <iscore/tools/Todo.hpp>

namespace Autom3D
{
InspectorWidget::InspectorWidget(
        const ProcessModel& autom3DModel,
        const iscore::DocumentContext& doc,
        QWidget* parent) :
    ProcessInspectorWidgetDelegate_T {autom3DModel, parent},
    m_dispatcher{doc.commandStack}
{

    using namespace DeviceExplorer;
    setObjectName("Autom3DInspectorWidget");
    setParent(parent);

    auto vlay = new QVBoxLayout;
    vlay->setSpacing(0);
    vlay->setContentsMargins(0,0,0,0);

    // LineEdit
    // If there is a DeviceExplorer in the current document, use it
    // to make a widget.
    // TODO instead of doing this, just make an address line edit factory.
    auto plug = doc.findPlugin<DeviceDocumentPlugin>();
    DeviceExplorerModel* explorer{};
    if(plug)
        explorer = plug->updateProxy.deviceExplorer;
    m_lineEdit = new AddressEditWidget{explorer, this};

    m_lineEdit->setAddress(process().address());
    con(process(), &ProcessModel::addressChanged,
            m_lineEdit, &AddressEditWidget::setAddress);

    connect(m_lineEdit, &AddressEditWidget::addressChanged,
            this, &InspectorWidget::on_addressChange);

    vlay->addWidget(m_lineEdit);

    // Min / max
    this->setLayout(vlay);
}

void InspectorWidget::on_addressChange(const ::State::Address& newAddr)
{
    // Various checks
    if(newAddr == process().address())
        return;

    if(newAddr.path.isEmpty())
        return;

    auto cmd = new ChangeAddress{process(), newAddr};

    m_dispatcher.submitCommand(cmd);
}
}
