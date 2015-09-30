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

    class TimerSystem : public ex::System<TimerSystem>, public ex::Receiver<TimerEvent> {
        
        explicit TimerSystem() {

        }

        void configure(ex::EventManager &event_manager) {
            event_manager.subscribe<TimerEvent>(*this);
        }

        void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

        void receive(const TimerEvent &timerEvent);
    };
}
