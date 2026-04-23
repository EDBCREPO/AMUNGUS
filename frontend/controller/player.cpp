#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace controller {

    expected_t<node_t,except_t> player_task( int task ) { switch( task ) {
        case 0 : return minigame_gas    ();     break;
        case 1 : return minigame_moon   ();     break;
        case 2 : return minigame_earth  ();     break;
        case 3 : return minigame_camera ();     break;
        case 4 : return minigame_simon  ();     break;
        case 5 : return minigame_number ();     break;
        case 6 : return minigame_card   ();     break;
        case 7 : return minigame_circle ();     break;
        case 8 : return minigame_motor  ();     break;
        case 9 : return minigame_astrod ();     break;
        case 10: return minigame_oxygen ();     break;
        case 11: return minigame_shield ();     break;
    //  case 12: return minigame_alert  ();     break;
    //  case 13: return minigame_ad     ();     break;
    //  case 14: return minigame_map    ();     break;
    //  case 15: return minigame_custom ();     break;
    //  case 16: return minigame_scann  ();     break;
    //  case 17: return minigame_vent   ();     break;
        case 18: return minigame_trash  ();     break;
        case 19: return minigame_wiring ();     break;
        case 20: return minigame_data   ();     break;
        case 21: return minigame_radio  ();     break;
        case 22: return minigame_reactor();     break;
        case 23: return minigame_electricity(); break;
    } return except_t( "invalid task" ); }

    /*─······································································─*/

    node_t player_ui() { return node_t( [=]( ptr_t<node_t> self ){
        
        auto uif = fs::read_file( "assets/ui/crewmate.html" );
        auto addr= process::invoke([=]( any_t raw ){
        auto val = raw.as<EM_VAL>(); if( self->has_child( "minigame" ) ){ return 1; } 
        auto tsk = player_task( val.as<int>() ); if( !tsk ) /*-------*/ { return 1; }
             self->append_child( "minigame", tsk.value() ); 
        return 1; });

        self->onClose([=]( node_t* ){ process::revoke( addr ); });

        uif.then([=]( string_t data ){

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( "file not found" );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t player_light() { return node::node_light( [=]( ptr_t<node_t> self ){

        auto pos = self->get_attribute<transform_3D_t>( "transform" );
        auto lgh = self->get_attribute<light_t>( "light" );
        auto rot = self->get_root();

        lgh->mode     = light::MODE::LIGHT_MODE_POINT;
        lgh->energy   = 30.f;
        lgh->wrap     = PI/6;
        pos->position = vec3_t ({     0,   6,   0 });
        pos->rotation = vec3_t ({ -PI/2,   0,   0 });
        lgh->color    = color_t({   255, 255, 255, 255 }); 

    }); }

    /*─······································································─*/

    node_t player_model() { return node::node_animated_model( 
        "./assets/model/astronaut.glb", 
    [=]( ptr_t<node_t> self ){

        auto pos = self->get_attribute<transform_3D_t>( "transform" );
        auto shp = self->get_attribute<shape_3D_t>( "shape" );
        auto dir = ptr_t<vec2_t>( 0UL, vec2_t({ 0, 0 }) );

        shp->animation.mode  = animation::MODE::ANIMATION_MODE_PLAY;
        shp->animation.index = 2;
        pos->scale           = vec3_t({ 10, 10, 10 });
        pos->position        = vec3_t({  0,  0,  0 });

        ptr_t<float> speed( 0UL, 15.f );
        ptr_t<float> fovy ( 0UL, 70.f );

    //  self->append_child( "light", player_light() );

        self->onLoop([=]( float delta ){
        *dir = vec2_t({ 0, 0 }); /**/ *speed = 0.0; 
            shp->animation.index = 0 ;* fovy = 68.;
            shp->animation.speed = 0 ;

            if( self->get_node( "../ui/minigame" )!=nullptr )
              { return; }

            if( key::is_down( 'W' ) ){
                shp->animation.index = 2  ;
                shp->animation.speed = 250;
                *fovy  = 70.; dir->y = 1  ; 
            } elif( key::is_down( 'S' ) ) {
                shp->animation.index = 2  ;
                shp->animation.speed = 250;
                *fovy  = 70.; dir->y =-1  ; 
            }

            if( key::is_down( 'A' ) ){
                shp->animation.index = 2  ;
                shp->animation.speed = 250;
                *fovy  = 70.; dir->x = 1  ; 
            } elif( key::is_down( 'D' ) ) {
                shp->animation.index = 2  ;
                shp->animation.speed = 250;
                *fovy  = 70.; dir->x =-1  ; 
            }

            if( key::is_down( key::BUTTON::KEY_LEFT_SHIFT ) ){ do {
            if( shp->animation.index == 0 ){ break; }
                shp->animation.index =  1  ;
                /*---*/ *fovy  = 75.;
            } while(0); *speed = 40.; } else { *speed = 30.; }

            self->get_root()->onSignal.emit( "ply_fovy", *fovy );
            if( dir->x==0 && dir->y==0 ){ return; }

            pos->rotation.y = atan2f( dir->y,-dir->x ) - PI / 2;

        do {
        
            auto tmp = vec3_t({
                 sin(pos->rotation.y)*delta**speed, 0,
                 cos(pos->rotation.y)*delta**speed
            });

            self->get_root()->onSignal.emit( "ply_move", tmp );

        } while(0); });

    }); }

    /*─······································································─*/

    node_t player_camera() { return node::node_camera_3D([=]( ptr_t<node_t> self ){

        auto pos = self->get_attribute<transform_3D_t>( "transform" );
        auto cam = self->get_attribute<camera_3D_t>   ( "camera" );
        /**/ cam->fovy = 70 ; cam->near = 1.; cam->far  = 300;

        self->get_root()->onSignal( "ply_fovy", [=]( any_t raw ){
            auto value= raw.as<float>();
            cam->fovy = math::move_toward( cam->fovy, value, 1 );
        });
        
        pos->position = vec3_t({ 0, 50, -20 });
        pos->rotation = vec3_t({ 4* PI / 10, 0, 0 });

    }); }

    /*─······································································─*/

    node_t player() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto pos = self->get_attribute<transform_3D_t>( "transform" );

        self->get_root()->onSignal( "ply_collision", [=]( any_t raw ){
            auto value       = raw.as<box_t>();
            pos->position.x += value.min.x;
            pos->position.z += value.min.z;
        });

        self->get_root()->onSignal( "ply_move", [=]( any_t raw ){
            auto value     = raw.as<vec3_t>();
            pos->position += value;

            auto box = box_t();
                 box.min = pos->translate.position - vec3_t({2,2,2});
                 box.max = pos->translate.position + vec3_t({2,2,2});
            
        self->get_root()->onSignal.emit( "ply_pos", pos->translate.position );
        self->get_root()->onSignal.emit( "ply_box", box );
        });

        self->append_child( "camera", player_camera() );
        self->append_child( "model" , player_model () );
        self->append_child( "ui"    , player_ui    () );

        self->get_root()->onSignal.on( "ply_task", [=]( any_t val ){
            console::log( ">>", val.has_value() );
        });

    }); }

}}

/*────────────────────────────────────────────────────────────────────────────*/