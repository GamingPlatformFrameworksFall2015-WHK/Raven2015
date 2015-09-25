#include "AudioSystem.h"
#include "SFML/Audio/Sound.hpp"

using namespace Raven;

/*
 * Continues processing on any existing audio files as necessary
 */
void AudioSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {

}

void AudioSystem::receive(const AudioEvent &event) {
    receiveEvent(event);
}

/*
 * Responds to requests for operations regarding audio resources.
 */
bool AudioSystem::response(const AudioEvent &event) {
    cout << "entering response" << endl;

    if (event.type == cmn::EAudioType::SOUND) {
        SoundMaker *sMaker = (SoundMaker*)event.maker;

        if (sMaker->soundMap.find(event.audioFileName) == sMaker->soundMap.end() &&
            event.operation != cmn::EAudioOperation::LOAD) {

            cerr << std::string(ERRORSTR_AUDIO_NO_RECORD) << endl;
            return true;
        }

        //Pre-Switch Declarations
        sf::SoundBuffer *buffer;

        // Process the audio operation appropriately
        switch (event.operation) {
        case cmn::EAudioOperation::LOAD:
            cout << "Entering load" << endl;
            buffer = new sf::SoundBuffer();
            if (!buffer->loadFromFile(event.audioFileName)) {
                cerr << "Error: Could not load sound file." << endl;
                return true;
            }
            sMaker->soundMap[event.audioFileName].reset(buffer);
            sMaker->sound.setBuffer(*sMaker->soundMap[event.audioFileName]);
            break;
        case cmn::EAudioOperation::UNLOAD:
            sMaker->soundMap.erase(event.audioFileName);
            break;
        case cmn::EAudioOperation::PLAY:
            cout << "Entering play" << endl;
            sMaker->sound.play();
            break;
        case cmn::EAudioOperation::PAUSE:
            sMaker->sound.pause();
            break;
        case cmn::EAudioOperation::STOP:
            sMaker->sound.stop();
            break;
        }

        cout << "Processing loops" << endl;

        // Evaluate potential loop behavior alterations
        if (event.loop == cmn::EAudioLoop::FALSE) {
            sMaker->sound.setLoop(false);
        }
        else if (event.loop == cmn::EAudioLoop::TRUE) {
            sMaker->sound.setLoop(true);
        }
        else {
            // Do nothing, leaving it "UNCHANGED"
        }

    } // Deal with the "Music" case
    else if (event.type == cmn::EAudioType::MUSIC) {
        MusicMaker *mMaker = (MusicMaker*)event.maker;

        if (mMaker->musicMap.find(event.audioFileName) == mMaker->musicMap.end() &&
            event.operation != cmn::EAudioOperation::LOAD) {

            cerr << std::string(ERRORSTR_AUDIO_NO_RECORD) << endl;
            return true;
        }

        // Process the audio operation appropriately
        switch (event.operation) {
        case cmn::EAudioOperation::LOAD:
            mMaker->musicMap[event.audioFileName]->openFromFile(event.audioFileName);
            break;
        case cmn::EAudioOperation::UNLOAD:
            mMaker->musicMap.erase(event.audioFileName);
            break;
        case cmn::EAudioOperation::PLAY:
            mMaker->musicMap[event.audioFileName]->play();
            break;
        case cmn::EAudioOperation::PAUSE:
            mMaker->musicMap[event.audioFileName]->pause();
            break;
        case cmn::EAudioOperation::STOP:
            mMaker->musicMap[event.audioFileName]->stop();
            break;
        }

        // Evaluate potential loop behavior alterations
        if (event.loop == cmn::EAudioLoop::FALSE) {
            mMaker->musicMap[event.audioFileName]->setLoop(false);
        }
        else if (event.loop == cmn::EAudioLoop::TRUE) {
            mMaker->musicMap[event.audioFileName]->setLoop(true);
        }
        else {
            // Do nothing, leaving it "UNCHANGED"
        }
    }

    cout << "cleanly leaving response" << endl;
    return true;
}
