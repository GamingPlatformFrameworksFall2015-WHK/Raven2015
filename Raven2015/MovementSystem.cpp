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


#include "MovementSystem.h"
#include "../ComponentLibrary.h"
#include "../entityx\Entity.h"

using namespace Raven;

/*
 * Iterate through each entity containing a Transform and Rigidbody,
 * calculating where it should move to. Uses C++11 Lambda function syntax.
 */
void MovementSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {

    // Acquire each entity containing a transform and a rigidbody
    es.each<Transform, Rigidbody>(
        [dt](ex::Entity entity, Transform &transform,Rigidbody &rigidbody) {

        // Update its transform based on its rigidbody data.
        transform.transform.x += rigidbody.velocity.x * (float) dt;
        transform.transform.y += rigidbody.velocity.y * (float) dt;
    });
}