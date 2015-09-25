#include "AudioSystem.h"
#include "SFML/Audio/Sound.hpp"

using namespace Raven;

/*
 * Continues processing on any existing audio files as necessary
 */
void AudioSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {

<<<<<<< HEAD

}
void AudioSystem::receive(const SoundLoadEvent &event) {
	receiveEvent(event);
}
void AudioSystem::receive(const SoundPlayEvent &event) {
	receiveEvent(event);
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
//THIS ONE GOT HACKED
bool AudioSystem::response(const SoundLoadEvent &event) {
    // If you can't find the file, load it
	cout << "loading soundfile" << endl;
    if (event.maker->soundMap.find(event.audioFileName) == event.maker->soundMap.end()) {
		event.maker->soundMap[event.audioFileName] = std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer());
        event.maker->soundMap[event.audioFileName]->loadFromFile(event.audioFileName);
        event.maker->sound.setBuffer(*event.maker->soundMap[event.audioFileName]);
    }
	return false;
}
void AudioSystem::receive(const SoundUnloadEvent &event) {
    if (event.maker->soundMap.find(event.audioFileName) != event.maker->soundMap.end()) {
        event.maker->soundMap.erase(event.audioFileName);
    }
}
//THIS ONE GOT HACKED
bool AudioSystem::response(const SoundPlayEvent &event) {
	cout << "playing soundfile" << endl;
	event.maker->sound.play();
/*
    if (event.maker->soundMap.find(event.audioFileName) != event.maker->soundMap.end()) {
		cout << "i break here" << endl;
        event.maker->sound.setBuffer(*event.maker->soundMap[event.audioFileName]);
        event.maker->sound.play();
		cout << "I lived" << endl;
    }
	cout << "do i run pass" << endl;*/
	return false;
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
=======
>>>>>>> master
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
