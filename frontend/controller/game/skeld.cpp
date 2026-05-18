#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace controller {
    
    node_t skeld_collision() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto obj =(*self->get_root()->get_attribute<object_t>( "object" ))["collision"];
        auto ply =  self->get_root()->get_child("player");
        auto idx = ptr_t<int>( 0UL, -1 );

        self->get_root()->onSignal( "ply_box", [=]( any_t raw ){
        auto box = raw.as<box_t>();

            while( *idx == -1 ){ int c=0; for( auto x: obj .as<array_t<object_t>>() ){
            /*-------------------------*/ for( auto y: x[0].as<array_t<object_t>>() ){

                auto bxx = y.as<box_t>();

                if( rl::CheckCollisionBoxes( bxx, box ) )
                  { *idx = c; return; }

            } c++; } return; }

            for( auto x: obj[*idx][1].as<array_t<object_t>>() ){ auto bound = x.as<box_t>();
            if ( rl::CheckCollisionBoxes( box, bound ) ){
                
                float overlap_x = fminf(box.max.x, bound.max.x) - fmaxf(box.min.x, bound.min.x);
                float overlap_z = fminf(box.max.z, bound.max.z) - fmaxf(box.min.z, bound.min.z);

                box_t  push = { 0 };
                vec3_t pa   = ( box  .max + box  .min ) / 2;
                vec3_t pb   = ( bound.max + bound.min ) / 2;

                if( overlap_x < overlap_z ){
                    push.min.x = ( pa.x > (bound.min.x + bound.max.x)/2 ? overlap_x : -overlap_x);
                } else {
                    push.min.z = ( pa.z > (bound.min.z + bound.max.z)/2 ? overlap_z : -overlap_z);
                }

                self->get_root()->onSignal.emit( "ply_collision", push );

            }};

            for( auto y: obj[*idx][0].as<array_t<object_t>>() ){
                 auto bxx = y.as<box_t>();
            if ( rl::CheckCollisionBoxes( bxx, box ) ){ return; }}
            
            *idx = -1;

        });

    }); }
    
    /*─······································································─*/

    node_t skeld_light() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto obj = (*self->get_root()->get_attribute<object_t>( "object" ))["light"];

        self->get_root()->onSignal( "ply_box", [=]( any_t raw ){
        auto box = raw.as<box_t>();

            while( !self->has_attribute /**/ ("address") ){
            for  ( auto &x: obj .as<array_t<object_t>>() ){
            for  ( auto &y: x[0].as<array_t<object_t>>() ){
            if   ( rl::CheckCollisionBoxes(box,y.as<box_t>() )){
            for  ( auto &x: self->get_children() )  { x->free(); }
            for  ( auto &z: x[1].as<array_t<object_t>>() ){

            self->append_child( node::node_light([=]( ptr_t<node_t> self ){
                auto pos = self->get_attribute<transform_3D_t>("transform");
                auto lgh = self->get_attribute<light_t>       ("light");
                pos->position = z.as<vec3_t>(); 
                lgh->energy   = 80.; pos->position.y = 8; 
                lgh->mode     = light::MODE::LIGHT_MODE_POINT;
                lgh->color    = rl::WHITE;
            }) );

            } self->set_attribute( "address", x ); return; }} } return; }

            auto adr = *self->get_attribute<object_t>( "address" );

            if ( adr[0].as<array_t<object_t>>().none([=]( object_t x ){
                 return rl::CheckCollisionBoxes( box, x.as<box_t>() ); 
            })){ self->remove_attribute( "address" ); return; }

        });

    }); }

    /*─······································································─*/

    node_t skeld_door(){ return node::node_3D([=]( ptr_t<node_t> self ){

        auto obj = (*self->get_root()->get_attribute<object_t>("object"))["door"];

        self->on3D([=](){ for( auto x: obj.as<array_t<object_t>>() ){
            auto tmp = x.as<vec4_t>(); auto pos = vec3_t({ tmp.x, tmp.y, tmp.z });
            rl::DrawSphere( pos, 1, type::cast<int>(tmp.w)==1 ? rl::YELLOW : rl::GREEN );
        } });

    }); }

    /*─······································································─*/

    node_t skeld_model() { return node::node_model( 
         model::load("./assets/model/skeld.glb"), 
    [=]( ptr_t<node_t> self ){

        auto pos = self->get_attribute<transform_3D_t>( "transform" );
        pos->scale = vec3_t({ 3, 3, 3 });

    }); }

    /*─······································································─*/

    node_t skeld() { return node::node_3D([=]( ptr_t<node_t> self ){

        self->append_child( "walls", skeld_collision() );
        self->append_child( "model", skeld_model/**/() );
    //  self->append_child( "light", skeld_light/**/() );
    //  self->append_child( "door" , skeld_door/*-*/() );

    }); }

}}

/*────────────────────────────────────────────────────────────────────────────*/