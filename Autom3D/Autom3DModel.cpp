#include <iscore/tools/std/Optional.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <QDebug>
#include <QPoint>

#include <Autom3D/Autom3DProcessMetadata.hpp>
#include "Autom3DLayerModel.hpp"
#include "Autom3DModel.hpp"
#include <iscore/model/ModelMetadata.hpp>
#include <State/Address.hpp>
#include <Autom3D/State/Autom3DState.hpp>
#include <iscore/tools/IdentifiedObjectMap.hpp>
#include <iscore/tools/SettableIdentifier.hpp>

namespace Process { class LayerModel; }
namespace Process { class ProcessModel; }
class QObject;
namespace Autom3D
{
ProcessModel::ProcessModel(
        const TimeValue& duration,
        const Id<Process::ProcessModel>& id,
        QObject* parent) :
    Process::ProcessModel {duration, id, Metadata<ObjectKey_k, ProcessModel>::get(), parent},
    m_startState{new ProcessState{*this, 0., this}},
    m_endState{new ProcessState{*this, 1., this}}
{
    metadata().setInstanceName(*this);
    m_handles.emplace_back(-0.5, -0.5, -0.5);
    m_handles.emplace_back(0, 0, 0);
    m_handles.emplace_back(0.5, 0.5, 0.5);
}

ProcessModel::ProcessModel(
        const ProcessModel& source,
        const Id<Process::ProcessModel>& id,
        QObject* parent):
    Process::ProcessModel {source, id, Metadata<ObjectKey_k, ProcessModel>::get(), parent},
    m_address(source.address()),
    m_min{source.min()},
    m_max{source.max()},
    m_handles{source.handles()},
    m_startState{new ProcessState{*this, 0., this}},
    m_endState{new ProcessState{*this, 1., this}}
{
    metadata().setInstanceName(*this);
}

QString ProcessModel::prettyName() const
{
    return metadata().getName() + " : " + address().toString();
}

ProcessState* ProcessModel::startStateData() const
{
    return m_startState;
}

ProcessState* ProcessModel::endStateData() const
{
    return m_endState;
}

::State::AddressAccessor ProcessModel::address() const
{
    return m_address;
}

Point ProcessModel::min() const
{
    return m_min;
}

Point ProcessModel::max() const
{
    return m_max;
}

void ProcessModel::setAddress(const State::AddressAccessor &arg)
{
    if(m_address == arg)
    {
        return;
    }

    m_address = arg;
    emit addressChanged(arg);
}

void ProcessModel::setMin(Point arg)
{
    if (m_min == arg)
        return;

    m_min = arg;
    emit minChanged(arg);
}

void ProcessModel::setMax(Point arg)
{
    if (m_max == arg)
        return;

    m_max = arg;
    emit maxChanged(arg);
}

const std::vector<Point>& ProcessModel::handles() const
{ return m_handles; }

void ProcessModel::setHandles(const std::vector<Point>& hdl)
{
  m_handles = hdl;
  emit handlesChanged();
}

void ProcessModel::setScale(Point p)
{
  m_scale = p;
  emit scaleChanged(p);
}

Point ProcessModel::scale()
{
  return m_scale;
}

void ProcessModel::setOrigin(Point p)
{
  m_origin = p;
  emit originChanged(p);
}

Point ProcessModel::origin()
{
  return m_origin;
}

bool ProcessModel::useDerivative() const
{ return m_useDerivative; }

void ProcessModel::setUseDerivative(bool b)
{ m_useDerivative = b; }
}
