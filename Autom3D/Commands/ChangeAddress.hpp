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
                const State::Address& newval);

    public:
        void undo() const override;
        void redo() const override;

    protected:
        void serializeImpl(DataStreamInput &) const override;
        void deserializeImpl(DataStreamOutput &) override;

    private:
        Path<ProcessModel> m_path;
        Device::FullAddressSettings m_old, m_new;
};

class UpdateSpline final : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), UpdateSpline, "UpdateSpline")
    public:
        UpdateSpline(
                Path<ProcessModel>&& path,
                std::vector<Point>&& newHandles);

    public:
        void undo() const override;
        void redo() const override;

    protected:
        void serializeImpl(DataStreamInput &) const override;
        void deserializeImpl(DataStreamOutput &) override;

    private:
        Path<ProcessModel> m_path;
        std::vector<Point> m_old;
        std::vector<Point> m_new;
};
}
