#pragma once

namespace ungine { namespace controller { 

    struct peer_t {
        queue_t<string_t> candidate;
        webrtc_t /*----*/ peer;
    };

    handler_t<peer_t> p2p_handler;

    /*─······································································─*/
    
    void p2p_send( string_t method, string_t path, string_t message ){
        p2p_handler->map([=]( ptr_t<peer_t> raw ){
            if( raw.null() ){ return; }
            apify::add(raw->peer).emit( method, path, message );
        });
    }

    optional_t<webrtc_t> p2p_get( uchar64 handler ){
        auto raw = p2p_handler.read( handler );
    if( raw.null() ){ return nullptr; }
        return raw->peer;
    }

    bool p2p_has( uchar64 handler ){ 
         return p2p_handler.has( handler );
    }

    void p2p_map( function_t<void,webrtc_t> callback ){ 
        p2p_handler->map([=]( ptr_t<peer_t> raw ){
        if( !raw.null() ){ callback( raw->peer ); } });
    }

    /*─······································································─*/

    void p2p_disconnect( uchar64 handle ){ p2p_handler.remove( handle ); }

    void p2p_disconnect() { p2p_handler.clear(); }

    /*─······································································─*/

    apify_host_t<webrtc_t> p2p_tunnel(){
        auto app = apify::add<webrtc_t>();
    
        app.on( "/addICE", [=]( apify_t<webrtc_t> cli ){
            console::log( cli.message );
        //  cli->append_ice_candidate( json::parse( cli.message ) );
        });

        app.on( "/message", [=]( apify_t<webrtc_t> cli ){

        });

        app.on( "/update" , [=]( apify_t<webrtc_t> cli ){

        });

        return app;
    }

    /*─······································································─*/

    promise_t<string_t,except_t> p2p_accept_offer( uchar64 handle, string_t spd ){
    return promise_t<string_t,except_t>([=](
           res_t<string_t> res, rej_t<except_t> rej
    ){
        
        auto raw = p2p_handler.read( handle ); if( raw.null() ){ 
             rej( except_t( "not found" ) ); 
        return; }
        
        raw->peer.accept_offer( spd ).join( res, rej );

    }); }

    promise_t<string_t,except_t> p2p_accept_answer( uchar64 handle, string_t spd ){
    return promise_t<string_t,except_t>([=](
           res_t<string_t> res, rej_t<except_t> rej
    ){

        auto raw = p2p_handler.read( handle ); if( raw.null() ){ 
             rej( except_t( "not found" ) ); 
        return; }
        
        raw->peer.accept_answer( spd ).join( res, rej );

    }); }

    promise_t<string_t,except_t> p2p_create_offer( uchar64 handle ){
    return promise_t<string_t,except_t>([=](
           res_t<string_t> res, rej_t<except_t> rej
    ){

        auto raw = p2p_handler.read( handle ); if( raw.null() ){ 
             rej( except_t( "not found" ) ); 
        return; }
        
        raw->peer.create_offer().join( res, rej );

    }); }

    void p2p_append_ice_candidates( uchar64 handle, object_t ice ) {
        auto raw = p2p_get( handle ); 
    if( !raw.has_value() ){ return; }
        raw.value().append_ice_candidate( ice );
    }

    ptr_t<string_t> p2p_get_ice_candidates( uchar64 handle ){

        auto raw = p2p_handler.read( handle ); 
        if( raw.null() ){ return nullptr; }
        auto ptr = raw->candidate.data();

        raw->candidate.clear(); return ptr;

    }

    /*─······································································─*/
    
    uchar64 p2p_connect( agent_webrtc_t agent ){

        auto app = apify::add<webrtc_t>();
        auto tmp = p2p_handler.create();
        auto cli = webrtc_t( &agent );
        auto peer= peer_t();

        app.add( p2p_tunnel() );
        peer.peer = cli; p2p_handler.update( tmp, peer );

        cli.onError  ([=]( except_t ){ p2p_handler.remove(tmp); });
        cli.onSignal ([=]( string_t data ){
            auto raw = p2p_handler.read( tmp );
        if( raw.null() ){ return; }
            auto cli = raw->peer;
        if( cli.is_closed() ){ raw->candidate.push(data); return; }
            apify::add( cli ).emit( nullptr, "/iceCandidate", data );
        });

        cli.onConnect([=]( webrtc_t cli ){

            cli.onData ([=]( string_t data ){
            if( app.next( cli, data )==0 ){ cli.close(); } });

            cli.onClose([=](){ 
                p2p_handler.remove(tmp); 
                console::log( "disconnected" );
            }); console::log( "-->connected" );

        });

    return tmp; }

}}