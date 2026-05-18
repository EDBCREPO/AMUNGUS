#pragma once

namespace ungine { namespace controller { 
    
    ptr_t<pair_t<tcp_t,ws_t>> ws_socket(0UL);
    invoke_t<int,string_t>    ws_invoker;

    /*─······································································─*/

    string_t& ws_endpoint() { thread_local static string_t out; return out; }

    bool  ws_is_connected() { return !ws_socket->second.is_closed(); }

    optional_t<ws_t> ws_client() {
    if   ( ws_socket->second.is_closed() ){ return nullptr; } 
    return ws_socket->second; }

    void ws_close() { 
         ws_socket->first .free(); /*--------------*/ 
         ws_socket->second.free(); ws_invoker.free();
    }

    /*─······································································─*/

    apify_host_t<ws_t> ws_tunnel() {
    apify_host_t<ws_t> app;

        app.on( "/handshake/:room", [=]( apify_t<ws_t> cli ){
            auto dir = path::join( "/tnnl", cli.params["pid"], cli.params["room"] );
            auto hdl = p2p_connect( agent_webrtc_t() );
            
            if( !p2p_has( hdl ) ){
                /*error logic*/    
            return; }

            auto ddr = ws_invoker.add([=]( int mode, string_t message ){ switch( mode ){ 
            case 0: /**/ break;
            case 1: /**/ break; } return -1; });

            p2p_create_offer( hdl )

            .then([=]( string_t spd ){

            })

            .fail([=]( except_t err ){

            });

        cli.done(); });

        app.on( "/connect/:room", [=]( apify_t<ws_t> cli ){
            auto hdl = string::to_u64( cli.params["room"] );
            auto raw = p2p_get( hdl );
        if( raw.has_value() ){ cli.done(); return; }
            raw.value().append_ice_candidate( cli.message );
        });

        app.on( "/ping/:room", [=]( apify_t<ws_t> cli ){
            console::log( "server ping" );
            // send pong
        cli.done(); });

        app.on( [=]( apify_t<ws_t> cli ){
            console::log( ">> not found:", cli.path );
        //  cli.emit( nullptr, dir, cli.message );
        });

    return app; }

    /*─······································································─*/

    promise_t<ws_t,except_t> ws_connect( string_t endp ) { 
    return promise_t<ws_t,except_t>([=](
           res_t<ws_t> res, rej_t<except_t> rej
    ){  

        if( ws_endpoint().empty() ){ rej( "invalid endpoint"  ); return; }
        if( ws_is_connected() )/**/{ res( ws_client().value() ); return; }
        
        ws_endpoint()=endp; ws_close();

        auto srv = ws::connect( endp ); 
        auto app = apify::add<ws_t>( ); 

        app.on( "/prms/:pid/resolve", [=]( apify_t<ws_t> cli ){
        auto pid = string::to_u64( cli.params["pid"] );
            ws_invoker.emit( pid, 1, cli.message );
        cli.done(); });

        app.on( "/prms/:pid/reject", [=]( apify_t<ws_t> cli ){
        auto pid = string::to_u64( cli.params["pid"] );
            ws_invoker.emit( pid, 0, cli.message );
        cli.done(); });

        app.add( "/tnnl/:pid", ws_tunnel() );
        app.on ( [=]( apify_t<ws_t> cli ){
            console::log( "404 error" );
        });

        srv.onConnect([=]( ws_t cli ){

            cli.onData ([=]( string_t data ){ app.next( cli, data ); });
            cli.onClose([=](){
                console::log( "disconnected" );
            }); console::log( "-->connected" );

        ws_socket->second.close();

        ws_socket->second = cli; res( cli ); }); 
        ws_socket->first  = srv;

        srv.onError([=]( except_t err ) { 
            ws_close(); rej( err.what() ); 
        });

    }); }

    /*─······································································─*/

    promise_t<string_t,except_t> ws_fetch( string_t method, string_t path, string_t message ){
    return promise_t<string_t,except_t> ([=]( 
           res_t<string_t> res, rej_t<except_t> rej 
    ){

        ws_connect( ws_endpoint() ).then([=]( ws_t cli ){

            if( ws_socket->second.is_closed() ){ rej( "not connected" ); return; }

            auto pid = ws_invoker.add([=]( int mode, string_t message ){
            switch( mode ){
                case 1: res( message ); break;
                case 0: rej( message ); break;
            }   return -1; });
            
            auto dir = regex::format( path, pid );
            apify::add( ws_socket->second ).emit( method, dir, message );

        }).fail([=]( except_t err ){ rej( err.what() ); }); 
    
    }); }

    void ws_send( string_t method, string_t path, string_t message ){
        if( ws_socket->second.is_closed() ){ return; }
        apify::add( ws_socket->second ).emit( method, path, message );
    }

}}