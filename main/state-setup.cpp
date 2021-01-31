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
    bool _isIndexInitator;
    byte _index;
    byte _blinkCount;

    
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
            if(_isIndexInitator){
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
            timer::cancel();
            _isIndexInitator = false;
            _index = 0;
            _blinkCount = 0;
            distributedTask::reset();
        }  
        if(buttonDoubleClicked()){
            _isIndexInitator = true;
            distributedTask::init(GAME_DEF_ACTION_INDEX, indexHandler, 0);
        }
        distributedTask::loop(data, GAME_DEF_ACTION_INDEX, indexHandler);
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_PLAY)) {
            handleNextState(data.action.payload);
            return;
        }
        
        if(_working){
            animate::spin(WHITE, GLOBALS_FAST_SPIN);
            return;
        }

        animate::pulse(WHITE, GLOBALS_SLOW_PULSE);
    }


}