#include "AudioSystem.h"
#include "SFML/Audio/Sound.hpp"

using namespace Raven;

/*
 * Continues processing on any existing audio files as necessary
 */
void AudioSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {

}

/*
 * Responds to requests for operations regarding audio resources.
 */
void AudioSystem::receive(const AudioEvent &event) {

    // Deal with the "Sound" case
    if (event.type == cmn::EAudioType::SOUND) {
        SoundMaker *sMaker = (SoundMaker*)event.maker;

        if (sMaker->soundMap.find(event.audioFileName) == sMaker->soundMap.end() &&
            event.operation != cmn::EAudioOperation::AUDIO_LOAD) {

            cerr << std::string(ERRORSTR_AUDIO_NO_RECORD) << endl;
            return;
        }

        //Pre-Switch Declarations
        sf::SoundBuffer *buffer;

        // Process the audio operation appropriately
        switch (event.operation) {
        case cmn::EAudioOperation::AUDIO_LOAD:
            buffer = new sf::SoundBuffer();
            if (!buffer->loadFromFile(event.audioFileName)) {
                cerr << "Error: Could not load sound file." << endl;
                return;
            }
            sMaker->soundMap[event.audioFileName].reset(buffer);
            sMaker->sound.setBuffer(*sMaker->soundMap[event.audioFileName]);
            break;
        case cmn::EAudioOperation::AUDIO_UNLOAD:
            sMaker->soundMap.erase(event.audioFileName);
            break;
        case cmn::EAudioOperation::AUDIO_PLAY:
            sMaker->sound.play();
            break;
        case cmn::EAudioOperation::AUDIO_PAUSE:
            sMaker->sound.pause();
            break;
        case cmn::EAudioOperation::AUDIO_STOP:
            sMaker->sound.stop();
            break;
        }

        // Evaluate potential loop behavior alterations
        if (event.loop == cmn::EAudioLoop::LOOP_FALSE) {
            sMaker->sound.setLoop(false);
        }
        else if (event.loop == cmn::EAudioLoop::LOOP_TRUE) {
            sMaker->sound.setLoop(true);
        }
        else {
            // Do nothing, leaving it "UNCHANGED"
        }

    } // Deal with the "Music" case
    else if (event.type == cmn::EAudioType::MUSIC) {
        MusicMaker *mMaker = (MusicMaker*)event.maker;

        if (mMaker->musicMap.find(event.audioFileName) == mMaker->musicMap.end() &&
            event.operation != cmn::EAudioOperation::AUDIO_LOAD) {

            cerr << std::string(ERRORSTR_AUDIO_NO_RECORD) << endl;
            return;
        }

        // Process the audio operation appropriately
        switch (event.operation) {
        case cmn::EAudioOperation::AUDIO_LOAD:
            mMaker->musicMap[event.audioFileName]->openFromFile(event.audioFileName);
            break;
        case cmn::EAudioOperation::AUDIO_UNLOAD:
            mMaker->musicMap.erase(event.audioFileName);
            break;
        case cmn::EAudioOperation::AUDIO_PLAY:
            mMaker->musicMap[event.audioFileName]->play();
            break;
        case cmn::EAudioOperation::AUDIO_PAUSE:
            mMaker->musicMap[event.audioFileName]->pause();
            break;
        case cmn::EAudioOperation::AUDIO_STOP:
            mMaker->musicMap[event.audioFileName]->stop();
            break;
        }

        // Evaluate potential loop behavior alterations
        if (event.loop == cmn::EAudioLoop::LOOP_FALSE) {
            mMaker->musicMap[event.audioFileName]->setLoop(false);
        }
        else if (event.loop == cmn::EAudioLoop::LOOP_TRUE) {
            mMaker->musicMap[event.audioFileName]->setLoop(true);
        }
        else {
            // Do nothing, leaving it "UNCHANGED"
        }
    }
}
