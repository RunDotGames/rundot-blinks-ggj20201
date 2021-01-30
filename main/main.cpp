#include "main.h"
#include "animate.h"
#include "state--common.h"
#include "timer.h"
#include "game-def.h"
#include "state-board.h"
#include "state-mine.h"
#include "state-swept.h"

namespace _main_ {
    
    void setup() {
        stateCommon::addState(stateBoard::loop, GAME_DEF_STATE_BOARD);
        stateCommon::addState(stateMine::loop, GAME_DEF_STATE_MINE);
        stateCommon::addState(stateSwept::loop, GAME_DEF_STATE_SWEPT);
    }

    
    void loop() {
        timer::loop();
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
            
            // if(!globalEvents::isHandled(data)) {
            
            stateCommon::loop(data);
            
            // }
        }
        stateCommon::LoopData empty{.face=FACE_COUNT, .action=action::Action{.type=ACTION_INVALID, .payload=ACTION_INVALID}};
        
        // if(!globalEvents::isHandled(empty)) {
        
        stateCommon::loop(empty);
        
        // }
        
    }

}
