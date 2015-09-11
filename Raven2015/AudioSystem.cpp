#include "AudioSystem.h"
#include "SFML/Audio/Sound.hpp"

using namespace Raven;

/*
 * Continues processing on any existing audio files as necessary
 */
void AudioSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {

    if (a) {
        es.each<SoundMaker>([&](ex::Entity &entity, SoundMaker &soundMaker) {

            events.emit<SoundEvent>(soundMaker.filenames[0]);

            /*
            events.emit<SoundEvent>(&entity, soundMaker.filenames[0], 
                soundMaker.audioType, cmn::EAudioOperation::LOAD, cmn::EAudioLoop::TRUE);
            events.emit<SoundEvent>(&entity, soundMaker.filenames[0], 
                soundMaker.audioType, cmn::EAudioOperation::PLAY, cmn::EAudioLoop::UNCHANGED);
            */
        });
        a = false;
    }
}

/*
 * Responds to requests for operations regarding audio resources.
 */
void AudioSystem::receive(const AudioEvent &event) {
    cout << "TEST1" << endl;
    
    // Accessor for map data structures tracking audio resources
    std::pair<ex::Entity, std::string> key = 
        std::make_pair(*event.owner, event.audioFileName);

    // Deal with the "Sound" case
    if (event.audioType == cmn::EAudioType::SOUND) {

        // Determine whether we already have this sound file on record.
        SOUNDMAP_T::const_iterator it = soundMap.find(
            std::make_pair(*event.owner, event.audioFileName));

        // If it isn't on record and we aren't loading a new file, do nothing.
        if (soundMap.cend() == it &&
            event.audioOperation != cmn::EAudioOperation::LOAD) {

            cerr << std::string(ERRORSTR_AUDIO_NO_RECORD) << endl;
            return;
        }

        //Pre-Switch Declarations
        sf::SoundBuffer buffer;
        std::unique_ptr<sf::Sound> sound;

        // Process the audio operation appropriately
        switch (event.audioOperation) {
        case cmn::EAudioOperation::LOAD:
            buffer.loadFromFile(event.audioFileName);
            soundMap[key] = buffer;
            break;
        case cmn::EAudioOperation::UNLOAD:
            soundMap.erase(key);
            break;
        case cmn::EAudioOperation::PLAY:
            sound.reset(new sf::Sound(soundMap[key]));
            sound->play();
            break;
        case cmn::EAudioOperation::PAUSE:
            sound.reset(new sf::Sound(soundMap[key]));
            sound->pause();
            break;
        case cmn::EAudioOperation::STOP:
            sound.reset(new sf::Sound(soundMap[key]));
            sound->stop();
            break;
        }

        // Evaluate potential loop behavior alterations
        if (event.audioLoop == cmn::EAudioLoop::FALSE) {
            sound->setLoop(false);
        }
        else if (event.audioLoop == cmn::EAudioLoop::TRUE) {
            sound->setLoop(true);
        }
        else {
            // Do nothing, leaving it "UNCHANGED"
        }

    } // Deal with the "Music" case
    else if (event.audioType == cmn::EAudioType::MUSIC) {

        // Determine whether we already have this music file on record.
        MUSICMAP_T::const_iterator it = musicMap.find(
            std::make_pair(*event.owner, event.audioFileName));

        // If it isn't on record and we aren't loading a new file, do nothing.
        if (musicMap.cend() == it &&
            event.audioOperation != cmn::EAudioOperation::LOAD) {

            cerr << std::string(ERRORSTR_AUDIO_NO_RECORD) << endl;
            return;
        }

        //Pre-Switch Declarations
        std::unique_ptr<sf::Music> music;

        // Process the audio operation appropriately
        switch (event.audioOperation) {
        case cmn::EAudioOperation::LOAD:
            music->openFromFile(event.audioFileName);
            break;
        case cmn::EAudioOperation::UNLOAD:
            musicMap.erase(key);
            break;
        case cmn::EAudioOperation::PLAY:
            musicMap[key].play();
            break;
        case cmn::EAudioOperation::PAUSE:
            musicMap[key].pause();
            break;
        case cmn::EAudioOperation::STOP:
            musicMap[key].stop();
            break;
        }

        // Evaluate potential loop behavior alterations
        if (event.audioLoop == cmn::EAudioLoop::FALSE) {
            music->setLoop(false);
        }
        else if (event.audioLoop == cmn::EAudioLoop::TRUE) {
            music->setLoop(true);
        }
        else {
            // Do nothing, leaving it "UNCHANGED"
        }
    }
}

