#include <QObject>


#include "Autom3DProcessMetadata.hpp"
#include <Process/ProcessFactoryKey.hpp>

namespace Autom3D
{
const ProcessFactoryKey& ProcessMetadata::concreteFactoryKey()
{
    static const ProcessFactoryKey name{"7a460ae5-2dd1-4e51-aeaf-d8f304dc433c"};
    return name;
}

QString ProcessMetadata::processObjectName()
{
    return "Autom3D";
}

QString ProcessMetadata::factoryPrettyName()
{
    return QObject::tr("Autom3D");
}
}
