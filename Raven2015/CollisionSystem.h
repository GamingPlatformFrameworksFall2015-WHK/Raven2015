/* Classname:   Gaming Platform Frameworks
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

#include "entityx\System.h"
#include "EventLibrary.h"

namespace Raven {

    class CollisionSystem : public ex::System<CollisionSystem>,
        public ex::Receiver<CollisionSystem> {

    public:
        /*
         *
         */
        explicit CollisionSystem() {
            eventToggle = true;
        }

        /*
         * Setup necessary static information
         */
        void configure(entityx::EventManager &event_manager) {
            event_manager.subscribe<CollisionEvent>(*this);
        }

        /*
         * Iterate through all objects with Colliders and emit CollisionEvents.
         */
        void update(ex::EntityManager &es, ex::EventManager &events,
            ex::TimeDelta dt) override;

        // Base receive
        template <typename T>
        void receiveEvent(const T &aEvent) {
            eventToggle = eventToggle ? response(aEvent) && false : true;
        }

        // Picks up CollisionEvents
        void receive(const CollisionEvent &event);
        
        //Processes the collision of two Entities with Colliders.
        bool response(const CollisionEvent &event);

        /*
         * Tests whether two entities' colliders register a collision.
         */
        std::unique_ptr<sf::Vector2f> CollisionSystem::testCollision(ex::Entity leftEntity,
            ex::Entity rightEntity);


    private:
        std::map<ex::Entity, std::set<ex::Entity>> collisionMap;
        int a;
        bool eventToggle;
    };

}
