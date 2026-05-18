#pragma once

namespace ungine { namespace controller {

    object_t task_emoji( string_t name ) { static object_t obj ({
        { "map"     , array_t<object_t>({ rect_t({ 6*128, 0*128, 128, 128 }), 0, "💻" }) },
        { "ad"      , array_t<object_t>({ rect_t({ 6*128, 0*128, 128, 128 }), 1, "🎯" }) },
        { "custom"  , array_t<object_t>({ rect_t({ 7*128, 6*128, 128, 128 }), 2, "👔" }) },
        { "scan"    , array_t<object_t>({ rect_t({ 1*128, 3*128, 128, 128 }), 3, "🧬" }) },
        { "vent"    , array_t<object_t>({ rect_t({ 1*128, 2*128, 128, 128 }), 4, "📥" }) },
        { "alert"   , array_t<object_t>({ rect_t({ 2*128, 1*128, 128, 128 }), 5, "📢" }) },
        { "camera"  , array_t<object_t>({ rect_t({ 1*128, 4*128, 128, 128 }), 6, "📺" }) },
        { "gas"     , array_t<object_t>({ rect_t({ 8*128, 7*128, 128, 128 }), 7, "⛽" }) },
        { "nav"     , array_t<object_t>({ rect_t({ 0*128, 1*128, 128, 128 }), 8, "🌕" }) },
        { "planet"  , array_t<object_t>({ rect_t({ 6*128, 1*128, 128, 128 }), 9, "🌎" }) },
        { "simon"   , array_t<object_t>({ rect_t({ 5*128, 1*128, 128, 128 }),10, "🧩" }) },
        { "order"   , array_t<object_t>({ rect_t({ 1*128, 5*128, 128, 128 }),11, "📟" }) },
        { "card"    , array_t<object_t>({ rect_t({ 5*128, 6*128, 128, 128 }),12, "🪪" }) },
        { "circle"  , array_t<object_t>({ rect_t({ 7*128, 3*128, 128, 128 }),13, "⚡"  }) },
        { "smotor"  , array_t<object_t>({ rect_t({ 8*128, 1*128, 128, 128 }),14, "🚀" }) },
        { "astrod"  , array_t<object_t>({ rect_t({ 8*128, 8*128, 128, 128 }),15, "🔫" }) },
        { "oxygen"  , array_t<object_t>({ rect_t({ 6*128, 4*128, 128, 128 }),16, "🌿" }) },
        { "shield"  , array_t<object_t>({ rect_t({ 7*128, 2*128, 128, 128 }),17, "🛡️" }) },
        { "trash"   , array_t<object_t>({ rect_t({ 3*128, 5*128, 128, 128 }),18, "🗑️" }) },
        { "wire"    , array_t<object_t>({ rect_t({ 7*128, 3*128, 128, 128 }),19, "⚡"  }) },
        { "data"    , array_t<object_t>({ rect_t({ 2*128, 7*128, 128, 128 }),20, "🗂️" }) },
        { "sradio"  , array_t<object_t>({ rect_t({ 2*128, 6*128, 128, 128 }),21, "📻" }) },
        { "sreactor", array_t<object_t>({ rect_t({ 0*128, 7*128, 128, 128 }),22, "☢️" }) },
        { "slight"  , array_t<object_t>({ rect_t({ 2*128, 2*128, 128, 128 }),23, "💡" }) },
        { "med"     , array_t<object_t>({ rect_t({ 5*128, 8*128, 128, 128 }),24, "🔬" }) },
        { "soxygen" , array_t<object_t>({ rect_t({ 8*128, 5*128, 128, 128 }),25, "☣️" }) },
        { "cmpt"    , array_t<object_t>({ rect_t({ 8*128, 0*128, 128, 128 }),26, "🌟" }) },
    }); return obj[name]; }

    /*─······································································─*/

    node_t task_sbtg() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto obj = (*self->get_root()->get_attribute<object_t>("object"))["sabotage"];
        auto img = (*self->get_parent()->get_attribute<texture_t>( "emoji" ));
        auto tmp = ptr_t<object_t>( 0UL, nullptr );

