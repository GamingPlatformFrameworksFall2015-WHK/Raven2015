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

using namespace Raven;

/*
 * Iterate through each entity containing a Transform and Rigidbody,
 * calculating where it should move to. Uses C++11 Lambda function syntax.
 */
void MovementSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {

    // Acquire each entity containing a tracker
    es.each<Tracker>([&](ex::Entity trackerEntity, Tracker &tracker) {
        // Distance between two points
        float xDis = 0.0, yDis = 0.0;
        // Holds closest pawn's transform
        float pawnX = 0.0, pawnY = 0.0;
        // Used for calculating closest pawn to tracker
        float tempDis = 0.0, minDis =  FLT_MAX;
        
        // Find closest pawn to tracker
        es.each<Pawn>([&](ex::Entity pawnEntity, Pawn &pawn) {
            xDis = pawnEntity.component<Transform>()->transform.x -
                trackerEntity.component<Transform>()->transform.x;
            yDis = pawnEntity.component<Transform>()->transform.y -
                trackerEntity.component<Transform>()->transform.y;

            tempDis = sqrtf(pow(xDis, 2) + pow(yDis, 2));

            
            if (tempDis < minDis) {
                minDis = tempDis;
                pawnX = pawnEntity.component<Transform>()->transform.x;
                pawnY = pawnEntity.component<Transform>()->transform.y;
            }
        });

        // Transform tracker towards pawn
        // If xDis is negative, pawn is to left of tracker
        if (xDis < 0.0) {
            trackerEntity.component<Transform>()->transform.x -= (FPS_100_TICK_TIME * 50);
        }
        else {
            trackerEntity.component<Transform>()->transform.x += (FPS_100_TICK_TIME * 50);
        }
        // If yDis is negative, pawn is below tracker
        if (yDis < 0.0) {
            trackerEntity.component<Transform>()->transform.y -= (FPS_100_TICK_TIME * 50);
        }
        else {
            trackerEntity.component<Transform>()->transform.y += (FPS_100_TICK_TIME * 50);
        }
        
    });

    // Acquire each entity containing a pacer
    es.each<Pacer>([&](ex::Entity entity, Pacer &pacer) {
        // If pacer has reached limit of its travel radius,
        // reverse its velocity so it will travel in opposite direction
        auto horAction = [&]() {
            if (abs(entity.component<Transform>()->transform.x) >
                abs(pacer.origin.x) + abs(pacer.radius)) {
                pacer.velocity = -pacer.velocity;
            }
        };
        auto vertAction = [&]() {
            if (abs(entity.component<Transform>()->transform.y) > 
                abs(pacer.origin.y) + abs(pacer.radius)) {
                pacer.velocity = -pacer.velocity;
            }
        };

        switch (pacer.direction) {
        case Pacer::Direction::HORIZONTAL: horAction(); break;
        case Pacer::Direction::VERTICAL: vertAction(); break;
        case Pacer::Direction::DIAGONAL: horAction(); vertAction(); break;
        }

        entity.component<Rigidbody>()->velocity = pacer.velocity;
    });

    // Acquire each entity containing a transform and a rigidbody
    es.each<Transform, Rigidbody>(
        [dt](ex::Entity entity, Transform &transform,Rigidbody &rigidbody) {

        // Update its transform based on its rigidbody data.
        transform.transform.x += rigidbody.velocity.x;
        transform.transform.y += rigidbody.velocity.y;
    });
}