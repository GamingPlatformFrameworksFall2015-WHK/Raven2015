#pragma once
#include "Common.h"
#include "ComponentLibrary.h"

namespace Raven {

    namespace EntityLibrary {

        struct Create {

            static ex::Entity Entity(std::string entityName = "Default Entity") {
                ex::Entity e = cmn::entities->create();
                e.assign<Data>()->name = entityName + " " + std::to_string(counter++);
                e.assign<Transform>();
                e.assign<Rigidbody>();
                return e;
            }

            static ex::Entity Player(std::string playerName = "Default Player") {
                ex::Entity e = Entity(playerName);
                e.assign<Pawn>();
                e.assign<BoxCollider>();
                e.assign<Renderer>();
                // Assign a RenderableSprite to the Player character here
            }

        private:
            static unsigned int counter;

        };
    }
}