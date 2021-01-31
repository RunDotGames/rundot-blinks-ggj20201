#include "state-setup.h"
#include "game-def.h"
#include "state--common.h"
#include "action.h"
#include "timer.h"
#include "state-play.h"
#include "globals.h"
#include "animate.h"

namespace stateSetup {

    bool _isMine;
    byte _viewState;


    void handleNextState(){
        byte targetState = GAME_DEF_STATE_PLAY;
        if(_isMine){
            targetState = GAME_DEF_STATE_MINE;
        }
        stateCommon::handleStateChange(targetState);
    }

    void handleAnimateDone(){
        if(_viewState == VIEW_STATE_FADE_IN) {
            _viewState = VIEW_STATE_FADE_OUT;
            animate::startAnim();
            timer::mark(STATE_SETUP_ANIM_DURATION, handleAnimateDone);
            return;
        }
        
        _viewState = VIEW_STATE_IDLE;
    }

    Color getIndcColor(){
        if(_isMine){
            return RED;
        }

        return GREEN;
    }

    Color getFadeTo(){
        if(_viewState == VIEW_STATE_FADE_OUT){
            return WHITE;
        }

        return getIndcColor();
    }

    Color getFadeFrom(){
        if(_viewState == VIEW_STATE_FADE_OUT){
            return getIndcColor();
        }

        return WHITE;
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            buttonDoubleClicked();
            timer::cancel();
            _isMine = false;
            _viewState = VIEW_STATE_IDLE;
        }  
        bool isClicked = buttonDoubleClicked();
        if(isClicked){
            if(isAlone()){
                _viewState = VIEW_STATE_FADE_IN;
                _isMine = !_isMine;
                animate::startAnim();
                timer::mark(STATE_SETUP_ANIM_DURATION, handleAnimateDone);
                return;
            }

            action::broadcast(GAME_DEF_ACTION_PLAY, millis());
            handleNextState();
            return;
        }

        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_PLAY)){
            handleNextState();
            return;
        }

        #ifdef GLOBALS_DEBUG
        setColor(WHITE);
        if(_isMine){
            setColor(RED)
        }
        #endif

        #ifndef GLOBALS_DEBUG
        if(_viewState == VIEW_STATE_IDLE){
            animate::pulse(WHITE, GLOBALS_SLOW_PULSE);
            return;
        }

        FOREACH_FACE(f){
            animate::animTransitionFace(getFadeFrom(), getFadeTo(), STATE_SETUP_ANIM_DURATION, f);
        }        
        #endif
        
    }


}