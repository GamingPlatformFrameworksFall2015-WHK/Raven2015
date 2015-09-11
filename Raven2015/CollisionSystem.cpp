#include "CollisionSystem.h"
#include "ComponentLibrary.h"
#include "entityx\Entity.h"
#include <typeinfo>

using namespace Raven;

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

    // Notify the user that a collision occurred via text output
    cout << "Collision: " + std::to_string(event.leftEntity.id().id()) + " X " + 
        std::to_string(event.rightEntity.id().id()) << endl;

    //event.events->emit<AudioEvent>(nullptr,"sample_audio_file.wav",)
}

/*
 * Tests whether two entities' colliders register a collision.
 */
void CollisionSystem::testCollision(ex::Entity leftEntity, 
    ex::Entity rightEntity, ex::EventManager &events) {

    // Grab relevant components (currently assuming only BoxColliders)
    ex::ComponentHandle<BoxCollider> leftBoxCollider = 
        leftEntity.component<BoxCollider>();
    /*ex::ComponentHandle<CircleCollider> leftCircleCollider = 
    leftEntity.component<CircleCollider>();*/
    ex::ComponentHandle<BoxCollider> rightBoxCollider = 
        rightEntity.component<BoxCollider>();
    /*ex::ComponentHandle<CircleCollider> rightCircleCollider = 
    leftEntity.component<CircleCollider>();*/

    // Determine which types of Colliders we are dealing with
    if (false) {
        // TODO
    }

    //Calculate differences between x and y collider origins
    float xDiff = rightBoxCollider.get()->origin.x - 
        leftBoxCollider.get()->origin.x;
    float yDiff = rightBoxCollider.get()->origin.y -
        leftBoxCollider.get()->origin.y;

    //Acquire one-half the length/height "reach" from each collider's origin
    float xlReach = leftBoxCollider.get()->length*0.5f;  // x-axis left reach
    float ylReach = leftBoxCollider.get()->height*0.5f;  // y-axis left reach
    float xrReach = rightBoxCollider.get()->length*0.5f; // x-axis right reach
    float yrReach = rightBoxCollider.get()->height*0.5f; // y-axis right reach
    
    //Define variables for tracking x and y coordinates of impact point.
    bool xcollided = false;
    bool ycollided = false;
    float collisionPointX = 0.0f;
    float collisionPointY = 0.0f;

    /*
     * Check whether each collider's origin plus that collider's reach toward 
     * the other would in turn intersect with the other's reach.
     */
    if (xDiff > 0) { // right collider is further right than left collider

        //step inward toward each other using the "reach" values
        float xOverlap = (rightBoxCollider.get()->origin.x - xrReach) -
            (leftBoxCollider.get()->origin.x + xlReach);

        // If the difference is negative, then 
        if (xOverlap < 0) {
            //They would have collided half way between the amount over-reached
            collisionPointX = 0.5f * abs(xOverlap);
            xcollided = true;
        }
    }
    else if(xDiff < 0) { // left collider is further right than right collider

        //step inward toward each other using the "reach" values
        float xOverlap = (leftBoxCollider.get()->origin.x - xlReach) -
            (rightBoxCollider.get()->origin.x + xrReach);

        // If the difference is negative, then 
        if (xOverlap < 0) {
            //They would have collided half way between the amount over-reached
            collisionPointX = 0.5f * abs(xOverlap);
            xcollided = true;
        }

    }
    else {
        xcollided = true;
        collisionPointX = leftBoxCollider.get()->origin.x;
    }

    if (yDiff > 0) { // right collider is further right than left collider

        //step inward toward each other using the "reach" values
        float yOverlap = (rightBoxCollider.get()->origin.y - yrReach) -
            (leftBoxCollider.get()->origin.y + ylReach);

        // If the difference is negative, then 
        if (yOverlap < 0) {
            //They would have collided half way between the amount over-reached
            collisionPointY = 0.5f * abs(yOverlap);
            ycollided = true;
        }
    }
    else if (yDiff < 0) { // left collider is further right than right collider

        //step inward toward each other using the "reach" values
        float yOverlap = (leftBoxCollider.get()->origin.y - ylReach) -
            (rightBoxCollider.get()->origin.y + yrReach);

        // If the difference is negative, then 
        if (yOverlap < 0) {
            //They would have collided half way between the amount over-reached
            collisionPointY = 0.5f * abs(yOverlap);
            ycollided = true;
        }

    }
    else {
        ycollided = true;
        collisionPointY = leftBoxCollider.get()->origin.y;
    }

    // If a collisionPoint value was evaluated, emit an event
    if (xcollided && ycollided) {
        sf::Vector2f collisionPoint(collisionPointX, collisionPointY);
        events.emit<CollisionEvent>(leftEntity, rightEntity, collisionPoint, 
            events);
    }
}

