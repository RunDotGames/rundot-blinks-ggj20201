#include "state-play.h"
#include "game-def.h"
#include "action.h"
#include "state-setup.h"
#include "timer.h"
#include "globals.h"
#include "animate.h"
#include "state-end.h"

namespace statePlay {

    byte _distances[FACE_COUNT];

    byte _myDistance = GAME_DEF_DIST_INVALID;

    bool _isSwept;
    bool _isLastSweep;
    bool _isSpinning;

    bool isLastSwept(){
        return _isLastSweep;
    }

    void draw(Color backing){
        #ifdef GLOBALS_DEBUG
        setColor(BLUE);
        if(_isSwept){
            setColor(dim(GREEN, 64));
        }
        if(_isLastSweep){
            setColor(GREEN);
        }
        
        FOREACH_FACE(f){
            if(_myDistance  == _distances[f] + 1){
                setColorOnFace(YELLOW, f);
            }
        }
        #endif

        #ifndef GLOBALS_DEBUG
        if(_isSpinning){
            animate::spin(BLUE, GLOBALS_FAST_SPIN);
            return;
        }

        if(!_isSwept){
            animate::pulse(BLUE, GLOBALS_SLOW_PULSE);
            return;
        }

        animate::pulse(backing, GLOBALS_SLOW_PULSE);
        if(!_isLastSweep){
            return;
        }

        FOREACH_FACE(f){
            if(_myDistance  == _distances[f] + 1){
                setColorOnFace(YELLOW, f);
            }
        }
        #endif
    }

    void resetDistances(){
        FOREACH_FACE(f){
            _distances[f] = GAME_DEF_DIST_INVALID;
        }
        _myDistance = GAME_DEF_DIST_INVALID;
    }

    void handleSpinDone(){
        _isSpinning = false;
    }

    void updateSweep(bool pressed, const stateCommon::LoopData& data){
        if(pressed && !_isSwept){
            _isSwept = true;
            _isLastSweep = true;
            _isSpinning = true;
            timer::mark(STATE_PLAY_SWEEP_SPIN_TIME, handleSpinDone);
            action::broadcast(GAME_DEF_ACTION_SWEEP, millis());
            return;
        }

        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_SWEEP)){
            _isLastSweep = false;
        }
    }

    void updateDistances(const stateCommon::LoopData& data){
        if(action::isBroadcastReceived(data.action, GAME_DEF_ACTION_RESET_DISTANCES)){
            resetDistances();
        }
        if(data.action.type != GAME_DEF_ACTION_MINE_POS){
            return;
        }

        _distances[data.face] = data.action.payload;
        byte oldDistance = _myDistance;
        _myDistance = GAME_DEF_DIST_INVALID;
        FOREACH_FACE(f){
            if(_distances[f] < _myDistance){
                _myDistance = _distances[f] + 1;
            }
        }
        if(_myDistance == oldDistance){
            return;
        }

        FOREACH_FACE(f){
            action::send(GAME_DEF_ACTION_MINE_POS, _myDistance, f);
        }
    }

    void updateInform(const stateCommon::LoopData& data){
        if(data.action.type != GAME_DEF_ACTION_REQUEST_INFORM){
            return;
        }

        if(_isSwept){
            setValueSentOnAllFaces(GAME_DEF_VAL_SWEPT_TAKEN);
            return;
        }

        setValueSentOnAllFaces(GAME_DEF_VAL_SWEPT_FREE);
    }
    
    void loop(const bool isEnter, const stateCommon::LoopData& data){
        bool isPressed = buttonDoubleClicked();
        if(isEnter){
            buttonDoubleClicked();
            timer::cancel();
            _isSwept = false;
            _isLastSweep = false;
            _isSpinning = false;
            resetDistances();
        }
        if(data.action.type == GAME_DEF_ACTION_BECOME_MINE){
            stateCommon::handleStateChange(GAME_DEF_STATE_MINE);
            return;
        }

        if(data.action.type == GAME_DEF_ACTION_BECOME_END){
            stateEnd::setVictory(data.action.payload);
            stateCommon::handleStateChange(GAME_DEF_STATE_END);
            return;
        }

        updateDistances(data);
        updateSweep(isPressed, data);
        updateInform(data);

        draw(GREEN);

    }

    

}