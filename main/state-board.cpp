#include "state-board.h"
#include "game-def.h"
#include "state--common.h"
#include "action.h"

namespace stateBoard {

    byte _mineDistance = STATE_BOARD_INVALID_DISTANCE;
    byte _mineFace = FACE_COUNT;
    
    void loop(const bool isEnter, const stateCommon::LoopData& data){
        updateMineDistance();
        setColor(WHITE);
        if(isEnter){
            setValueSentOnAllFaces(GAME_DEF_VALUE_IDLE);
        }
        bool isClicked = buttonSingleClicked();
        if(isAlone() && isClicked){
            stateCommon::handleStateChange(GAME_DEF_STATE_MINE);
            return;
        }

        if(isClicked){
            action::broadcast(GAME_DEF_ACTION_SWEEP, millis());
            stateCommon::handleStateChange(GAME_DEF_STATE_SWEPT);
            return;
        }
    }

    byte getMineFace(){
        return _mineFace;
    }

    void updateMineDistance(){
        byte mineIndex = FACE_COUNT;
        byte currentMineDistance = STATE_BOARD_INVALID_DISTANCE;
        byte currentMineFace = FACE_COUNT;
        FOREACH_FACE(f){
            if(isValueReceivedOnFaceExpired(f)){
                continue;
            }

            byte faceVal = getLastValueReceivedOnFace(f);
            if(faceVal < GAME_DEF_VALUE_MINE_MIN){
                continue;
            }

            byte distance = faceVal - GAME_DEF_VALUE_MINE_MIN;
            if(distance < currentMineDistance){
                currentMineDistance = distance;
                currentMineFace = f;
            }
        }

        if(currentMineDistance == _mineDistance){
            return;
        }

        _mineDistance = currentMineDistance;
        _mineFace = currentMineFace;
        if(_mineDistance == STATE_BOARD_INVALID_DISTANCE){
            setValueSentOnAllFaces(GAME_DEF_VALUE_IDLE);
            return;
        }

        setValueSentOnAllFaces(_mineDistance + GAME_DEF_VALUE_MINE_MIN + 1);
    }
}