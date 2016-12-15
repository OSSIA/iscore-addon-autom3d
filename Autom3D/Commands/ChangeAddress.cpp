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
#include <iscore/model/path/Path.hpp>
#include <iscore/model/path/PathSerialization.hpp>
#include <iscore/model/tree/TreeNode.hpp>
#include <ossia/network/domain/domain.hpp>
#include <ossia/editor/value/value_conversion.hpp>
#include <ossia/editor/value/value.hpp>
namespace Autom3D
{
ChangeAddress::ChangeAddress(
        const ProcessModel& autom,
        const State::AddressAccessor &newval):
    m_path{autom},
    m_old{autom.address(), autom.min(), autom.max()},
    m_new{Explorer::makeFullAddressAccessorSettings(
              newval,
              iscore::IDocument::documentContext(autom),
              std::array<float, 3>{0., 0., 0.},
              std::array<float, 3>{1., 1., 1.})}
{
}


void ChangeAddress::undo() const
{
    auto& autom = m_path.find();

    auto& dom = m_old.domain.get();
    autom.setMin(dom.convert_min<std::array<float, 3>>());
    autom.setMax(dom.convert_max<std::array<float, 3>>());

    autom.setAddress(m_old.address);
}

void ChangeAddress::redo() const
{
    auto& autom = m_path.find();

    auto& dom = m_new.domain.get();
    autom.setMin(dom.convert_min<std::array<float, 3>>());
    autom.setMax(dom.convert_max<std::array<float, 3>>());

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
        std::vector<State::vec3f>&& newHandles):
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
