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
#include "TimerSystem.h"

namespace Raven {

    void TimerSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {

    }

    void TimerSystem::receive(const TimerEvent &timerEvent) {
        
        std::map<std::string, Timer>::iterator it = timerEvent.timeTable->timerMap.find(timerEvent.timerName);

        if (it != timerEvent.timeTable->timerMap.end()) {
            if (timerEvent.timerOperation == cmn::ETimerOperation::TIMER_ADD) {
                cerr << "Error: Attempt to add timer when timer was found in timerMap." << endl;
            }
        }
        else {
            if (timerEvent.timerOperation != cmn::ETimerOperation::TIMER_ADD) {
                cerr << "Error: Attempt to perform operation on non-existent timer" << endl;
            }
        }
        
        Timer timer = timerEvent.timeTable->timerMap[timerEvent.timerName];

        switch (timerEvent.timerOperation) {
        case cmn::ETimerOperation::NO_TIMER_OPERATION:
            cerr << "Error: Timer event emitted with no operation." << endl;
            break;
        case cmn::ETimerOperation::TIMER_START:
            timer.play();
            break;
        case cmn::ETimerOperation::TIMER_PAUSE:
            timer.pause();
            break;
        case cmn::ETimerOperation::TIMER_SCAN:
            timer.scan(timerEvent.scanTime);
            break;
        case cmn::ETimerOperation::TIMER_RESTART:
            timer.restart();
            break;
        case cmn::ETimerOperation::TIMER_ADD:
            timerEvent.timeTable->timerMap[timerEvent.timerName] = timer;
            break;
        case cmn::ETimerOperation::TIMER_REMOVE:
            timerEvent.timeTable->timerMap.erase(timerEvent.timerName);
            break;
        }
    }
}
