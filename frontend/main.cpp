#define NODEPP_MODULE_NAME "module"

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/nodepp.h>
#include <ungine/ungine.h>
#include <nodepp/json.h>
#include <nodepp/bind.h>
#include <apify/apify.h>
#include <nodepp/ws.h>
#include <nodepp/fs.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

#include "controller/minigame.cpp"
#include "controller/player.cpp"
#include "controller/contrl.cpp"
#include "controller/world.cpp"
#include "controller/skeld.cpp"
#include "controller/task.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { void main() {

    node::node_scene( [=]( ptr_t<node_t> self ){

        self->append_child( "contr" , controller::contrl() );
        self->append_child( "world" , controller::world () );
        self->append_child( "player", controller::player() );
        self->append_child( "skeld" , controller::skeld () );
        self->append_child( "task"  , controller::task  () );
        
    });

}}

/*────────────────────────────────────────────────────────────────────────────*/

void onMain(){
    ungine::engine::start  ( 800, 600, "MyGame" ); // ( 600, 400, "MyGame" );
    ungine::engine::set_fps( 60 ); ungine::main();
}

/*────────────────────────────────────────────────────────────────────────────*/