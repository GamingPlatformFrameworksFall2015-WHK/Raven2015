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
#include "Common.h"

class MovementSystem : public ex::System<MovementSystem> {

public:

    /*
     * Default null constructor
     */
    explicit MovementSystem() {

    }

    /*
     * Updates the movements of entities with Transform & Rigidbody components.
     */
    void update(ex::EntityManager &es, ex::EventManager &events, 
        ex::TimeDelta dt) override; 

};
