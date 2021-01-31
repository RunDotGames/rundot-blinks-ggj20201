#include "main.h"
#include "animate.h"
#include "state--common.h"
#include "timer.h"
#include "game-def.h"
#include "state-setup.h"
#include "state-play.h"
#include "state-mine.h"
#include "state-end.h"

namespace _main_ {
    
    bool _pendingReset = false;

    void setup() {
        stateCommon::addState(stateSetup::loop, GAME_DEF_STATE_SETUP);
        stateCommon::addState(statePlay::loop, GAME_DEF_STATE_PLAY);
        stateCommon::addState(stateMine::loop, GAME_DEF_STATE_MINE);
        stateCommon::addState(stateEnd::loop, GAME_DEF_STATE_END);
    }

    bool isResetHandled(const stateCommon::LoopData& data){
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_RESET)) {
            stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
            return true;
        }
        return false;
    }

    void changeAllToReset(){
        action::broadcast(GAME_DEF_ACTION_RESET,  millis());
        stateCommon::handleStateChange(GAME_DEF_STATE_DEFAULT);
    }

    bool checkWake() {
        bool isWoken = hasWoken();
        bool isLongPressed = buttonLongPressed();
        bool isReleased = buttonReleased();
        if(isWoken){
            _pendingReset = false;
        }
        if(isLongPressed) {
            _pendingReset = true;
        }
        if(_pendingReset) {
            animate::pulse(WHITE, 2);
            if(isReleased){
                changeAllToReset();
                _pendingReset = false;
            }
            return true;
        }
        return false;
    }

    void loop() {
        timer::loop();
        if(checkWake()){
            return;
        }
        bool hadAnyEvent = false;
        FOREACH_FACE(face) {
            if(!isDatagramReadyOnFace(face)) {
                continue;
            }

            if(getDatagramLengthOnFace(face) != ACTION_LEN) {
                markDatagramReadOnFace(face);
                continue;
            }
            
            const byte* buffer = getDatagramOnFace(face);
            const action::Action action = action::decode(buffer);
            markDatagramReadOnFace(face);
            stateCommon::LoopData data{.face=face, .action=action};
            if(!isResetHandled(data)) {
                hadAnyEvent = true;
                stateCommon::loop(data);
            }
        }
        if(hadAnyEvent){
            return;
        }

        stateCommon::LoopData empty{.face=FACE_COUNT, .action=action::Action{.type=ACTION_INVALID, .payload=ACTION_INVALID}};
        stateCommon::loop(empty);
    }

}
