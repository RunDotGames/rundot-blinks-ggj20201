#include "state-mine.h"
#include "state--common.h"
#include "game-def.h"
#include "action.h"
#include "timer.h"
#include "globals.h"
#include "animate.h"

namespace stateMine {

    byte _targetedFace = FACE_COUNT;
    bool _exploded;
    bool _trapped;
    
    void triggerEnd(bool isVictory){
        FOREACH_FACE(f){
            action::send(GAME_DEF_ACTION_BECOME_END, isVictory, f);
        }
    }

    void readyUp(){
        FOREACH_FACE(f){
            action::send(GAME_DEF_ACTION_MINE_POS, 0, f);
        }
    }

    void swapOutMine(){
        action::send(GAME_DEF_ACTION_BECOME_MINE, millis(), _targetedFace);
        stateCommon::handleStateChange(GAME_DEF_STATE_PLAY);
    }

    void drawStandard(){
        if(_trapped){
            animate::pulse(YELLOW, GLOBALS_FAST_PULSE);
            return;
        }

        if(_exploded){
            animate::pulse(RED, GLOBALS_FAST_PULSE);
            return;
        }

        animate::pulse(BLUE, GLOBALS_SLOW_PULSE);
    }

    void drawDebug(){
        setColor(RED);
    }

    void gatherInform(){
        byte freeFaces[FACE_COUNT];
        byte freeFaceCount = 0;
        FOREACH_FACE(f){
            freeFaces[f] = false;
            if(isValueReceivedOnFaceExpired(f)){
                continue;
            }
            byte value = getLastValueReceivedOnFace(f);
            if(value != GAME_DEF_VAL_SWEPT_FREE){
                continue;
            }

            freeFaces[f] = true;
            freeFaceCount++;
        }
        if(freeFaceCount == 0){
            _trapped = true;
            triggerEnd(true);
            return;
        }

        byte randomFace = random(freeFaceCount-1);
        byte targetFace = 0;
        FOREACH_FACE(f){
            if(!freeFaces[f]){
                continue;
            }

            if(targetFace != randomFace){
                targetFace++;
                continue;
            }
            
            _targetedFace = f;
            break;
        }
        action::broadcast(GAME_DEF_ACTION_RESET_DISTANCES, millis());
        timer::mark(GLOBALS_MESSAGE_DELAY, swapOutMine);
    }

    void requestInform(){
        FOREACH_FACE(f){
            action::send(GAME_DEF_ACTION_REQUEST_INFORM, millis(), f);
        }
        timer::mark(GLOBALS_MESSAGE_DELAY, gatherInform);
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        bool isPressed = buttonDoubleClicked();
        if(isEnter){
            buttonDoubleClicked();
            _trapped = false;
            _exploded = false;
            _targetedFace = FACE_COUNT;
            timer::mark(GLOBALS_MESSAGE_DELAY, readyUp);
        }

        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_SWEEP)){
            timer::mark(GLOBALS_MESSAGE_DELAY, requestInform);
        }

        if(isPressed && !_exploded && !_trapped){
            _exploded = true;
            triggerEnd(false);
        }
        
        switch (globals::getDisplayMode()) {
            case GLOBALS_DISPLAY_MODE_DEBUG:
                drawDebug();
                break;
            default:
                drawStandard();
                break;
        }
    }

}