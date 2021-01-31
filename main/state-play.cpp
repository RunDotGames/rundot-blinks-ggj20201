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
    byte _lieDirection;

    bool isLastSwept(){
        return _isLastSweep;
    }

    void drawPointers(bool isHard){
        byte pointerCount = 0;
        byte pointerFace = FACE_COUNT;
        Color color = YELLOW;
        if(isHard){
            color = RED;
        }
        FOREACH_FACE(f){
            byte fromOnFace =_distances[f] + 1;
            if(_myDistance  != fromOnFace){
                continue;
            }

            setColorOnFace(color, f);
            pointerCount++;
            pointerFace = f;
        }
        if(!isHard || pointerCount > 1) {
            return;
        }
        if(_lieDirection == FACE_COUNT){
            if(random(1) == 0){
                _lieDirection = FACE_COUNT + 1;
            } else {
                _lieDirection = FACE_COUNT - 1;
            }
        }
        setColorOnFace(color, (pointerFace + _lieDirection) % FACE_COUNT);
    }

    void drawDebug(){
        setColor(BLUE);
        if(_isSwept){
            setColor(dim(GREEN, 64));
        }
        if(_isLastSweep){
            setColor(GREEN);
        }
        drawPointers(false);
    }

    void drawStandard(Color backing, bool isHardMode){
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

        drawPointers(isHardMode);
    }

    void draw(Color backing){
        switch (globals::getDisplayMode()) {
            case GLOBALS_DISPLAY_MODE_DEBUG:
                drawDebug();
                break;
            case GLOBALS_DISPLAY_MODE_HARD:
                drawStandard(backing, true);
                break;
            default:
                drawStandard(backing, false);
                break;
        }
    }

    void resetDistances(){
        _lieDirection = FACE_COUNT;
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