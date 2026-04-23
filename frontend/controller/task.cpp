#pragma once

namespace ungine { namespace controller {

    rect_t task_emoji( string_t name ) { static object_t obj ({
        { "alert"      , rect_t({ 2*128, 1*128, 128, 128 }) },
        { "map"        , rect_t({ 6*128, 0*128, 128, 128 }) },
        { "gas"        , rect_t({ 8*128, 7*128, 128, 128 }) },
        { "med"        , rect_t({ 5*128, 8*128, 128, 128 }) },
        { "nav"        , rect_t({ 8*128, 1*128, 128, 128 }) },
        { "planet"     , rect_t({ 6*128, 1*128, 128, 128 }) },
        { "camera"     , rect_t({ 1*128, 4*128, 128, 128 }) },
        { "custom"     , rect_t({ 7*128, 6*128, 128, 128 }) },
        { "simon"      , rect_t({ 5*128, 1*128, 128, 128 }) },
        { "order"      , rect_t({ 1*128, 5*128, 128, 128 }) },
        { "card"       , rect_t({ 5*128, 6*128, 128, 128 }) },
        { "circle"     , rect_t({ 7*128, 3*128, 128, 128 }) },
        { "motor"      , rect_t({ 8*128, 1*128, 128, 128 }) },
        { "astrod"     , rect_t({ 8*128, 8*128, 128, 128 }) },
        { "oxygen"     , rect_t({ 6*128, 4*128, 128, 128 }) },
        { "shield"     , rect_t({ 7*128, 2*128, 128, 128 }) },
        { "scann"      , rect_t({ 1*128, 3*128, 128, 128 }) },
        { "trash"      , rect_t({ 3*128, 5*128, 128, 128 }) },
        { "wire"       , rect_t({ 7*128, 3*128, 128, 128 }) },
        { "data"       , rect_t({ 2*128, 7*128, 128, 128 }) },
        { "vent"       , rect_t({ 1*128, 2*128, 128, 128 }) },
        { "reactor"    , rect_t({ 0*128, 7*128, 128, 128 }) },
        { "electricity", rect_t({ 2*128, 2*128, 128, 128 }) },
        { "radio"      , rect_t({ 2*128, 6*128, 128, 128 }) },
    }); return obj[name].as<rect_t>(); }

    /*─······································································─*/

    node_t task_sbtg() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto raw = fs::read_file( "assets/object/sabotage.json" ).await().value();
        auto tsk = object_t(); auto obj = json::parse( raw ); 
        auto img = texture::load( "assets/ui/emoji.png" );
        auto tmp = ptr_t<object_t>( 0UL, nullptr );

        for( auto &x: obj.keys() ){
        for( auto &y: obj[x].as<array_t<object_t>>() ){
             auto p = vec3_t({
                 y["pos"][0].as<float>(), 2,
                 y["pos"][2].as<float>()
             }); y["pos"] = p * 3;
        }}

        self->get_root()->onSignal( "ply_pos", [=]( any_t raw ){

            auto pos = raw.as<vec3_t>(); if ( tmp->has_value() ){
            if( math::distance( (*tmp)["pos"].as<vec3_t>(), pos ) > 15 ){ 
                self->get_root()->onSignal.emit( "ply_task", nullptr );
                (*tmp).clear(); 
            } return; }
            
            for( auto x: obj.keys() ){ 
            for( auto y: obj[x].as<array_t<object_t>>() ){
            if ( math::distance( y["pos"].as<vec3_t>(), pos ) < 15 ){
                *tmp = object_t({
                    { "name", x }, 
                    { "pos", y["pos"] },
                    { "emj" , task_emoji( x ) }
                }); 
                self->get_root()->onSignal.emit( "ply_task", *tmp );
            }}}
    
        });

        self->onClose([=]( node_t* ){ texture::unload(img); });

        self->on2D([=](){
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        for( auto x: obj.keys() ){ for( auto y: obj[x].as<array_t<object_t>>() ){
        auto c = self->get_viewport()->camera3D;
        auto z = cam3D::world_2_vec( *c, y["pos"].as<vec3_t>() ); 
             rl::DrawCircleLines( z.x, z.y, 5, rl::RED );
        }} });

