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

    //
    // Actors are entities that have a precise location in the game space
    //
    class Actor : public Entity {
    public:
        // Default Constructor
        Actor() {
            assign<Transform>();
        }
    };

    //
    // Agents are Actors that listen for user-defined input actions and react accordingly by default
    //
    class Agent : public Actor {
    public:
        // Default Constructor
        Agent() : Actor() {
            assign<ActionListener>();
        }
    };

    //
    // Automatons are Actors that react to system-defined events
    //
    class Automaton : public Actor {
    public:
        // Default Constructor
        Automaton() : Actor() {
            assign<CoreBehavior>();
        }
    };
    */

}
