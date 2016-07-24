#include <Device/Node/DeviceNode.hpp>
#include <Explorer/Explorer/DeviceExplorerModel.hpp>
#include <QString>
#include <QStringList>
#include <algorithm>

#include <Autom3D/Autom3DModel.hpp>
#include "ChangeAddress.hpp"
#include <Device/Address/AddressSettings.hpp>
#include <Device/Address/Domain.hpp>
#include <State/Address.hpp>
#include <State/Value.hpp>
#include <State/ValueConversion.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/tools/ModelPath.hpp>
#include <iscore/tools/ModelPathSerialization.hpp>
#include <iscore/tools/TreeNode.hpp>
namespace Autom3D
{
ChangeAddress::ChangeAddress(
        Path<ProcessModel> &&path,
        const ::State::Address &newval):
    m_path{std::move(path)} // TODO check for std::move everywhere
{
    auto& autom = m_path.find();

    // Get the current data.
    m_old.address = autom.address();
    m_old.domain.min.val = toTuple(autom.min());
    m_old.domain.max.val = toTuple(autom.max());


    if(auto deviceexplorer = Explorer::try_deviceExplorerFromObject(autom))
    {
        // Note : since we change the address, we also have to update the min / max if possible.
        // To do this, we must go and check into the device explorer.
        // If the node isn't found, we fallback on common values.

        // Get the new data.
        auto newpath = newval.path;
        newpath.prepend(newval.device);
        auto new_n = Device::try_getNodeFromString(deviceexplorer->rootNode(), std::move(newpath));
        if(new_n)
        {
            ISCORE_ASSERT(!new_n->is<Device::DeviceSettings>());
            m_new = Device::FullAddressSettings::make<Device::FullAddressSettings::as_child>(new_n->get<Device::AddressSettings>(), newval);
        }
        else
        {
            m_new.address = newval;
            m_new.domain.min.val = State::tuple_t{0., 0., 0.};
            m_new.domain.max.val = State::tuple_t{1., 1., 1.};
        }
    }
}


void ChangeAddress::undo() const
{
    auto& autom = m_path.find();

    autom.setMin(fromTuple(::State::convert::value<State::tuple_t>(m_old.domain.min)));
    autom.setMax(fromTuple(::State::convert::value<State::tuple_t>(m_old.domain.max)));

    autom.setAddress(m_old.address);
}

void ChangeAddress::redo() const
{
    auto& autom = m_path.find();

    autom.setMin(fromTuple(::State::convert::value<State::tuple_t>(m_new.domain.min)));
    autom.setMax(fromTuple(::State::convert::value<State::tuple_t>(m_new.domain.max)));

    autom.setAddress(m_new.address);
}

void ChangeAddress::serializeImpl(DataStreamInput & s) const
{
    s << m_path << m_old << m_new;
}

void ChangeAddress::deserializeImpl(DataStreamOutput & s)
{
    s >> m_path >> m_old >> m_new;
}




UpdateSpline::UpdateSpline(
        Path<ProcessModel> &&path,
        std::vector<Point>&& newHandles):
    m_path{std::move(path)},
    m_new{std::move(newHandles)}
{
    auto& autom = m_path.find();
    m_old = autom.handles();
}


void UpdateSpline::undo() const
{
    auto& autom = m_path.find();

    autom.setHandles(m_old);
}

void UpdateSpline::redo() const
{
    auto& autom = m_path.find();

    autom.setHandles(m_new);
}

void UpdateSpline::serializeImpl(DataStreamInput& s) const
{
    s << m_path << m_old << m_new;
}

void UpdateSpline::deserializeImpl(DataStreamOutput& s)
{
    s >> m_path >> m_old >> m_new;
}
}
