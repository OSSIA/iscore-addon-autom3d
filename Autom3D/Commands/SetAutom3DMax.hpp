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

        SetMin(const ProcessModel& path, State::vec3f newval):
            iscore::PropertyCommand{path, "min", QVariant::fromValue(newval)}
        {

        }
};

class SetMax final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetMax, "Set curve maximum")
        public:

        SetMax(const ProcessModel& path, State::vec3f newval):
            iscore::PropertyCommand{path, "max", QVariant::fromValue(newval)}
        {

        }
};

class SetScale final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetScale, "Set curve scale")
        public:

        SetScale(const ProcessModel& path, State::vec3f newval):
            iscore::PropertyCommand{path, "scale", QVariant::fromValue(newval)}
        {

        }
};

class SetOrigin final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetOrigin, "Set curve origin")
        public:

        SetOrigin(const ProcessModel& path, State::vec3f newval):
            iscore::PropertyCommand{path, "origin", QVariant::fromValue(newval)}
        {

        }
};


class SetUseDerivative final : public iscore::PropertyCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), SetUseDerivative, "Set curve derivative mode")
        public:

        SetUseDerivative(const ProcessModel& path, bool newval):
            iscore::PropertyCommand{path, "useDerivative", QVariant::fromValue(newval)}
        {

        }
};
}
