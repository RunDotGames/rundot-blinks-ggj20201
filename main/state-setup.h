
#ifndef HEADER_STATE_SETUP
    #define HEADER_STATE_SETUP

    #include "state--common.h"
    #include "action.h"

    #define STATE_SETUP_ANIM_DURATION 1000
    #define VIEW_STATE_IDLE 0
    #define VIEW_STATE_FADE_OUT 1
    #define VIEW_STATE_FADE_IN 2

    namespace stateSetup {
        void loop(const bool isEnter, const stateCommon::LoopData& data);
    }
    
#endif