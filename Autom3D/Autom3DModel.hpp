#pragma once
#include <Process/Process.hpp>
#include <Autom3D/State/Autom3DState.hpp>
#include <Autom3D/Autom3DProcessMetadata.hpp>
#include <State/Address.hpp>
#include <QByteArray>
#include <QString>

#include <Autom3D/Point.hpp>

#include <Process/TimeValue.hpp>
#include <iscore/serialization/VisitorInterface.hpp>

class DataStream;
class JSONObject;
namespace Process { class LayerModel; }
namespace Process { class ProcessModel; }
class QObject;
#include <iscore/model/Identifier.hpp>
#include <iscore_plugin_autom3d_export.h>


namespace Autom3D
{

class ISCORE_PLUGIN_AUTOM3D_EXPORT ProcessModel final : public Process::ProcessModel
{
        ISCORE_SERIALIZE_FRIENDS
        MODEL_METADATA_IMPL(Autom3D::ProcessModel)

        Q_OBJECT
        Q_PROPERTY(::State::AddressAccessor address READ address WRITE setAddress NOTIFY addressChanged)
        // Min and max to scale the curve with at execution
        Q_PROPERTY(State::vec3f min READ min WRITE setMin NOTIFY minChanged)
        Q_PROPERTY(State::vec3f max READ max WRITE setMax NOTIFY maxChanged)
        Q_PROPERTY(State::vec3f scale READ scale WRITE setScale NOTIFY scaleChanged)
        Q_PROPERTY(State::vec3f origin READ origin WRITE setOrigin NOTIFY originChanged)
        Q_PROPERTY(bool useDerivative READ useDerivative WRITE setUseDerivative NOTIFY useDerivativeChanged)

    public:
        ProcessModel(const TimeValue& duration,
                     const Id<Process::ProcessModel>& id,
                     QObject* parent);

        template<typename Impl>
        ProcessModel(Impl& vis, QObject* parent) :
            Process::ProcessModel{vis, parent},
            m_startState{new ProcessState{*this, 0., this}},
            m_endState{new ProcessState{*this, 1., this}}
        {
            vis.writeTo(*this);
        }

        //// ProcessModel ////
        QString prettyName() const override;

        /// States
        ProcessState* startStateData() const override;
        ProcessState* endStateData() const override;

        //// Autom3DModel specifics ////
        ::State::AddressAccessor address() const;

        State::vec3f min() const;
        State::vec3f max() const;

        void setAddress(const ::State::AddressAccessor& arg);

        void setMin(State::vec3f arg);
        void setMax(State::vec3f arg);

        const std::vector<State::vec3f>& handles() const;
        void setHandles(const std::vector<State::vec3f>& hdl);

        void setScale(State::vec3f p);

        State::vec3f scale() const;

        void setOrigin(State::vec3f p);

        State::vec3f origin() const;

        bool useDerivative() const;
        void setUseDerivative(bool b);

    signals:
        void addressChanged(const ::State::AddressAccessor& arg);

        void minChanged(State::vec3f arg);
        void maxChanged(State::vec3f arg);

        void scaleChanged(State::vec3f arg);
        void originChanged(State::vec3f arg);

        void handlesChanged();

        void useDerivativeChanged(bool);

    protected:
        ProcessModel(const ProcessModel& source,
                        const Id<Process::ProcessModel>& id,
                        QObject* parent);

    private:
        ::State::AddressAccessor m_address;

        State::vec3f m_min{};
        State::vec3f m_max{};

        State::vec3f m_scale{50, 50, 50};
        State::vec3f m_origin{500, 500, 500};

        bool m_useDerivative = false;

        std::vector<State::vec3f> m_handles;
        ProcessState* m_startState{};
        ProcessState* m_endState{};
};
}

