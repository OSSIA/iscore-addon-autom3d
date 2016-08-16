#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
template <typename T> class Reader;
template <typename T> class Writer;

namespace Autom3D
{
class Layer;
}
/////// ViewModel
template<>
void Visitor<Reader<DataStream>>::readFrom(const Autom3D::Layer& lm)
{
}

template<>
void Visitor<Writer<DataStream>>::writeTo(Autom3D::Layer& lm)
{
}



template<>
void Visitor<Reader<JSONObject>>::readFrom(const Autom3D::Layer& lm)
{
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(Autom3D::Layer& lm)
{
}
