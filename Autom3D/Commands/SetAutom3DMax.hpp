#pragma once
#include <Autom3D/Commands/Autom3DCommandFactory.hpp>
#include <iscore/command/PropertyCommand.hpp>
#include <Autom3D/Point.hpp>

namespace Autom3D
{
class ProcessModel;

class SetMin final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetMin, "Set curve minimum")
    public:

        SetMin(Path<ProcessModel>&& path, State::vec3f newval):
            iscore::PropertyCommand{std::move(path), "min", QVariant::fromValue(newval)}
        {

        }
};

class SetMax final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetMax, "Set curve maximum")
        public:

        SetMax(Path<ProcessModel>&& path, State::vec3f newval):
            iscore::PropertyCommand{std::move(path), "max", QVariant::fromValue(newval)}
        {

        }
};

class SetScale final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetScale, "Set curve scale")
        public:

        SetScale(Path<ProcessModel>&& path, State::vec3f newval):
            iscore::PropertyCommand{std::move(path), "scale", QVariant::fromValue(newval)}
        {

        }
};

class SetOrigin final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetOrigin, "Set curve origin")
        public:

        SetOrigin(Path<ProcessModel>&& path, State::vec3f newval):
            iscore::PropertyCommand{std::move(path), "origin", QVariant::fromValue(newval)}
        {

        }
};


class SetUseDerivative final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetUseDerivative, "Set curve derivative mode")
        public:

        SetUseDerivative(Path<ProcessModel>&& path, bool newval):
            iscore::PropertyCommand{std::move(path), "useDerivative", QVariant::fromValue(newval)}
        {

        }
};
}
