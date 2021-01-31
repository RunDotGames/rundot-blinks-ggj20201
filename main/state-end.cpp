#include "state-end.h"
#include "state--common.h"
#include "game-def.h"
#include "action.h"
#include "timer.h"
#include "globals.h"
#include "animate.h"
#include "state-play.h"

namespace stateEnd {


    bool _victory;

    void setVictory(bool victory){
        _victory = victory;
    }

    void passAlong(){
        FOREACH_FACE(f){
            action::send(GAME_DEF_ACTION_BECOME_END, _victory, f);
        }
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter){
            timer::mark(STATE_END_PASS_SPEED, passAlong);
        }

        if(_victory){
            animate::spin(GREEN, GLOBALS_FAST_SPIN);
            return;
        }

        if(statePlay::isLastSwept()){
            statePlay::draw(ORANGE);
            return;
        }

        animate::spin(ORANGE, GLOBALS_FAST_SPIN);
    }

}