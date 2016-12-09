#include "Autom3DCommandFactory.hpp"
#include <iscore/command/Command.hpp>

namespace Autom3D
{
const CommandGroupKey& CommandFactoryName() {
    static const CommandGroupKey key{"Autom3D"};
    return key;
}
}
