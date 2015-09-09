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
#include <map>

#define SOUNDMAP_T std::map<std::pair<ex::Entity, std::string>, sf::SoundBuffer>

class AudioSystem : public ex::System<AudioSystem>,
    ex::Receiver<AudioEvent> {

    /*
     *
     */
    explicit AudioSystem() {

    }

    /*
     *
     */
    void update(ex::EntityManager &es, ex::EventManager &events, 
        ex::TimeDelta dt) override; 

    /*
     *
     */
    void receive(const AudioEvent &event);

    // maps an Entity-soundFileName combo to a SoundBuffer to store the sound.
    SOUNDMAP_T soundMap;
    
};