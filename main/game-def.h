#ifndef HEADER_GAME_DEF
    #define HEADER_GAME_DEF

    #include "state--common.h"

    #define GAME_DEF_STATE_SETUP 0
    #define GAME_DEF_STATE_PLAY 1
    #define GAME_DEF_STATE_MINE 2
    #define GAME_DEF_STATE_END 3
    #define GAME_DEF_STATE_LENGTH 4
    #define GAME_DEF_STATE_DEFAULT GAME_DEF_STATE_SETUP

    /*
    * Avoiding a dependency cycle between this game definition and the more generalized state machine.
    * Keeping the heirarchy GameDefintions -> StateMachine w/out needing added indirection level
    * Explicitly enforce state machine size matches game state count at build time
    */
    #if GAME_DEF_STATE_LENGTH != STATE_COMMON_STATE_LEN
        #error "game state length must equal state machine state length"
    #endif
    
    #define GAME_DEF_DIST_INVALID 128

    #define GAME_DEF_ACTION_PLAY 1
    #define GAME_DEF_ACTION_MINE_POS 2
    #define GAME_DEF_ACTION_SWEEP 3
    #define GAME_DEF_ACTION_REQUEST_INFORM 4
    #define GAME_DEF_ACTION_RESET_DISTANCES 5
    #define GAME_DEF_ACTION_BECOME_MINE 6
    #define GAME_DEF_ACTION_RESET 7
    #define GAME_DEF_ACTION_BECOME_END 8
    #define GAME_DEF_ACTION_INDEX 9 //10, 11
    #define GAME_DEF_ACTION_CHANGE_DISPLAY 12

    #define GAME_DEF_VAL_SWEPT_TAKEN 2
    #define GAME_DEF_VAL_SWEPT_FREE 1
    

#endif