#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
#include <QJsonObject>
#include <QJsonValue>
#include <algorithm>

#include "Autom3DLayerModel.hpp"
#include "Autom3DModel.hpp"
#include <State/Address.hpp>
#include <iscore/plugins/documentdelegate/plugin/ElementPluginModelList.hpp>
#include <iscore/serialization/JSONValueVisitor.hpp>
#include <iscore/serialization/VisitorCommon.hpp>
#include <iscore/tools/std/StdlibWrapper.hpp>

template<>
struct TSerializer<DataStream, std::vector<QVector3D>>
{
    static void readFrom(
            DataStream::Serializer& s,
            const std::vector<QVector3D>& vec)
    {
        s.stream() << (int32_t)vec.size();
        for(const auto& elt : vec)
            s.stream() << elt;
    }

    static void writeTo(
            DataStream::Deserializer& s,
            std::vector<QVector3D>& vec)
    {
        int32_t n = 0;
        s.stream() >> n;

        vec.clear();
        vec.resize(n);
        for(int i = 0; i < n; i++)
        {
            s.stream() >> vec[i];
        }
    }
};

template<>
void Visitor<Reader<DataStream>>::readFrom(const Autom3D::ProcessModel& autom)
{
    readFrom(*autom.pluginModelList);

    m_stream << autom.address();
    m_stream << autom.min();
    m_stream << autom.max();
    m_stream << autom.handles();

    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(Autom3D::ProcessModel& autom)
{
    autom.pluginModelList = new iscore::ElementPluginModelList{*this, &autom};

    State::Address address;
    Autom3D::Point min, max;
    std::vector<Autom3D::Point> handles;

    m_stream >> address >> min >> max >> handles;

    autom.setAddress(address);
    autom.setMin(min);
    autom.setMax(max);
    autom.setHandles(handles);

    checkDelimiter();
}


template<>
void Visitor<Reader<JSONValue>>::readFrom(const Autom3D::Point& pt)
{
    QJsonArray arr;
    arr.push_back(pt.x());
    arr.push_back(pt.y());
    arr.push_back(pt.z());
    val = arr;
}

template<>
void Visitor<Writer<JSONValue>>::writeTo(Autom3D::Point& pt)
{
    QJsonArray arr = val.toArray();
    pt = Autom3D::Point(arr[0].toDouble(), arr[1].toDouble(), arr[2].toDouble());
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const Autom3D::ProcessModel& autom)
{
    m_obj["PluginsMetadata"] = toJsonValue(*autom.pluginModelList);

    m_obj["Address"] = toJsonObject(autom.address());
    m_obj["Min"] = toJsonValue(autom.min());
    m_obj["Max"] = toJsonValue(autom.max());
    m_obj["Handles"] = toJsonValueArray(autom.handles());
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(Autom3D::ProcessModel& autom)
{
    Deserializer<JSONValue> elementPluginDeserializer(m_obj["PluginsMetadata"]);
    autom.pluginModelList = new iscore::ElementPluginModelList{elementPluginDeserializer, &autom};

    autom.setAddress(fromJsonObject<State::Address>(m_obj["Address"].toObject()));
    autom.setMin(fromJsonValue<Autom3D::Point>(m_obj["Min"]));
    autom.setMax(fromJsonValue<Autom3D::Point>(m_obj["Max"]));

    autom.setHandles(fromJsonValueArray<std::vector<Autom3D::Point>>(m_obj["Handles"].toArray()));
}


// Dynamic stuff
namespace Autom3D
{
void ProcessModel::serialize(const VisitorVariant& vis) const
{
    serialize_dyn(vis, *this);
}

Process::LayerModel* ProcessModel::loadLayer_impl(
        const VisitorVariant& vis,
        QObject* parent)
{
    return deserialize_dyn(vis, [&] (auto&& deserializer)
    {
        auto autom = new LayerModel{
                        deserializer, *this, parent};

        return autom;
    });
}
}
