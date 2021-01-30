#include "state-setup.h"
#include "game-def.h"
#include "state--common.h"
#include "action.h"
#include "timer.h"
#include "state-play.h"

namespace stateSetup {

    bool _isMine;

    void handleNextState(){
        byte targetState = GAME_DEF_STATE_PLAY;
        if(_isMine){
            targetState = GAME_DEF_STATE_MINE;
        }
        stateCommon::handleStateChange(targetState);
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            _isMine = false;
        }
        bool isClicked = buttonSingleClicked();
        if(isClicked){
            if(isAlone()){
                _isMine = !_isMine;
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

        setColor(WHITE);
        if(_isMine){
            setColor(RED); //DEBUG, flash later
        }
        
    }


}