#pragma once

#define ERRORSTR_AUDIO_TYPE_UNINIT "Error: uninitialized audio type " + \
    std::string("encountered.")
#define ERRORSTR_AUDIO_OPERATION_UNINT "Error: uninitialized audio " + \
    std::string("operation encountered.")
#define ERRORSTR_AUDIO_NO_RECORD "Error: Attempting to perform non-loading " + \
    std::string("operation on a new audio resource.")

#include <iostream>
#include "entityx\config.h"

using std::cout;
using std::cerr;
using std::endl;

namespace ex = entityx;
namespace Raven {

    namespace Common {

        // An enumeration type detailing the types of processable audio files.
        enum EAudioType { NO_TYPE, SOUND, MUSIC };

        // An enumeration type detailing the possible operations for audio files.
        enum EAudioOperation { NO_OPERATION, LOAD, UNLOAD, PLAY, PAUSE, STOP };

        // An enumeration type detailing whether an audio resource should loop.
        enum EAudioLoop { FALSE, TRUE, UNCHANGED };

        // A standard pixel-based unit of measurement for the x-axis
        static const float STD_UNITX = 64.0f;

        // A standard pixel-based unit of measurement for the y-axis
        static const float STD_UNITY = 64.0f;

    };

}

namespace cmn = Raven::Common;
namespace rvn = Raven;

