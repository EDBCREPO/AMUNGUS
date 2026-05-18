#pragma once 

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace controller { node_t game_scene() {

    return node::node_scene( [=]( ptr_t<node_t> self ){
        
        controller::json_init( self );
        
        self->append_child( "ctrl"  , controller::mouse () );
        self->append_child( "world" , controller::world () );
        self->append_child( "task"  , controller::task  () );
        self->append_child( "skeld" , controller::skeld () );
        self->append_child( "player", controller::player() );

        EM_EVAL( _STRING_(
        document.querySelector("body").setAttribute( "mode", "game" ); ));

    });

}}}

/*────────────────────────────────────────────────────────────────────────────*/