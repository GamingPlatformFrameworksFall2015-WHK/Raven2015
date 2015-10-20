#include "Common.h"

namespace Raven {

    namespace Common {

        extern ex::EntityManager* entities = nullptr;
        extern ex::EventManager* events = nullptr;
        extern Game* game = nullptr;
        
		const std::set<std::string> CollisionInformation::settings({ COLLISION_LAYER_SETTINGS_SOLID, COLLISION_LAYER_SETTINGS_FIXED });
    }
}
