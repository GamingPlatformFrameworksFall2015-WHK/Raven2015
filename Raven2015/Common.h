#pragma once

#define ERRORSTR_AUDIO_TYPE_UNINIT "Error: uninitialized audio type " + \
                                   "encountered."
#define ERRORSTR_AUDIO_OPERATION_UNINT "Error: uninitialized audio " + \
                                   "operation encountered."
#define ERRORSTR_AUDIO_NO_RECORD "Error: Attempting to non-load a new sound."

using std::cout;
using std::cerr;
using std::endl;

namespace ex = entityx;
//namespace cmn = Common...at bottom of page

namespace Common {

    // An enumeration type detailing the types of processable audio files.
    enum EAudioType {NO_TYPE, SOUND, MUSIC};

    // An enumeration type detailing the possible operations for audio files.
    enum EAudioOperation {NO_OPERATION, LOAD, UNLOAD, PLAY, PAUSE, STOP, RESTART};

    // A standard pixel-based unit of measurement for the x-axis
    static const float STD_UNITX = 64.0f;

    // A standard pixel-based unit of measurement for the y-axis
    static const float STD_UNITY = 64.0f;
    
};

namespace cmn = Common;
