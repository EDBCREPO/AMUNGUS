#pragma once

namespace ungine { namespace controller {

    node_t world() { return node::node_3D([]( ptr_t<node_t> self ){
        
        self->get_viewport()->background = color::hex( "000000FF" );
        
        self->append_child( node::node_light([=]( ptr_t<node_t> self ){

            auto pos = self->get_attribute<transform_3D_t>( "transform" );
            auto lgh = self->get_attribute<light_t>       ( "light" );

            pos->position = vec3_t({ 0, 15, 0 });

            lgh->mode   = light::MODE::LIGHT_MODE_AMBIENT;
            lgh->energy = 10.f;
            lgh->color  = color_t({ 255, 255, 255, 255 }); 

        }));

        self->on2D([=](){ rl::DrawFPS ( 10 , 10 ); });

        auto shd = shader::load_canvas_shader( _STRING_(
        float noise( vec2 co ){
            return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
        }
        material_t onMain( material_t mat ){

            vec2 texl = 1./vec2(
            /*--------*/ unpack( fetch3D( global, ivec3(  6, 0, 1 ), matrix_size() ) ),
            /*--------*/ unpack( fetch3D( global, ivec3(  6, 0, 0 ), matrix_size() ) ));
            float flag = unpack( fetch3D( global, ivec3( 11, 0, 0 ), matrix_size() ) );
            float time = unpack( fetch3D( global, ivec3(  2, 0, 0 ), matrix_size() ) );

            vec4 t = texture3D( normal, vec3( mat.uv + vec2(-1.,-1.)*texl, 0 ), 7 )*-1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 0.,-1.)*texl, 0 ), 7 )* 0.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 1.,-1.)*texl, 0 ), 7 )* 1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2(-1., 0.)*texl, 0 ), 7 )*-1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 0., 0.)*texl, 0 ), 7 )* 0.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 1., 0.)*texl, 0 ), 7 )* 1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2(-1., 1.)*texl, 0 ), 7 )*-1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 0., 1.)*texl, 0 ), 7 )* 0.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 1., 1.)*texl, 0 ), 7 )* 1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2(-1.,-1.)*texl, 0 ), 7 )* 1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 0.,-1.)*texl, 0 ), 7 )* 1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 1.,-1.)*texl, 0 ), 7 )* 1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2(-1., 0.)*texl, 0 ), 7 )* 0.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 0., 0.)*texl, 0 ), 7 )* 0.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 1., 0.)*texl, 0 ), 7 )* 0.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2(-1., 1.)*texl, 0 ), 7 )*-1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 0., 1.)*texl, 0 ), 7 )*-1.
            /*--*/ + texture3D( normal, vec3( mat.uv + vec2( 1., 1.)*texl, 0 ), 7 )*-1.;

        //  float line = 1. - step( 0.5, abs( t.r + t.g + t.b ) );
            float line = 1. - step( 0.5, t.r + t.g + t.b );
            float focs = 1. - smoothstep( 0.0, 0.2, mat.depth );

            if( flag == 2. ){
                float n = noise( mat.uv * time * 10. ) * 0.15;
                mat.fragment= vec4( vec3( 0., 1.+n, 0. ) *
                    min( mat.light, vec3( 0.45 ) )
                , mat.model * line ); 
            return mat; }

            if( flag == 1. ){ mat.fragment = vec4( max(
                vec3( mat.albedo * focs      ),
                vec3( mat.albedo * mat.light )
            ) , mat.model * line ); return mat; }

            mat.fragment = vec4( mat.albedo * mat.light, mat.model * line ); 

        return mat; } ));

        auto shp = self->get_root()->get_attribute<shape_3D_t>( "shape" );
        shp->model.materials[0].shader = shd;

        self->onClose([=]( node_t* ){ shader::unload(shd); });

    }); }

} }