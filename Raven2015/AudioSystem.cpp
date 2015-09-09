#include "AudioSystem.h"

/*
 *
 */
void AudioSystem::receive(const AudioEvent &event) {
    
    if (event.audioType == cmn::EAudioType::SOUND) {

        // Determine whether we already have this sound file on record.
        SOUNDMAP_T::const_iterator it = soundMap.find(
            std::make_pair(event.owner, event.audioFileName));

        // If it isn't on record and we aren't loading a new file, do nothing.
        if (soundMap.cend() == it &&
            event.audioOperation != cmn::EAudioOperation::LOAD) {

            cerr << ERRORSTR_AUDIO_NO_RECORD << endl;
            return;
        }

        //Pre-Switch Declarations
        sf::SoundBuffer buffer;

        // Process the audio operation appropriately
        switch (event.audioOperation) {
        case cmn::EAudioOperation::LOAD:
            buffer.loadFromFile(event.audioFileName);
            soundMap[make_pair(event.owner, event.audioFileName)] = buffer;
            break;
        case cmn::EAudioOperation::UNLOAD:

            break;
        case cmn::EAudioOperation::PLAY:

            break;
        case cmn::EAudioOperation::PAUSE:

            break;
        case cmn::EAudioOperation::STOP:

            break;
        case cmn::EAudioOperation::RESTART:

            break;
        }
    }
    else if (event.audioType == cmn::EAudioType::MUSIC) {

    }
}
