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

#include "CollisionSystem.h"    // 
#include "ComponentLibrary.h"   // For the colliders
#include "entityx\Entity.h"     // For entityx::Entity
#include <algorithm>            // For std::min

/*
 * Iterate through all objects with Colliders and emit CollisionEvents.
 */
void CollisionSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {

    es.each<BoxCollider>([&](ex::Entity leftEntity, BoxCollider &leftCollider) {
        es.each<BoxCollider>([&](ex::Entity rightEntity, BoxCollider &rightCollider) {
            testCollision(leftEntity, rightEntity, events);
        });
    });
}

/*
 * Processes data involving the collision of two Entities with Colliders.
 */
void CollisionSystem::receive(const CollisionEvent &event) {

    /*
    // Notify the user that a collision occurred via text output
    cout << "Collision: " + std::to_string(event.leftEntity.id().id()) + " X " +
        std::to_string(event.rightEntity.id().id()) << endl;
    */

}

/*
 * Tests whether two entities' colliders register a collision.
 */
void CollisionSystem::testCollision(ex::Entity leftEntity, 
    ex::Entity rightEntity, ex::EventManager &events) {

    // Grab relevant components from the left entity
    ex::ComponentHandle<Transform> leftTransform =
        leftEntity.component<Transform>();
    ex::ComponentHandle<BoxCollider> leftBoxCollider = 
        leftEntity.component<BoxCollider>();
    /*ex::ComponentHandle<CircleCollider> leftCircleCollider = 
    leftEntity.component<CircleCollider>();*/

    // Grab relevant components from the right entity
    ex::ComponentHandle<Transform> rightTransform =
        rightEntity.component<Transform>();
    ex::ComponentHandle<BoxCollider> rightBoxCollider = 
        rightEntity.component<BoxCollider>();
    /*ex::ComponentHandle<CircleCollider> rightCircleCollider = 
    leftEntity.component<CircleCollider>();*/

    // Check type of collision

    // Calculate the exact collision originOffsets
    float rightColliderX = rightTransform.get()->transform.x + 
        rightBoxCollider.get()->originOffset.x;
    float leftColliderX = leftTransform.get()->transform.x + 
        leftBoxCollider.get()->originOffset.x;
    float rightColliderY = rightTransform.get()->transform.y + 
        rightBoxCollider.get()->originOffset.y;
    float leftColliderY = leftTransform.get()->transform.y + 
        leftBoxCollider.get()->originOffset.y;

    //Define variables for tracking x and y coordinates of impact point.
    bool xcollided = false;
    bool ycollided = false;

    // Calculate the distances apart they are
    float xDiff = abs(leftColliderX - rightColliderX);
    float yDiff = abs(leftColliderY - rightColliderY);

    // Assuming the boxes are axis-aligned, check whether they overlap
    if (xDiff * 2 <
        (leftBoxCollider.get()->width + rightBoxCollider.get()->width) &&
        yDiff * 2 <
        (leftBoxCollider.get()->height + rightBoxCollider.get()->height)) {

        sf::Vector2f collisionPoint(xDiff * 0.5, yDiff * 0.5);
        collisionPoint.x += std::min(leftColliderX, rightColliderX);
        collisionPoint.y += std::min(leftColliderY, rightColliderY);

        events.emit<CollisionEvent>(
            leftEntity, rightEntity, collisionPoint, &events);
    }
}

