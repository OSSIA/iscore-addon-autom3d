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
        Q_PROPERTY(::State::Address address READ address WRITE setAddress NOTIFY addressChanged)
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
        ::State::Address address() const;

        Point min() const;
        Point max() const;

        void setAddress(const ::State::Address& arg);

        void setMin(Point arg);
        void setMax(Point arg);

        auto& handles() const
        { return m_handles; }
        void setHandles(const std::vector<Point>& hdl)
        {
            m_handles = hdl;
            emit handlesChanged();
        }

        void setScale(Point p)
        {
            m_scale = p;
            emit scaleChanged(p);
        }

        Point scale()
        {
            return m_scale;
        }

        void setOrigin(Point p)
        {
            m_origin = p;
            emit originChanged(p);
        }

        Point origin()
        {
            return m_origin;
        }

        bool useDerivative() const
        { return m_useDerivative; }
        void setUseDerivative(bool b)
        { m_useDerivative = b; }

    signals:
        void addressChanged(const ::State::Address& arg);

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
        ::State::Address m_address;

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
