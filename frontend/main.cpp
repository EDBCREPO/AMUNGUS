#define NODEPP_MODULE_NAME "module"

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h>
#include <nodepp/crypto.h>
#include <nodepp/webrtc.h>
#include <nodepp/http.h>
#include <nodepp/json.h>
#include <nodepp/bind.h>
#include <nodepp/ws.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

#include <ungine/ungine.h>
#include <apify/apify.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

#include "controller/control/wrtc.cpp"
#include "controller/control/ws.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

#include "controller/control/modal.cpp"
#include "controller/control/menu.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

#include "controller/game/minigame.cpp"
#include "controller/game/utils.cpp"
#include "controller/game/player.cpp"
#include "controller/game/world.cpp"
#include "controller/game/skeld.cpp"
#include "controller/game/task.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

#include "controller/scene/game.cpp"
#include "controller/scene/menu.cpp"
#include "controller/scene/main.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { void main() { controller::main_scene(); }}

/*────────────────────────────────────────────────────────────────────────────*/

void onMain(){
    ungine::engine::start  ( 1024, 1024, "MyGame" );
    ungine::engine::set_fps( 30 ); ungine::main();
}

/*────────────────────────────────────────────────────────────────────────────*/