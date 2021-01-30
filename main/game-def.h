#ifndef HEADER_GAME_DEF
    #define HEADER_GAME_DEF

    #include "state--common.h"

    #define GAME_DEF_STATE_BOARD 0
    #define GAME_DEF_STATE_MINE 1
    #define GAME_DEF_STATE_SWEPT 2
    #define GAME_DEF_STATE_LENGTH 3
    #define GAME_DEF_STATE_DEFAULT GAME_DEF_STATE_BOARD

    /*
    * Avoiding a dependency cycle between this game definition and the more generalized state machine.
    * Keeping the heirarchy GameDefintions -> StateMachine w/out needing added indirection level
    * Explicitly enforce state machine size matches game state count at build time
    */
    #if GAME_DEF_STATE_LENGTH != STATE_COMMON_STATE_LEN
        #error "game state length must equal state machine state length"
    #endif

    
    #define GAME_DEF_VALUE_IDLE 0
    #define GAME_DEF_VALUE_MINE_MIN 1

    #define GAME_DEF_ACTION_SWEEP 1

#endif