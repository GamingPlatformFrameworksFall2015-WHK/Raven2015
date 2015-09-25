#pragma once

#include "Common.h"
#include "entityx\System.h"
#include "EventLibrary.h"

//Assuming there exists an event with this name in EventLibrary.h
struct EventName : public entityx::Event<EventName> { EventName() {} };

class SystemName : public entityx::System<SystemName>, public entityx::Receiver<EventName> {
    
    //Perform initializations
    explicit SystemName() {
        eventToggle = true;
    }

    //Subscribe to events
    void configure(entityx::EventManager &event_manager) {
        event_manager.subscribe<EventName>(*this);
    }

    //Update data and perform logic every "tick"
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;

    // Base receive used to fix a problem where each events.emit results in two receive functions
    template <typename T>
    void receiveEvent(const T &aEvent) {
        eventToggle = eventToggle ? response(aEvent) && false : true;
    }

    //--------For each event you will respond to, you will need these two functions...------------

    /*
     * Picks up the named event
     */
    void receive(const EventName &event) {
        receiveEvent(event);
    }

    /*
     * Responds to the event (your ACTUAL "receive" implementation)
     * Note: it does not matter what boolean this returns.
     */
    bool response(const EventName &event);

    //--------------------------------------------------------------------------------------------

private:
    bool eventToggle;
};
