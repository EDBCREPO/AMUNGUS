#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace controller {
    
    node_t skeld_collision() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto raw = fs::read_file( "assets/object/collision.json" ).await();
        if( !raw ){ throw except_t( "file not found" ); }
        auto tmp = json::parse( raw.value() );
        auto ply = self->get_root()->get_child("player");

        for( auto &x: tmp["col"].as<array_t<object_t>>() ){
            box_t box; box.min = vec3_t({
                x["min"][0].as<float>(),
                x["min"][1].as<float>(),
                x["min"][2].as<float>()
            }); box.max = vec3_t({
                x["max"][0].as<float>(),
                x["max"][1].as<float>(),
                x["max"][2].as<float>()
            }); x = box;
        }
        
        self->get_root()->onSignal( "ply_box", [=]( any_t raw ){
        auto box = raw.as<box_t>();
            
            tmp["col"].as<array_t<object_t>>().map([=]( object_t raw ){ auto bound = raw.as<box_t>();
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

            }});

        });

    }); }
    
    /*─······································································─*/

    node_t skeld_light() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto raw = fs::read_file( "assets/object/light.json" ).await();
        auto jsn = json::parse( raw.value() );

        for( auto &x: jsn/*-*/.as<array_t<object_t>>() ){
        for( auto &y: x["box"].as<array_t<object_t>>() ){
            box_t box; box.min = vec3_t({
                y["min"][0].as<float>(),
                y["min"][1].as<float>(),
                y["min"][2].as<float>()
            })*3; /**/ box.max = vec3_t({
                y["max"][0].as<float>(),
                y["max"][1].as<float>(),
                y["max"][2].as<float>()
            })*3; y = box; }
        for( auto &y: x["lgh"].as<array_t<object_t>>() ){
            vec3_t pos = vec3_t({
                y["pos"][0].as<float>(),
                y["pos"][1].as<float>(),
                y["pos"][2].as<float>()
            })*3; y = pos;
        }}

        self->get_root()->onSignal( "ply_box", [=]( any_t raw ){
        auto box = raw.as<box_t>();

            while( !self->has_attribute /*--*/ ( "address" ) ){
            for  ( auto &x: jsn/*-*/.as<array_t<object_t>>() ){
            for  ( auto &y: x["box"].as<array_t<object_t>>() ){
            if   ( rl::CheckCollisionBoxes(box,y.as<box_t>() )){
            for  ( auto &x: self->get_children() ){ x->free(); }
            for  ( auto &z: x["lgh"].as<array_t<object_t>>() ){

            self->append_child( node::node_light([=]( ptr_t<node_t> self ){
                auto pos = self->get_attribute<transform_3D_t>("transform");
                auto lgh = self->get_attribute<light_t>       ("light");
                pos->position = z.as<vec3_t>(); lgh->energy = 80.;
                lgh->mode     = light::MODE::LIGHT_MODE_POINT;
                lgh->color    = rl::WHITE;
            }) );

            } self->set_attribute( "address", x ); return; }} } break; }

            auto adr = *self->get_attribute<object_t>( "address" );

            if ( adr["box"].as<array_t<object_t>>().none([=]( object_t x ){
                 return rl::CheckCollisionBoxes( box, x.as<box_t>() ); 
            })){ self->remove_attribute( "address" ); return; }

        });

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
        self->append_child( "light", skeld_light/**/() );

    }); }

}}

/*────────────────────────────────────────────────────────────────────────────*/