#include "state-play.h"
#include "game-def.h"
#include "action.h"
#include "state-setup.h"
#include "timer.h"

namespace statePlay {

    byte _distances[FACE_COUNT];

    byte _myDistance = GAME_DEF_DIST_INVALID;

    bool _isSwept;
    bool _isLastSweep;

    void resetDistances(){
        FOREACH_FACE(f){
            _distances[f] = GAME_DEF_DIST_INVALID;
        }
        _myDistance = GAME_DEF_DIST_INVALID;
    }

    void updateSweep(bool pressed, const stateCommon::LoopData& data){
        if(pressed && !_isSwept){
            _isSwept = true;
            _isLastSweep = true;
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
        if(!action::isBroadcastReceived(data.action, GAME_DEF_ACTION_REQUEST_INFORM)){
            return;
        }

        if(_isSwept){
            setValueSentOnAllFaces(GAME_DEF_VAL_SWEPT_TAKEN);
            return;
        }
        setValueSentOnAllFaces(GAME_DEF_VAL_SWEPT_FREE);
    }
    
    void loop(const bool isEnter, const stateCommon::LoopData& data){
        bool isPressed = buttonSingleClicked();
        if(isEnter){
            _isSwept = false;
            _isLastSweep = false;
            resetDistances();
        }
        if(data.action.type == GAME_DEF_ACTION_BECOME_MINE){
            stateCommon::handleStateChange(GAME_DEF_STATE_MINE);
            return;
        }

        updateDistances(data);
        updateSweep(isPressed, data);
        updateInform(data);
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

    }

    

}