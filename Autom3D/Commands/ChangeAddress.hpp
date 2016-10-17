#pragma once
#include <Autom3D/Commands/Autom3DCommandFactory.hpp>
#include <Autom3D/Point.hpp>
#include <Device/Address/AddressSettings.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

struct DataStreamInput;
struct DataStreamOutput;
namespace State {
struct Address;
}  // namespace iscore

namespace Autom3D
{
class ProcessModel;
class ChangeAddress final : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), ChangeAddress, "ChangeAddress")
    public:
        ChangeAddress(
                Path<ProcessModel>&& path,
                const State::AddressAccessor& newval);

    public:
        void undo() const override;
        void redo() const override;

    protected:
        void serializeImpl(DataStreamInput &) const override;
        void deserializeImpl(DataStreamOutput &) override;

    private:
        Path<ProcessModel> m_path;
        State::AddressAccessor m_oldAddress, m_newAddress;
        Device::Domain m_oldDomain, m_newDomain;
};

// MOVEME
class UpdateSpline final : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), UpdateSpline, "UpdateSpline")
    public:
        UpdateSpline(
                Path<ProcessModel>&& path,
                std::vector<State::vec3f>&& newHandles);

    public:
        void undo() const override;
        void redo() const override;

    protected:
        void serializeImpl(DataStreamInput &) const override;
        void deserializeImpl(DataStreamOutput &) override;

    private:
        Path<ProcessModel> m_path;
        std::vector<State::vec3f> m_old;
        std::vector<State::vec3f> m_new;
};
}
