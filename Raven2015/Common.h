#pragma once

#define ERRORSTR_AUDIO_TYPE_UNINIT "Error: uninitialized audio type encountered."
#define ERRORSTR_AUDIO_OPERATION_UNINT "Error: uninitialized audio operation encountered."
#define ERRORSTR_AUDIO_NO_RECORD "Error: Attempting to perform non-loading operation on a new audio resource."
#define FPS_30_TICK_TIME 0.0333333333333f
#define FPS_60_TICK_TIME 0.0166666666666f
#define SOUNDMAP_T std::map<std::string, std::shared_ptr<sf::SoundBuffer>>
#define MUSICMAP_T std::map<std::string, std::shared_ptr<sf::Music>>
#define NO_ACTION_STR "NO ACTION"

#include <iostream>
#include "entityx\config.h"
#include "tinyxml2.h"

using std::cout;
using std::cerr;
using std::endl;
using std::cin;

namespace ex = entityx;
namespace Raven {

    namespace Common {

        // An enumeration type detailing the types of processable audio files.
        enum EAudioType { NO_TYPE, SOUND, MUSIC };

        // An enumeration type detailing the possible operations for audio files.
        enum EAudioOperation { NO_AUDIO_OPERATION, AUDIO_LOAD, AUDIO_UNLOAD, AUDIO_PLAY, AUDIO_PAUSE, AUDIO_STOP };

        // An enumeration type detailing the possible operations for timers.
        enum ETimerOperation { NO_TIMER_OPERATION, TIMER_START, TIMER_PAUSE, TIMER_RESTART, TIMER_SCAN, TIMER_REMOVE, TIMER_ADD };

        // An enumeration type detailing how a loop state should be assigned.
        enum ELoop { LOOP_FALSE, LOOP_TRUE, LOOP_UNCHANGED };

        // A specification of the ELoop type for audio resources only
        typedef ELoop EAudioLoop;

        /* 
         * An enumeration type detailing a set of macro render-sorting layers.
         * NO_LAYER     = "null" value
         * Background   = Objects that exist in the back of the game space (typically environmental)
         * Foreground   = Objects that exist in the main game space
         * HUD          = Objects that exist overlaying the game space
         * GUI          = Objects associated with the game's menu system, super-ceding all others
         */
        enum ELayer { NO_LAYER, Background, Foreground, HUD, GUI };

        // A specification of the ELayer type for Rendering assets only
        typedef ELayer ERenderingLayer;

        // A standard pixel-based unit of measurement for the x-axis
        static const float STD_UNITX = 64.0f;

        // A standard pixel-based unit of measurement for the y-axis
        static const float STD_UNITY = 64.0f;

        // The size of borders for imported spritesheets, in pixels
        static const int BORDER_PADDING = 2;

        // The size of borders for sprites within imported spritesheets, in pixels
        static const int SHAPE_PADDING = 2;

        // Helper function for clamping primitives between two values (used in animations)
        template <typename T>
        void clamp(T &toClamp, T min, T max) {
            if (toClamp < min) {
                toClamp = min;
            }
            if (toClamp > max) {
                toClamp = max;
            }
        }

        class Serializable {
        public:
            // Default Null Constructor
            Serializable() {}

            // Primary enforcement of abstract class
            virtual ~Serializable() = 0;

            // Allow the ISerializable to output an XML string representation of its properties
            std::string(*serialize)();

            // Allow the ISerializable to assign new values to its properties via an XML string
            void(*deserialize)(std::string &xml);
        };

        //static void ()

    };

}

namespace cmn = Raven::Common;
namespace rvn = Raven;

