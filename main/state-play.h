
#ifndef HEADER_STATE_PLAY
    #define HEADER_STATE_PLAY

    #include "state--common.h"

    #define STATE_PLAY_SWEEP_SPIN_TIME 750

    namespace statePlay {
        void loop(const bool isEnter, const stateCommon::LoopData& data);
    }
    
#endif