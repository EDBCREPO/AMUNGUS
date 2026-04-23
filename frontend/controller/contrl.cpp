#pragma once

namespace ungine { namespace controller {

    node_t mouse_event() { return node_t([=]( ptr_t<node_t> self ){

        vec3_t mousePosition ({ 0., 0., 0. }); node_t* root = self->get_root();
        root->set_attribute( "mousePosition", mousePosition );

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
        
        self->onClose([=]( node_t* ){
            EM_CALL( abort, "abort" );
            process::revoke( addr );
        });

    }); }

    node_t websocket() { return node_t([=]( ptr_t<node_t> self ){
    /*
      
    auto cli = ws::connect( "ws://[::1]:8000" );
    auto apf = apify::add<ws_t>();

    apf.on( "/app/connect", [=]( apify_t<ws_t> cli ){
        console::log( "hello world!", cli.message );
        cli.done();
    });

    apf.on( [=]( apify_t<ws_t> cli ){
        console::log( "404 error" );
    });

    cli.onConnect([=]( ws_t cli ){

        cli.onData([=]( string_t data ){
            console::log( ">>", data );
            apf.next( cli, data );
        });
        
        cli.onClose([=](){
            console::log( "closed" );
        }); console::log( "connected" );

        apify::add(cli).emit( "/app/connect", "mojon" );

    });

    cli.onError([=]( except_t err ){
        console::log( err.what() );
    });

    */
    }); }

    node_t ui_intervace() { return node_t([=]( ptr_t<node_t> self ){

    /*
        string_t addr = process::invoke([=]( any_t raw ){
        EM_VAL   value= raw.as<EM_VAL>();

            switch( value["type"].as<int>() ){

                case 1: do {
                    static bool b=0; b=!b;
                    onLamp.emit( "room", b );
                } while(0); break;

                case 0: do {
                    static bool b=0; b=!b;
                    onLamp.emit( "player", b );
                } while(0); break;

            }

            console::log( "hello world! from C++" );
        return 1; });

        EM_EVAL( _STRING_(

            window.document.querySelector( "[button-a]" ).addEventListener('click',()=>{
                ${1}._nodepp_invoke_( "${0}", { type: 1 } );
            });

            window.document.querySelector( "[button-b]" ).addEventListener('click',()=>{
                ${1}._nodepp_invoke_( "${0}", { type: 0 } );
            });

        ), addr, NODEPP_MODULE_NAME );
    */

    }); }

    node_t contrl() { return node_t([=]( ptr_t<node_t> self ){

        self->append_child( "ws", websocket   () );
        self->append_child( "ui", ui_intervace() );
        self->append_child( "ms", mouse_event () );

    }); }

}}