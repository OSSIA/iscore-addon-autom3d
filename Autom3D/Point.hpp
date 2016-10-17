#pragma once
#include <QVector3D>
#include <State/Value.hpp>
#include <State/ValueConversion.hpp>
namespace Autom3D
{
using Point = QVector3D;
inline State::vec3f toTuple(Point p)
{
    return {p.x(), p.y(), p.z()};
}

inline Point fromTuple(State::vec3f t)
{
    return {t[0], t[1], t[2]};
}
}
