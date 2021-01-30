
#ifndef HEADER_STATE_BOARD
    #define HEADER_STATE_BOARD

    #include "state--common.h"

    #define STATE_BOARD_INVALID_DISTANCE (0xff - 1)

    namespace stateBoard {
        void loop(const bool isEnter, const stateCommon::LoopData& data);
        byte getMineFace();
        void updateMineDistance();
    }
    
#endif