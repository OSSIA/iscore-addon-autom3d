#pragma once
#include <Process/LayerModel.hpp>
#include <Autom3D/Autom3DModel.hpp>

namespace Autom3D
{
using Layer = Process::LayerModel_T<ProcessModel>;
}

DEFAULT_MODEL_METADATA(Autom3D::Layer, "Autom3DLayer")
