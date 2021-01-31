#ifndef HEADER_GLOBALS
    #define HEADER_GLOBALS

    #include "blink-types.h"
    
    #define GLOBALS_SLOW_PULSE 16
    #define GLOBALS_FAST_PULSE 4
    #define GLOBALS_FAST_SPIN 4
    #define GLOBALS_MESSAGE_DELAY 200
    
    #define GLOBALS_DISPLAY_MODE_STANDARD 0
    #define GLOBALS_DISPLAY_MODE_HARD 1
    #define GLOBALS_DISPLAY_MODE_DEBUG 2
    #define GLOBALS_DISPLAY_MODE_COUNT 3

    namespace globals {
        void setDisplayMode(byte displayMode);
        byte getDisplayMode();
    }
    
    
#endif