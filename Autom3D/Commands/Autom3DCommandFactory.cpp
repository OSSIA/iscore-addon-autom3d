#include "Autom3DCommandFactory.hpp"
#include <iscore/command/Command.hpp>

namespace Autom3D
{
const CommandParentFactoryKey& CommandFactoryName() {
    static const CommandParentFactoryKey key{"Autom3D"};
    return key;
}
}
