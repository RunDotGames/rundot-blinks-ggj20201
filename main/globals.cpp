#include "globals.h"

namespace globals {

    byte _displayMode = GLOBALS_DISPLAY_MODE_STANDARD;

    void setDisplayMode(byte displayMode){
        _displayMode = displayMode;
    }

    byte getDisplayMode(){
        return _displayMode;
    }
}