/*
 * Responds to requests for operations regarding audio resources.
 */
void AudioSystem::receive(const SoundEvent &event) {
    cout << "TEST2" << endl;
    
    // Accessor for map data structures tracking audio resources
    std::pair<ex::Entity, std::string> key = 
        std::make_pair(*event.owner, event.audioFileName);

    // Deal with the "Sound" case
    if (event.audioType == cmn::EAudioType::SOUND) {

        // Determine whether we already have this sound file on record.
        SOUNDMAP_T::const_iterator it = soundMap.find(
            std::make_pair(*event.owner, event.audioFileName));

        // If it isn't on record and we aren't loading a new file, do nothing.
        if (soundMap.cend() == it &&
            event.audioOperation != cmn::EAudioOperation::LOAD) {

            cerr << std::string(ERRORSTR_AUDIO_NO_RECORD) << endl;
            return;
        }

        //Pre-Switch Declarations
        sf::SoundBuffer buffer;
        std::unique_ptr<sf::Sound> sound;

        // Process the audio operation appropriately
        switch (event.audioOperation) {
        case cmn::EAudioOperation::LOAD:
            buffer.loadFromFile(event.soundMaker.soundMap);
            soundMap[key] = buffer;
            break;
        case cmn::EAudioOperation::UNLOAD:
            soundMap.erase(key);
            break;
        case cmn::EAudioOperation::PLAY:
            sound.reset(new sf::Sound(soundMap[key]));
            sound->play();
            break;
        case cmn::EAudioOperation::PAUSE:
            sound.reset(new sf::Sound(soundMap[key]));
            sound->pause();
            break;
        case cmn::EAudioOperation::STOP:
            sound.reset(new sf::Sound(soundMap[key]));
            sound->stop();
            break;
        }

        // Evaluate potential loop behavior alterations
        if (event.audioLoop == cmn::EAudioLoop::FALSE) {
            sound->setLoop(false);
        }
        else if (event.audioLoop == cmn::EAudioLoop::TRUE) {
            sound->setLoop(true);
        }
        else {
            // Do nothing, leaving it "UNCHANGED"
        }

    } // Deal with the "Music" case
    else if (event.audioType == cmn::EAudioType::MUSIC) {

        // Determine whether we already have this music file on record.
        MUSICMAP_T::const_iterator it = musicMap.find(
            std::make_pair(*event.owner, event.audioFileName));

        // If it isn't on record and we aren't loading a new file, do nothing.
        if (musicMap.cend() == it &&
            event.audioOperation != cmn::EAudioOperation::LOAD) {

            cerr << std::string(ERRORSTR_AUDIO_NO_RECORD) << endl;
            return;
        }

        //Pre-Switch Declarations
        std::unique_ptr<sf::Music> music;

        // Process the audio operation appropriately
        switch (event.audioOperation) {
        case cmn::EAudioOperation::LOAD:
            music->openFromFile(event.audioFileName);
            break;
        case cmn::EAudioOperation::UNLOAD:
            musicMap.erase(key);
            break;
        case cmn::EAudioOperation::PLAY:
            musicMap[key].play();
            break;
        case cmn::EAudioOperation::PAUSE:
            musicMap[key].pause();
            break;
        case cmn::EAudioOperation::STOP:
            musicMap[key].stop();
            break;
        }

        // Evaluate potential loop behavior alterations
        if (event.audioLoop == cmn::EAudioLoop::FALSE) {
            music->setLoop(false);
        }
        else if (event.audioLoop == cmn::EAudioLoop::TRUE) {
            music->setLoop(true);
        }
        else {
            // Do nothing, leaving it "UNCHANGED"
        }
    }
}