        self->get_root()->onSignal( "ply_pos", [=]( any_t raw ){

            auto pos = raw.as<vec3_t>(); if ( tmp->has_value() ){
            if( math::distance( (*tmp)["pos"].as<vec3_t>(), pos )>10 ){ 
                self->get_root()->onSignal.emit( "ply_task", nullptr );
                (*tmp).clear(); 
            } return; }
            
            for( auto x: obj.keys() ){ 
            for( auto y: obj[x].as<array_t<object_t>>() ){
            if ( math::distance( y.as<vec3_t>(), pos )<10 ){
                *tmp = object_t({
                    { "name", x }, { "pos", y },
                    { "emj" , task_emoji( x ) }
                }); 
                self->get_root()->set_attribute( "sbtg"    , *tmp );
                self->get_root()->onSignal.emit( "ply_task", *tmp );
            }}}
    
        });

        self->on2D([=](){
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        for( auto x: obj.keys() ){ 
        for( auto y: obj[x].as<array_t<object_t>>() ){
        auto c = self->get_viewport()->camera3D;
        auto z = cam3D::world_2_vec( *c, y.as<vec3_t>()); 
             rl::DrawCircle( z.x, z.y, 5, rl::YELLOW );
        }} });

        self->on2D([=](){ 
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        auto c = self->get_viewport()->camera3D; if( tmp->has_value() ){
        auto z = cam3D::world_2_vec( *c, (*tmp)["pos"].as<vec3_t>() ); 
        
            rl::DrawCircle( z.x, z.y, 24, color_t({ 0x22, 0x22, 0x22, 0xff }) );
            rl::DrawTexturePro( 
                img, (*tmp)["emj"][0].as<rect_t>(),
                rect_t({ z.x, z.y, 24, 24 }),
                vec2_t({ 12, 12 }), 0.f, rl::WHITE
            );

        }});

    }); }

    /*─······································································─*/

    node_t task_vent() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto obj = (*self->get_root()->get_attribute<object_t>("object"))["vent"];
        auto img = (*self->get_parent()->get_attribute<texture_t>( "emoji" ));
        auto tmp = ptr_t<object_t>( 0UL, nullptr );

        self->get_root()->onSignal( "ply_pos", [=]( any_t raw ){

            auto pos = raw.as<vec3_t>(); if ( tmp->has_value() ){
            if( math::distance( (*tmp)["pos"].as<vec3_t>(), pos ) >9 ){ 
                self->get_root()->onSignal.emit( "ply_task", nullptr );
                (*tmp).clear(); 
            } return; }

            for( auto x: obj.as<array_t<object_t>>() ){ int c=0;
            for( auto y: x  .as<array_t<object_t>>() ){
            if ( math::distance( y.as<vec3_t>(), pos ) <9 ){
                *tmp = object_t({
                    { "name","vent" }, { "c", c },
                    { "pos" , y } , { "vent", x },
                    { "emj" , task_emoji( "vent" ) }
                }); 
                self->get_root()->set_attribute( "vent"    , *tmp );
                self->get_root()->onSignal.emit( "ply_task", *tmp );
            } c++; }}
    
        });

        self->on2D([=](){
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        for( auto x: obj.as<array_t<object_t>>() ){ 
        for( auto y: x  .as<array_t<object_t>>() ){
        auto c = self->get_viewport()->camera3D;
        auto z = cam3D::world_2_vec( *c, y.as<vec3_t>() ); 
             rl::DrawCircleLines( z.x, z.y, 5, rl::WHITE );
        }} });

        self->on2D([=](){ 
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        auto c = self->get_viewport()->camera3D; if( tmp->has_value() ){
        auto z = cam3D::world_2_vec( *c, (*tmp)["pos"].as<vec3_t>() ); 
        
            rl::DrawCircle( z.x, z.y, 24, color_t({ 0x22, 0x22, 0x22, 0xff }) );
            rl::DrawTexturePro( 
                img, (*tmp)["emj"][0].as<rect_t>(),
                rect_t({ z.x, z.y, 24, 24 }),
                vec2_t({ 12, 12 }), 0.f, rl::WHITE
            );

        }});

    }); }

    /*─······································································─*/

    node_t task_ctrl() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto obj = (*self->get_root()->get_attribute<object_t>("object"))["control"];
        auto img = (*self->get_parent()->get_attribute<texture_t>( "emoji" ));
        auto tmp = ptr_t<object_t>( 0UL, nullptr );

        self->get_root()->onSignal( "ply_pos", [=]( any_t raw ){

            auto pos = raw.as<vec3_t>(); if ( tmp->has_value() ){
            if( math::distance( (*tmp)["pos"].as<vec3_t>(), pos ) >9 ){ 
                self->get_root()->onSignal.emit( "ply_task", nullptr );
                (*tmp).clear(); 
            } return; }

            for( auto x: obj.keys() ){ 
            for( auto y: obj[x].as<array_t<object_t>>() ){
            if ( math::distance( y.as<vec3_t>(),pos )<9 ){
                *tmp = object_t({
                    { "name", x },{ "pos" , y },
                    { "emj" , task_emoji( x ) }
                }); 
                self->get_root()->set_attribute( "ctrl"    , *tmp );
                self->get_root()->onSignal.emit( "ply_task", *tmp );
            }}}
    
        });

        self->on2D([=](){
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        for( auto x: obj.keys() ){ for( auto y: obj[x].as<array_t<object_t>>() ){
        auto c = self->get_viewport()->camera3D;
        auto z = cam3D::world_2_vec( *c, y.as<vec3_t>() ); 
             rl::DrawCircleLines( z.x, z.y, 5, rl::WHITE );
        }} });

        self->on2D([=](){ 
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        auto c = self->get_viewport()->camera3D; if( tmp->has_value() ){
        auto z = cam3D::world_2_vec( *c, (*tmp)["pos"].as<vec3_t>() ); 
            
            rl::DrawCircle( z.x, z.y, 24, color_t({ 0x22, 0x22, 0x22, 0xff }) );
            rl::DrawTexturePro( 
                img, (*tmp)["emj"][0].as<rect_t>(),
                rect_t({ z.x, z.y, 24, 24 }),
                vec2_t({ 12, 12 }), 0.f, rl::WHITE
            );

        }});

    }); }

    /*─······································································─*/

    node_t task_task() { return node::node_3D([=]( ptr_t<node_t> self ){


        auto obj = (*self->get_root()->get_attribute<object_t>("object"))["task"];
        auto img = (*self->get_parent()->get_attribute<texture_t>( "emoji" ));
        auto tmp = ptr_t<object_t>( 0UL, nullptr );

        self->get_root()->onSignal( "ply_pos", [=]( any_t raw ){

            auto pos = raw.as<vec3_t>(); if ( tmp->has_value() ){
            if( math::distance( (*tmp)["pos"].as<vec3_t>(), pos ) >9 ){ 
                self->get_root()->onSignal.emit( "ply_task", nullptr );
                (*tmp).clear(); 
            } return; }

            for( auto x: obj.keys() ){ 
            for( auto y: obj[x].as<array_t<object_t>>() ){
            if ( math::distance( y.as<vec3_t>(), pos ) <9 ){
                *tmp = object_t({
                    { "name", x }, { "pos", y },
                    { "emj" , task_emoji( x ) }
                }); 
                self->get_root()->set_attribute( "task"    , *tmp );
                self->get_root()->onSignal.emit( "ply_task", *tmp );
            }}}
    
        });

        self->on2D([=](){
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        for( auto x: obj.keys() ){ for( auto y: obj[x].as<array_t<object_t>>() ){
        auto c = self->get_viewport()->camera3D;
        auto z = cam3D::world_2_vec( *c, y.as<vec3_t>() ); 
             rl::DrawCircle( z.x, z.y, 5, rl::WHITE );
        }} });

        self->on2D([=](){ 
        if( matrix::get_pixel_float( self->get_viewport()->matrix, 11, 0, 0 )==1. ){ return; }
        auto c = self->get_viewport()->camera3D; if( tmp->has_value() ){
        auto z = cam3D::world_2_vec( *c, (*tmp)["pos"].as<vec3_t>() ); 
            
            rl::DrawCircle( z.x, z.y, 24, color_t({ 0x22, 0x22, 0x22, 0xff }) );
            rl::DrawTexturePro( 
                img, (*tmp)["emj"][0].as<rect_t>(),
                rect_t({ z.x, z.y, 24, 24 }),
                vec2_t({ 12, 12 }), 0.f, rl::WHITE
            );

        }});

    }); }

    /*─······································································─*/

    node_t task() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto img = texture::load( "assets/image/emoji.png" );
        self->set_attribute( "emoji", img );
        self->onClose([=](){ texture::unload(img); });

        self->append_child( "sbtg", task_sbtg() );
        self->append_child( "task", task_task() );
        self->append_child( "ctrl", task_ctrl() );
        self->append_child( "vent", task_vent() );

    }); }

} }