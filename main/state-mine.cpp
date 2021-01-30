#include "state-mine.h"
#include "state--common.h"
#include "game-def.h"
#include "action.h"
#include "timer.h"

namespace stateMine {

    byte _targetedFace = FACE_COUNT;
    
    void readyUp(){
        FOREACH_FACE(f){
            action::send(GAME_DEF_ACTION_MINE_POS, 0, f);
        }
    }

    void swapOutMine(){
        action::send(GAME_DEF_ACTION_BECOME_MINE, millis(), _targetedFace);
        stateCommon::handleStateChange(GAME_DEF_STATE_PLAY);
    }

    void gatherInform(){
        FOREACH_FACE(f){
            if(isValueReceivedOnFaceExpired(f)){
                continue;
            }
            byte value = getLastValueReceivedOnFace(f);
            if(value == GAME_DEF_VAL_SWEPT_FREE){
                //TODO: choose random not first
                _targetedFace = f;
                action::broadcast(GAME_DEF_ACTION_RESET_DISTANCES, millis());
                timer::mark(200, swapOutMine);
            }
        }
    }

    void requestInform(){
        action::broadcast(GAME_DEF_ACTION_REQUEST_INFORM, millis());
        timer::mark(200, gatherInform);
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        setColor(RED);
        if(isEnter){
            _targetedFace = FACE_COUNT;
            timer::mark(200, readyUp);
        }

        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_SWEEP)){
            timer::mark(200, requestInform);
        }
    }

}