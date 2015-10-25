#include "Common.h"

namespace Raven {

    namespace Common {

        const std::set<std::string> CollisionInformation::settings({ "Solid", "Fixed" });

        extern ex::EntityManager* entities = nullptr;
        extern ex::EventManager* events = nullptr;
        extern Game* game = nullptr;
    }
}
