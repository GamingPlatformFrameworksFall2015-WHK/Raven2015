#include "AudioSystem.h"
#include "SFML/Audio/Sound.hpp"

using namespace Raven;

/*
 * Continues processing on any existing audio files as necessary
 */
void AudioSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {


}

void AudioSystem::receive(const MusicLoadEvent &event) {
    // If you can't find the file, load it
    if (event.maker->musicMap.find(event.audioFileName) == event.maker->musicMap.end()) {
        event.maker->musicMap[event.audioFileName]->openFromFile(event.audioFileName);
    }
}
void AudioSystem::receive(const MusicUnloadEvent &event) {
    if (event.maker->musicMap.find(event.audioFileName) != event.maker->musicMap.end()) {
        event.maker->musicMap.erase(event.audioFileName);
    }
}
void AudioSystem::receive(const MusicPlayEvent &event) {
    if (event.maker->musicMap.find(event.audioFileName) != event.maker->musicMap.end()) {
        event.maker->musicMap[event.audioFileName]->play();
    }
}
void AudioSystem::receive(const MusicPauseEvent &event) {
    if (event.maker->musicMap.find(event.audioFileName) != event.maker->musicMap.end()) {
        event.maker->musicMap[event.audioFileName]->pause();
    }
}
void AudioSystem::receive(const MusicStopEvent &event) {
    if (event.maker->musicMap.find(event.audioFileName) != event.maker->musicMap.end()) {
        event.maker->musicMap[event.audioFileName]->stop();
    }
}
void AudioSystem::receive(const MusicCustomEvent &event) {
    event.func(event.audioFileName, event.maker.get());
}
void AudioSystem::receive(const SoundLoadEvent &event) {
    // If you can't find the file, load it
    if (event.maker->soundMap.find(event.audioFileName) == event.maker->soundMap.end()) {
        event.maker->soundMap.insert(std::make_pair(event.audioFileName, std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer())));
        event.maker->soundMap[event.audioFileName]->loadFromFile(event.audioFileName);
        event.maker->sound.setBuffer(*event.maker->soundMap[event.audioFileName]);
    }
}
void AudioSystem::receive(const SoundUnloadEvent &event) {
    if (event.maker->soundMap.find(event.audioFileName) != event.maker->soundMap.end()) {
        event.maker->soundMap.erase(event.audioFileName);
    }
}
void AudioSystem::receive(const SoundPlayEvent &event) {
    if (event.maker->soundMap.find(event.audioFileName) != event.maker->soundMap.end()) {
        event.maker->sound.setBuffer(*event.maker->soundMap[event.audioFileName]);
        event.maker->sound.play();
    }
}
void AudioSystem::receive(const SoundPauseEvent &event) {
    if (event.maker->soundMap.find(event.audioFileName) != event.maker->soundMap.end()) {
        event.maker->sound.setBuffer(*event.maker->soundMap[event.audioFileName]);
        event.maker->sound.pause();
    }
}
void AudioSystem::receive(const SoundStopEvent &event) {
    if (event.maker->soundMap.find(event.audioFileName) != event.maker->soundMap.end()) {
        event.maker->sound.setBuffer(*event.maker->soundMap[event.audioFileName]);
        event.maker->sound.stop();
    }
}
void AudioSystem::receive(const SoundCustomEvent &event) {
    event.func(event.audioFileName, event.maker.get());
}

void AudioSystem::receive(const CollisionEvent &event) {
    receiveEvent(event);
}

bool AudioSystem::response(const CollisionEvent &event) {

    return false;
}

