#include "state-setup.h"
#include "game-def.h"
#include "state--common.h"
#include "action.h"
#include "timer.h"
#include "state-play.h"
#include "globals.h"
#include "animate.h"
#include "distributed-task.h"

namespace stateSetup {

    bool _working;
    bool _isIndexInitiator;
    byte _index;
    byte _blinkCount;

    void handleDisplayChange(byte displayMode){
        globals::setDisplayMode(displayMode);
    }
    
    void handleNextState(byte mineIndex){
        byte targetState = GAME_DEF_STATE_PLAY;
        if(mineIndex == _index) {
            targetState = GAME_DEF_STATE_MINE;
        }
        stateCommon::handleStateChange(targetState);
    }

    void rollMine(){
        byte mineIndex = random(_blinkCount-1);
        action::broadcast(GAME_DEF_ACTION_PLAY, mineIndex);
        handleNextState(mineIndex);
    }

    byte indexHandler(const byte op, const byte payload){
        if(op == DISTRIBUTED_TASK_OP_PASSING_IN) {
            _working = true;
            _index = payload;
            return payload + 1;
        }

        if(op == DISTRIBUTED_TASK_OP_PASSED_DONE) {
            _blinkCount = payload;
            if(_isIndexInitiator){
                timer::mark(GLOBALS_MESSAGE_DELAY, rollMine);
            }
            return payload;
        }

        return payload;
    }

    void loop(const bool isEnter, const stateCommon::LoopData& data){
        if(isEnter) {
            _working = false;
            buttonDoubleClicked();
            buttonMultiClicked();
            timer::cancel();
            _isIndexInitiator = false;
            _index = 0;
            _blinkCount = 0;
            globals::setDisplayMode(GLOBALS_DISPLAY_MODE_STANDARD);
            distributedTask::reset();
        }  
        bool isMultiClicked = buttonMultiClicked();
        bool isDoubleClicked = buttonDoubleClicked();

        if(isDoubleClicked){
            _isIndexInitiator = true;
            distributedTask::init(GAME_DEF_ACTION_INDEX, indexHandler, 0);
        }
        if(isMultiClicked) {
            byte display = (globals::getDisplayMode() + 1) % GLOBALS_DISPLAY_MODE_COUNT;
            handleDisplayChange(display);
            action::broadcast(GAME_DEF_ACTION_CHANGE_DISPLAY, display);
        }
        distributedTask::loop(data, GAME_DEF_ACTION_INDEX, indexHandler);
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_CHANGE_DISPLAY)){
            handleDisplayChange(data.action.payload);
            return;
        }

        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_PLAY)) {
            handleNextState(data.action.payload);
            return;
        }
        
        if(_working){
            animate::spin(WHITE, GLOBALS_FAST_SPIN);
            return;
        }
        
        animate::pulse(WHITE, GLOBALS_SLOW_PULSE);
        FOREACH_FACE(f){
            if(f < globals::getDisplayMode()){
                animate::pulseFace(f, ORANGE, GLOBALS_SLOW_PULSE);
            }
        }
    }


}