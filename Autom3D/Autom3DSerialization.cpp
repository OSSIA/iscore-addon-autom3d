#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
#include <QJsonObject>
#include <QJsonValue>
#include <algorithm>

#include "Autom3DLayerModel.hpp"
#include "Autom3DModel.hpp"
#include <State/Address.hpp>
#include <iscore/serialization/JSONValueVisitor.hpp>
#include <iscore/serialization/VisitorCommon.hpp>

template <>
void DataStreamReader::read(const Autom3D::ProcessModel& autom)
{
    m_stream << autom.address();
    m_stream << autom.handles();
    m_stream << autom.min();
    m_stream << autom.max();
    m_stream << autom.scale();
    m_stream << autom.origin();
    m_stream << autom.useDerivative();

    insertDelimiter();
}

template <>
void DataStreamWriter::write(Autom3D::ProcessModel& autom)
{
    State::AddressAccessor address;
    State::vec3f min, max, scale, origin;
    std::vector<State::vec3f> handles;
    bool useDerivative;

    m_stream >> address >> handles >> min >> max >> scale >> origin >> useDerivative;

    autom.setAddress(address);
    autom.setHandles(handles);
    autom.setMin(min);
    autom.setMax(max);
    autom.setScale(scale);
    autom.setOrigin(origin);
    autom.setUseDerivative(useDerivative);

    checkDelimiter();
}

template <>
void JSONObjectReader::read(const Autom3D::ProcessModel& autom)
{
    obj[strings.Address] = toJsonObject(autom.address());
    obj[strings.Min] = toJsonValue(autom.min());
    obj[strings.Max] = toJsonValue(autom.max());
    obj["Scale"] = toJsonValue(autom.scale());
    obj["Origin"] = toJsonValue(autom.origin());
    obj["Derivative"] = autom.useDerivative();
    obj["Handles"] = toJsonValueArray(autom.handles());
}

template <>
void JSONObjectWriter::write(Autom3D::ProcessModel& autom)
{
    autom.setAddress(fromJsonObject<State::AddressAccessor>(obj[strings.Address]));
    autom.setMin(fromJsonValue<State::vec3f>(obj[strings.Min]));
    autom.setMax(fromJsonValue<State::vec3f>(obj[strings.Max]));
    autom.setScale(fromJsonValue<State::vec3f>(obj["Scale"]));
    autom.setOrigin(fromJsonValue<State::vec3f>(obj["Origin"]));
    autom.setUseDerivative(obj["Derivative"].toBool());

    autom.setHandles(fromJsonValueArray<std::vector<State::vec3f>>(obj["Handles"].toArray()));
}
