#include "state-swept.h"
#include "state--common.h"
#include "game-def.h"
#include "action.h"
#include "state-board.h"

namespace stateSwept {
    
    bool _lastSwept;

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        stateBoard::updateMineDistance();
        if(isEnter){
            setValueSentOnAllFaces(GAME_DEF_VALUE_IDLE);
            _lastSwept = true;
        }
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_SWEEP)){
            _lastSwept = false;
        }

        setColor(GREEN);
        
        if(!_lastSwept){
            return;
        }

        byte mineIndex = stateBoard::getMineFace();
        if(mineIndex == FACE_COUNT){
            return;
        }

        byte next = (FACE_COUNT + mineIndex + 1) % FACE_COUNT;
        byte last = (FACE_COUNT + mineIndex - 1) % FACE_COUNT;

        FOREACH_FACE(f){
            byte delta = f < mineIndex ? mineIndex - f : f - mineIndex;
            
            if(f == mineIndex || f == next || f == last) {
                setColorOnFace(YELLOW, f);
            }
        }
    }

}