
#ifndef HEADER_STATE_SETUP
    #define HEADER_STATE_SETUP

    #include "state--common.h"
    #include "action.h"

    namespace stateSetup {
        void loop(const bool isEnter, const stateCommon::LoopData& data);
    }
    
#endif