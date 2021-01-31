
#ifndef HEADER_STATE_END
    #define HEADER_STATE_END

    #include "state--common.h"

    #define STATE_END_PASS_SPEED 300

    namespace stateEnd {
        void loop(const bool isEnter, const stateCommon::LoopData& data);
        void setVictory(bool isVictory);
    }
    
#endif