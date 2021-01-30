#include "state-mine.h"
#include "game-def.h"
#include "action.h"

namespace stateMine {
    
    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter){
            setValueSentOnAllFaces(GAME_DEF_VALUE_MINE_MIN);
        }
        bool isClicked = buttonSingleClicked();
        if(isAlone()){
            if(isClicked) {
                stateCommon::handleStateChange(GAME_DEF_STATE_BOARD);
                return;
            }

            setColor(RED);
            return;
        }

        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_SWEEP)){
            //TODO Handle Mine Encouters Sweep
        }
        setColor(WHITE);
        
    }

}