        self->on2D([=](){ 
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        auto c = self->get_viewport()->camera3D; if( tmp->has_value() ){
        auto z = cam3D::world_2_vec( *c, (*tmp)["pos"].as<vec3_t>() ); 
        
            rl::DrawCircle( z.x, z.y, 30, color_t({ 0x22, 0x22, 0x22, 0xff }) );
            rl::DrawTexturePro( 
                img, (*tmp)["emj"].as<rect_t>(),
                rect_t({ z.x, z.y, 30, 30 }),
                vec2_t({ 15, 15 }), 0.f, rl::WHITE
            );

        }});

    }); }

    /*─······································································─*/

    node_t task_vent() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto raw = fs::read_file( "assets/object/vent.json" ).await().value();
        auto tsk = object_t(); auto obj = json::parse( raw ); 
        auto img = texture::load( "assets/ui/emoji.png" );
        auto tmp = ptr_t<object_t>( 0UL, nullptr );

        for( auto &x: obj.keys() ){
        for( auto &y: obj[x].as<array_t<object_t>>() ){
             auto p = vec3_t({
                 y["pos"][0].as<float>(), 0,
                 y["pos"][2].as<float>()
             }); y["pos"] = p * 3;
        }}

        self->get_root()->onSignal( "ply_pos", [=]( any_t raw ){

            auto pos = raw.as<vec3_t>(); if ( tmp->has_value() ){
            if( math::distance( (*tmp)["pos"].as<vec3_t>(), pos ) > 10 ){ 
                self->get_root()->onSignal.emit( "ply_task", nullptr );
                (*tmp).clear(); 
            } return; }

            for( auto x: obj.keys() ){ 
            for( auto y: obj[x].as<array_t<object_t>>() ){
            if ( math::distance( y["pos"].as<vec3_t>(), pos ) < 10 ){
                *tmp = object_t({
                    { "name", x }, 
                    { "pos", y["pos"] },
                    { "emj" , task_emoji( "vent" ) }
                }); 
                self->get_root()->onSignal.emit( "ply_task", *tmp );
            }}}
    
        });

        self->onClose([=]( node_t* ){ texture::unload(img); });

        self->on2D([=](){
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        for( auto x: obj.keys() ){ for( auto y: obj[x].as<array_t<object_t>>() ){
        auto c = self->get_viewport()->camera3D;
        auto z = cam3D::world_2_vec( *c, y["pos"].as<vec3_t>() ); 
             rl::DrawCircleLines( z.x, z.y, 5, rl::WHITE );
        }} });

        self->on2D([=](){ 
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        auto c = self->get_viewport()->camera3D; if( tmp->has_value() ){
        auto z = cam3D::world_2_vec( *c, (*tmp)["pos"].as<vec3_t>() ); 
        
            rl::DrawCircle( z.x, z.y, 30, color_t({ 0x22, 0x22, 0x22, 0xff }) );
            rl::DrawTexturePro( 
                img, (*tmp)["emj"].as<rect_t>(),
                rect_t({ z.x, z.y, 30, 30 }),
                vec2_t({ 15, 15 }), 0.f, rl::WHITE
            );

        }});

    }); }

    /*─······································································─*/

    node_t task_ctrl() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto raw = fs::read_file( "assets/object/task.json" ).await().value();
        auto tsk = object_t(); auto obj = json::parse( raw ); 
        auto img = texture::load( "assets/ui/emoji.png" );
        auto tmp = ptr_t<object_t>( 0UL, nullptr );

        for( auto &x: obj.keys() ){
        for( auto &y: obj[x].as<array_t<object_t>>() ){
             auto p = vec3_t({
                 y["pos"][0].as<float>(), 2,
                 y["pos"][2].as<float>()
             }); y["pos"] = p * 3;
        }}

        tsk["alert"]  = obj["alert"] ; obj.erase("alert" );
        tsk["custom"] = obj["custom"]; obj.erase("custom");
        tsk["camera"] = obj["camera"]; obj.erase("camera");
        tsk["map"]    = obj["map"]   ; obj.erase("map");

        self->get_root()->onSignal( "ply_pos", [=]( any_t raw ){

            auto pos = raw.as<vec3_t>(); if ( tmp->has_value() ){
            if( math::distance( (*tmp)["pos"].as<vec3_t>(), pos ) > 10 ){ 
                self->get_root()->onSignal.emit( "ply_task", nullptr );
                (*tmp).clear(); 
            } return; }

            for( auto x: tsk.keys() ){ 
            for( auto y: tsk[x].as<array_t<object_t>>() ){
            if ( math::distance( y["pos"].as<vec3_t>(), pos ) < 10 ){
                *tmp = object_t({
                    { "name", x }, 
                    { "pos", y["pos"] },
                    { "emj" , task_emoji( x ) }
                }); 
                self->get_root()->onSignal.emit( "ply_task", *tmp );
            }}}
    
        });

        self->onClose([=]( node_t* ){ texture::unload(img); });

        self->on2D([=](){
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        for( auto x: tsk.keys() ){ for( auto y: tsk[x].as<array_t<object_t>>() ){
        auto c = self->get_viewport()->camera3D;
        auto z = cam3D::world_2_vec( *c, y["pos"].as<vec3_t>() ); 
             rl::DrawCircleLines( z.x, z.y, 5, rl::WHITE );
        }} });

        self->on2D([=](){ 
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        auto c = self->get_viewport()->camera3D; if( tmp->has_value() ){
        auto z = cam3D::world_2_vec( *c, (*tmp)["pos"].as<vec3_t>() ); 
            
            rl::DrawCircle( z.x, z.y, 30, color_t({ 0x22, 0x22, 0x22, 0xff }) );
            rl::DrawTexturePro( 
                img, (*tmp)["emj"].as<rect_t>(),
                rect_t({ z.x, z.y, 30, 30 }),
                vec2_t({ 15, 15 }), 0.f, rl::WHITE
            );

        }});

    }); }

    /*─······································································─*/

    node_t task_task() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto raw = fs::read_file( "assets/object/task.json" ).await().value();
        auto tsk = object_t(); auto obj = json::parse( raw ); 
        auto img = texture::load( "assets/ui/emoji.png" );
        auto tmp = ptr_t<object_t>( 0UL, nullptr );

        for( auto &x: obj.keys() ){
        for( auto &y: obj[x].as<array_t<object_t>>() ){
             auto p = vec3_t({
                 y["pos"][0].as<float>(), 2,
                 y["pos"][2].as<float>()
             }); y["pos"] = p * 3;
        }}

        tsk["alert"]  = obj["alert"] ; obj.erase("alert" );
        tsk["custom"] = obj["custom"]; obj.erase("custom");
        tsk["camera"] = obj["camera"]; obj.erase("camera");
        tsk["map"]    = obj["map"]   ; obj.erase("map");

        self->get_root()->onSignal( "ply_pos", [=]( any_t raw ){

            auto pos = raw.as<vec3_t>(); if ( tmp->has_value() ){
            if( math::distance( (*tmp)["pos"].as<vec3_t>(), pos ) > 10 ){ 
                self->get_root()->onSignal.emit( "ply_task", nullptr );
                (*tmp).clear(); 
            } return; }

            for( auto x: obj.keys() ){ 
            for( auto y: obj[x].as<array_t<object_t>>() ){
            if ( math::distance( y["pos"].as<vec3_t>(), pos ) < 10 ){
                *tmp = object_t({
                    { "name", x }, 
                    { "pos", y["pos"] },
                    { "emj" , task_emoji( x ) }
                }); 
                self->get_root()->onSignal.emit( "ply_task", *tmp );
            }}}
    
        });

        self->onClose([=]( node_t* ){ texture::unload(img); });

        self->on2D([=](){
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        for( auto x: obj.keys() ){ for( auto y: obj[x].as<array_t<object_t>>() ){
        auto c = self->get_viewport()->camera3D;
        auto z = cam3D::world_2_vec( *c, y["pos"].as<vec3_t>() ); 
             rl::DrawCircle( z.x, z.y, 5, rl::WHITE );
        }} });

        self->on2D([=](){ 
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        auto c = self->get_viewport()->camera3D; if( tmp->has_value() ){
        auto z = cam3D::world_2_vec( *c, (*tmp)["pos"].as<vec3_t>() ); 
            
            rl::DrawCircle( z.x, z.y, 30, color_t({ 0x22, 0x22, 0x22, 0xff }) );
            rl::DrawTexturePro( 
                img, (*tmp)["emj"].as<rect_t>(),
                rect_t({ z.x, z.y, 30, 30 }),
                vec2_t({ 15, 15 }), 0.f, rl::WHITE
            );

        }});

    }); }

    /*─······································································─*/

    node_t task() { return node::node_3D([=]( ptr_t<node_t> self ){

    //  self->append_child( "sbtg", task_sbtg() );
        self->append_child( "task", task_task() );
        self->append_child( "ctrl", task_ctrl() );
    //  self->append_child( "vent", task_vent() );

    }); }

} }