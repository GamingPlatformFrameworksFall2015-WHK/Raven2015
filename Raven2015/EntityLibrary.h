/*
* Classname:   Gaming Platform Frameworks
* Project:     Raven
* Version:     1.0
*
* Copyright:   The contents of this document are the property of its creators.
*              Reproduction or usage of it without permission is prohibited.
*
* Owners:      Will Nations
*              Hailee Ammons
*              Kevin Wang
*/
#pragma once

#include "Common.h"
#include "entityx\Entity.h"
#include "ComponentLibrary.h"

namespace ex = entityx;

namespace Raven {

    /*
     * Base class for creating pre-defined entities.
     * Will later try to adopt Unity-like Prefab architecture if there's time.
     */
    class Entity : public ex::Entity {
    public:
        Entity() {}

        Entity(ex::Entity &entity) {
            *this = entity;
        }

        Entity &operator=(const ex::Entity &entity) {
            id_ = entity.id();
            manager_ = entity.manager();
        }

    };

    /*
     * Actors are entities that have a precise location in the game space
     */
    class Actor : public Entity {
    public:
        // Default Constructor
        Actor() {
            assign<Transform>();
        }
    };


}
