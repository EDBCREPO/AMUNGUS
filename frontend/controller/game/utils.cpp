#pragma once

namespace ungine { namespace controller {

    node_t mouse() { return node_t([=]( ptr_t<node_t> self ){

        vec3_t mousePosition ({ 0., 0., 0. }); node_t* root = self->get_root();
        root->set_attribute( "mousePosition", mousePosition );
        self->get_root()->onSignal.clear( "ctr_mouse_pos" );

        auto mpos = root->get_attribute<vec3_t>( "mousePosition" );
        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); 
             mpos->x = value["x"].as<float>();
             mpos->y = value["y"].as<float>();
             mpos->z = value["z"].as<float>();
        self->get_root()->onSignal.emit( "ctr_mouse_pos", *mpos );
        return 1; });

        EM_VAL abort = EM_EVAL( _STRING_(
        const controller = new AbortController();

            window.addEventListener('mousedown', (e) => {
            ${0}._nodepp_invoke_( "${1}", {
                x: e.clientX, y: e.clientY, z: e.buttons
            }); }, { signal: controller.signal });

            window.addEventListener('mousemove', (e) => {
            ${0}._nodepp_invoke_( "${1}", {
                x: e.clientX, y: e.clientY, z: e.buttons
            }); }, { signal: controller.signal });

            window.addEventListener('mouseup', (e) => {
            ${0}._nodepp_invoke_( "${1}", {
                x: e.clientX, y: e.clientY, z: e.buttons
            }); }, { signal: controller.signal });

            return controller;
        ), NODEPP_MODULE_NAME, addr );
        
        self->onClose([=](){
            EM_CALL( abort, "abort" );
            process::revoke( addr );
        });

    }); }

    /*─······································································─*/

    void json_init( ptr_t<node_t> self ) {

        auto raw = fs::read_file( "assets/json/skeld.json" ).await();
        auto obj = json::parse( raw.value() ); 

        for( auto &x: obj["sabotage"].keys() ){
        for( auto &y: obj["sabotage"][x].as<array_t<object_t>>() ){
             auto p = vec3_t({
                 y[0].as<float>(), 2,
                 y[2].as<float>()
             }); y = p * 3;
        }}
        
        for( auto &x: obj["camera"].as<array_t<object_t>>() ){
            vec3_t pos = vec3_t({
                x[0][0].as<float>(),
                x[0][1].as<float>(),
                x[0][2].as<float>()
            })*3; vec3_t rot = vec3_t({
                x[1][0].as<float>(),
                x[1][1].as<float>(),
                x[1][2].as<float>()
            }); x[0]=pos; x[1]=rot;
        }

        for( auto &x: obj["task"].keys() ){
        for( auto &y: obj["task"][x].as<array_t<object_t>>() ){
             auto p = vec3_t({
                 y[0].as<float>(), 2,
                 y[2].as<float>()
             }); y = p * 3;
        }}

        for( auto &x: obj["vent"].as<array_t<object_t>>() ){
        for( auto &y: x.as<array_t<object_t>>() ){
             auto p = vec3_t({
                 y[0].as<float>(), 0,
                 y[2].as<float>()
             }); y = p * 3;
        }}

        for( auto &x: obj["control"].keys() ){
        for( auto &y: obj["control"][x].as<array_t<object_t>>() ){
             auto p = vec3_t({
                 y[0].as<float>(), 2,
                 y[2].as<float>()
             }); y = p * 3;
        }}

        for( auto &x: obj["light"].as<array_t<object_t>>() ){
        for( auto &y: x[0].as<array_t<object_t>>() ){
            box_t box; box.min = vec3_t({
                y[0][0].as<float>(),
                y[0][1].as<float>(),
                y[0][2].as<float>()
            })*3; /**/ box.max = vec3_t({
                y[1][0].as<float>(),
                y[1][1].as<float>(),
                y[1][2].as<float>()
            })*3; y = box; 
        }
        for( auto &y: x[1].as<array_t<object_t>>() ){
            vec3_t pos = vec3_t({
                y[0].as<float>(),
                y[1].as<float>(),
                y[2].as<float>()
            })*3; y = pos;
        }}

        for( auto &x: obj["door"].as<array_t<object_t>>() ){
            auto tmp = vec4_t ({
                x[0].as<float>(), 2, 
                x[2].as<float>(),
                x[3].as<float>()
            }) * vec4_t({ 3, 1, -3 , 1 }); 
        x = tmp; }

        for( auto &x: obj["collision"].as<array_t<object_t>>() ){
        for( auto &y: x[0].as<array_t<object_t>>() ){
            box_t box; box.min = vec3_t({
                y[0][0].as<float>(),
                y[0][1].as<float>(),
                y[0][2].as<float>()
            })*3; /**/ box.max = vec3_t({
                y[1][0].as<float>(),
                y[1][1].as<float>(),
                y[1][2].as<float>()
            })*3; y = box; 
        }
        for( auto &y: x[1].as<array_t<object_t>>() ){
            box_t box; box.min = vec3_t({
                y[0][0].as<float>(),
                y[0][1].as<float>(),
                y[0][2].as<float>()
            })*3; /**/ box.max = vec3_t({
                y[1][0].as<float>(),
                y[1][1].as<float>(),
                y[1][2].as<float>()
            })*3; y = box; 
        }}

        self->get_root()->set_attribute( "object", obj ); 

    }

}}