#include "main.h"
#include "animate.h"
#include "state--common.h"
#include "timer.h"
#include "game-def.h"
#include "state-setup.h"
#include "state-play.h"
#include "state-mine.h"

namespace _main_ {
    
    void setup() {
        stateCommon::addState(stateSetup::loop, GAME_DEF_STATE_SETUP);
        stateCommon::addState(statePlay::loop, GAME_DEF_STATE_PLAY);
        stateCommon::addState(stateMine::loop, GAME_DEF_STATE_MINE);
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
