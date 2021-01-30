#include "state-mine.h"
#include "state--common.h"
#include "game-def.h"
#include "action.h"
#include "timer.h"

namespace stateMine {
    
    void readyUp(){
        FOREACH_FACE(f){
            action::send(GAME_DEF_ACTION_MINE_POS, 0, f);
        }
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        setColor(RED);
        if(isEnter){
            timer::mark(200, readyUp);
        }

        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_SWEEP)){
            //TODO HANDLE MINE RESPONDS TO SWEEP
        }
    }

}