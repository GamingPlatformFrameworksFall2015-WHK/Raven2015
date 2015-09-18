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
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/Music.hpp"
#include <map>
#include <vector>
#include "ComponentLibrary.h"
#include "EventLibrary.h"

#define SOUNDMAP_T std::map<std::pair<ex::Entity, std::string>, sf::SoundBuffer>
#define MUSICMAP_T std::map<std::pair<ex::Entity, std::string>, sf::Music>

namespace Raven {

    /*
     * An AudioSystem that receives AudioEvents and updates game systems with
     * audio-related information.
     *
     * -------------
     * Instructions:
     * -------------
     * When users wish to use an audio resource, a LOAD operation must be called
     * from an AudioEvent. Depending on the size of the audio file, you may wish to
     * use a SoundEvent (for under a minute) or MusicEvent (for longer files).
     *
     * Typical operations include PLAY, PAUSE, & STOP. When finished, use the
     * UNLOAD operation to free the space for other resources.
     *
     * Because audio resources are mapped using Entity-filename pairs, users
     * should designate an Entity "owner" for each named audio resource. Attempting
     * to load the same audio file to the same Entity will result in the buffer or
     * stream becoming replaced with another copy of itself, interrupting the
     * previous audio resource regardless of what state it was in.
     *
     */
    class AudioSystem : public ex::System<AudioSystem>,
        public ex::Receiver<AudioSystem> {

    public:
        /*
         *
         */
        explicit AudioSystem() {
            eventToggle = true;
        }

        /*
         * Setup necessary static information
         */
        void configure(entityx::EventManager &event_manager) {

            // Music Events
            event_manager.subscribe<MusicLoadEvent>(*this);
            event_manager.subscribe<MusicUnloadEvent>(*this);
            event_manager.subscribe<MusicPlayEvent>(*this);
            event_manager.subscribe<MusicPauseEvent>(*this);
            event_manager.subscribe<MusicStopEvent>(*this);
            event_manager.subscribe<MusicCustomEvent>(*this);

            // Sound events
            event_manager.subscribe<SoundLoadEvent>(*this);
            event_manager.subscribe<SoundUnloadEvent>(*this);
            event_manager.subscribe<SoundPlayEvent>(*this);
            event_manager.subscribe<SoundPauseEvent>(*this);
            event_manager.subscribe<SoundStopEvent>(*this);
            event_manager.subscribe<SoundCustomEvent>(*this);

            event_manager.subscribe<CollisionEvent>(*this);
        }

        /*
         * Continues processing on any existing audio files as necessary
         */
        void update(ex::EntityManager &es, ex::EventManager &events,
            ex::TimeDelta dt) override;

        // Base receive
        template <typename T>
        void receiveEvent(const T &aEvent) {
            eventToggle = eventToggle ? response(aEvent) && false : true;
        }

        // AudioEvent receptions...
        void receive(const MusicLoadEvent &event);   // Loads music file
        void receive(const MusicUnloadEvent &event); // Unloads music file
        void receive(const MusicPlayEvent &event);   // Plays music file
        void receive(const MusicPauseEvent &event);  // Pauses music file
        void receive(const MusicStopEvent &event);   // Pauses and Scans back
        void receive(const MusicCustomEvent &event); // User defined
        void receive(const SoundLoadEvent &event);   // Loads sound file
        void receive(const SoundUnloadEvent &event); // Unloads sound file
        void receive(const SoundPlayEvent &event);   // Plays sound file
        void receive(const SoundPauseEvent &event);  // Pauses sound file
        void receive(const SoundStopEvent &event);   // Pauses and Scan back
        void receive(const SoundCustomEvent &event); // User defined

        // Collision reception
        void receive(const CollisionEvent &event);
        bool response(const CollisionEvent &event);

        bool eventToggle;
    };

}
