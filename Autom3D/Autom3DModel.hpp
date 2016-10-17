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
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore_plugin_autom3d_export.h>


namespace Autom3D
{

class ISCORE_PLUGIN_AUTOM3D_EXPORT ProcessModel final : public Process::ProcessModel
{
        ISCORE_SERIALIZE_FRIENDS(ProcessModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(ProcessModel, JSONObject)
        MODEL_METADATA_IMPL(Autom3D::ProcessModel)

        Q_OBJECT
        Q_PROPERTY(::State::AddressAccessor address READ address WRITE setAddress NOTIFY addressChanged)
        // Min and max to scale the curve with at execution
        Q_PROPERTY(Point min READ min WRITE setMin NOTIFY minChanged)
        Q_PROPERTY(Point max READ max WRITE setMax NOTIFY maxChanged)

    public:
        ProcessModel(const TimeValue& duration,
                     const Id<Process::ProcessModel>& id,
                     QObject* parent);

        template<typename Impl>
        ProcessModel(Deserializer<Impl>& vis, QObject* parent) :
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

        Point min() const;
        Point max() const;

        void setAddress(const ::State::AddressAccessor& arg);

        void setMin(Point arg);
        void setMax(Point arg);

        const std::vector<Point>& handles() const;
        void setHandles(const std::vector<Point>& hdl);

        void setScale(Point p);

        Point scale();

        void setOrigin(Point p);

        Point origin();

        bool useDerivative() const;
        void setUseDerivative(bool b);

    signals:
        void addressChanged(const ::State::AddressAccessor& arg);

        void minChanged(Point arg);
        void maxChanged(Point arg);

        void scaleChanged(Point arg);
        void originChanged(Point arg);

        void handlesChanged();

    protected:
        ProcessModel(const ProcessModel& source,
                        const Id<Process::ProcessModel>& id,
                        QObject* parent);

    private:
        ::State::AddressAccessor m_address;

        Point m_min{};
        Point m_max{};

        Point m_scale{50, 50, 50};
        Point m_origin{500, 500, 500};

        bool m_useDerivative = false;

        std::vector<Point> m_handles;
        ProcessState* m_startState{};
        ProcessState* m_endState{};
};
}
Q_DECLARE_METATYPE(Autom3D::Point)
