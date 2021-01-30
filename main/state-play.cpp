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
    
    void loop(const bool isEnter, const stateCommon::LoopData& data){
        bool isPressed = buttonSingleClicked();
        if(isEnter){
            // timer::mar
            _isSwept = false;
            _isLastSweep = false;
            FOREACH_FACE(f){
                _distances[f] = GAME_DEF_DIST_INVALID;
            }
            _myDistance = GAME_DEF_DIST_INVALID;
        }
        updateDistances(data);
        updateSweep(isPressed, data);
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