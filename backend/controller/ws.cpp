#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { struct session_t {
    object_t data;   ws_t  peer; string_t room;
    uchar64  stmp;   ulong size;
    invoke_t<int,string_t> invk;
}; }

namespace controller { 
using  handler_map_t= map_t<string_t,handler_t<session_t>>;    
/*--*/ handler_map_t& get_redux_hadler(){ thread_local static handler_map_t out; return out; }}

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller {
    
    string_t id_to_room( uchar64 rid, string_t region, int cpuid ) {
    string_t tmp( sizeof(uchar64), 0x00 ); 
    
        memcpy( tmp.get(), &rid, sizeof(uchar64) );

        auto enc = crypto::encoder::BASE58(); enc.update(tmp);
        auto out = regex::match_all( enc.get(), "\\w{0,6}" );
        
    out.unshift( region + encoder::hex::atob( cpuid ) ); 
    return out.join("-"); }

    /*.......................................................................*/

    ptr_t<session_t> room_to_id( string_t room, int cpuid ){

        auto spl = string::split( room, "-" );

        if( spl.some([=]( string_t val ){ return val.empty(); }) ){ return nullptr; }
        if( spl[0].slice_view(2) != encoder::hex::atob( cpuid )  ){ return nullptr; }

        auto reg = spl[0].slice(0,2);
        auto que = get_redux_hadler()[reg];

        auto raw = spl.slice_view(1).join("");
        auto dec = crypto::decoder::BASE58( );
        auto rid = type::cast<uchar64>( 0UL );
             dec.update(raw);
        auto tmp = dec.get();

    memcpy( &rid, tmp.get(), sizeof(uchar64) );
    return que.read( rid ); }

    /*.......................................................................*/

    optional_t<pair_t<handler_t<session_t>,uchar64>> get_room( string_t room, int cpuid ){

        auto spl = string::split( room, "-" );

        if( spl.some([=]( string_t val ){ return val.empty(); }) ){ return nullptr; }
        if( spl[0].slice_view(2) != encoder::hex::atob( cpuid )  ){ return nullptr; }

        auto reg = spl[0].slice(0,2);
        auto que = get_redux_hadler()[reg];

        auto raw = spl.slice_view(1).join("");
        auto dec = crypto::decoder::BASE58( );
        auto rid = type::cast<uchar64>( 0UL );
             dec.update(raw);
        auto tmp = dec.get();

    memcpy( &rid, tmp.get(), sizeof(uchar64) );
    return pair_t<handler_t<session_t>,uchar64>({ que, rid }); }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { 

    void ws_get_list( apify_t<ws_t> cli, int cpuid ){

        auto dir = path::join( "/prms", cli.params["pid"], "/resolve" );
        auto reg = cli.params[ "region" ]; auto hdl = get_redux_hadler();
        queue_t<object_t> tmp; int c=100; if( !hdl.has(reg) ){ return; }

        auto   x = hdl[reg].get_queue().last(); 
        while( x!= nullptr && c>0 ){
        tmp.push( object_t({
           { "room", id_to_room( hdl[reg].get_handler(x), reg, cpuid ) },
           { "size", x->data->size }, { "stmp", x->data->stmp },
           { "data", x->data->data }  /*---------------------*/
        }) ); 
        if( x->data->size >= 10 ){ break; } x=x->prev; c--; }

        cli.emit( nullptr, dir, json::stringify( array_t<object_t>(tmp.data()) ) );
    }

    /*.......................................................................*/

    void ws_get_room( apify_t<ws_t> cli, ptr_t<session_t> rid, int cpuid ){
        auto dir = path::join( "/prms", cli.params["pid"], "/resolve" );
 
        cli.emit( nullptr, dir, json::stringify( object_t({
            { "size", rid->size }, { "room", cli.params["room"] },
            { "stmp", rid->stmp }, { "data", rid->data }
        }) ));

    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { apify_host_t<ws_t> ws_resolver_api( int cpuid ){

    auto que = get_redux_hadler();
    auto app = apify::add<ws_t>();

    /*.......................................................................*/

    app.on( "/create/:region", [=]( apify_t<ws_t> cli ){
        auto dir = path::join( "/prms", cli.params["pid"], "/resolve" );
        auto reg = cli.params["region"].slice( 0, 2 );
        if( cli.message.empty() ) /*---*/ { return; }

        do { auto ses = que[reg].read( cli->tag() );
        if ( ses.null() ){ break; } 
             ses->peer = ws_t();
        } while(0);

        if( reg.size() != 2 ){ return; }

        auto rid = que[reg].create(); auto ses = session_t();
             ses.peer = cli.get_socket();  ses.data=json::parse(cli.message);
             ses.stmp = process::now  ();  ses.size=1UL;
             ses.room = id_to_room( rid, reg, cpuid ) ;

        que[reg].update( rid, ses ); cli->tag()= rid;
        cli.emit( nullptr, dir, ses.room );
    });

    /*.......................................................................*/

    app.on( "/pong/:room", [=]( apify_t<ws_t> cli ){
        auto raw = get_room( cli.params["room"], cpuid );
        
        if( !raw.has_value() ) /*----------*/ { return; }
        if( raw.value().second != cli->tag() ){ return; }

        auto obj = json::parse( cli.message );
        auto que = raw.value().first ;
        auto rid = que.read( raw.value().second );

        if( rid.null() ) /**/ { return; } try {

        rid->size = obj["size"].as<float>();
        rid->data = obj["data"];

        } catch( except_t ) {}

    cli.done(); });

    /*.......................................................................*/

    app.on( "/connect/:room", [=]( apify_t<ws_t> cli ){
        auto res = path::join( "/prms", cli.params["pid"], "/resolve" );
        auto rej = path::join( "/prms", cli.params["pid"], "/reject"  );
        auto raw = get_room( cli.params["room"], cpuid );
        
        if( !raw.has_value() ){ return; }

        auto que = raw.value().first ;
        auto rid = que.read( raw.value().second );

        if( cli->tag() != 0x00 )   { cli->close(); }
        if( rid.null() ) /*-----*/ { return; }
        if( rid->peer.is_closed() ){ 
            rid->peer = cli.get_socket(); rid->size = 0;
            cli.emit( nullptr, res, nullptr );
        return; }

        if( rid->size >= 10 ) {
            cli.emit( nullptr, rej, nullptr );
        return; }

        auto addr = rid->invk.add([=]( int mode, string_t message ){ 
        if( mode==1 ) { 
            rid->size += 1; cli->tag()= raw.value().second;
        } switch( mode ){
            case  1: cli.emit( nullptr, res, message ); break;
            case  0: cli.emit( nullptr, rej, message ); break; 
        } return -1; });

        auto dir = path::join( "/tnnl", addr, "connect", cli.params["room"] );
        apify::add( rid->peer ).emit( cli.method, dir, cli.message );
    
    cli.done(); });

    /*.......................................................................*/

    app.on( "/list/:region",[=]( apify_t<ws_t> cli ){ ws_get_list( cli, cpuid ); });
    app.on( "/info/:room"  ,[=]( apify_t<ws_t> cli ){
    auto rid = room_to_id( cli.params["room"], cpuid );
        if ( rid.null() ) /*-----*/ { return; }
    //  if ( rid->peer.is_closed() ){ return; } 
    ws_get_room( cli, rid, cpuid ); });

    /*.......................................................................*/

    app.on([=]( apify_t<ws_t> cli ){
        auto dir= path::join( "/prms", cli.params["pid"], "/reject" );
        cli.emit( nullptr, dir, "not found" );
    });

    /*.......................................................................*/

    process::add( coroutine::add( COROUTINE(){
    coBegin ; coDelay(1); do { 

        if( que.empty() )/*---------------*/{ break; } auto x = que.raw()/*--*/.get();
        if( x ==nullptr )/*---------------*/{ break; } auto y = x->data.second->get();
        if( y ==nullptr )/*-*/{ que.erase(x); break; } auto z = x->data.second;
        if( y->next==nullptr ){ que.raw().next(); }

        do{
        if( y->data->stmp>process::now() ){ z->next(); break; }
        if( y->data->peer.is_available() ){ z->next(); 
            
            auto dir = path::join( "/tnnl/_", "ping", y->data->room );
            y->data->stmp=process::now() + TIME_SECONDS(30); 
            apify::add( y->data->peer ).emit( nullptr, dir, nullptr );

        } else { z->erase(y); return 1;  }} while(0);

        if( y->data->size >= 10 ){ z->move( z->first(), y ); }
        if( x->next==nullptr ){ break; } return 1; 
    
    } while(0); coDelay(10000); coGoto(0); coFinish }));

    return app;
}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { apify_host_t<ws_t> ws_tunnel_api( int cpuid ){
    
    auto app = apify::add<ws_t>();

    /*.......................................................................*/

    app.on( "/:room/:path", [=]( apify_t<ws_t> cli ){
        auto res= path::join( "/prms", cli.params["pid"], "/resolve" );
        auto rej= path::join( "/prms", cli.params["pid"], "/reject"  );
        auto rid = room_to_id( cli.params["room"], cpuid );

        if ( rid.null() ) /*-----*/ { return; }
        if ( rid->peer.is_closed() ){ return; }

        auto raw = rid->invk.add([=]( int mode, string_t message ){ switch( mode ){
        case 1: cli.emit( nullptr, res, message ); break;
        case 2: cli.emit( nullptr, rej, message ); break; } return -1; });

        auto dir = path::join( "/tnnl", raw, cli.params["path"], cli.params["room"] );
        apify::add( rid->peer ).emit( cli.method, dir, cli.message );
        
    cli.done(); });

    /*.......................................................................*/

    app.on( "/:room", [=]( apify_t<ws_t> cli ){
        auto rid = room_to_id( cli.params["room"], cpuid );
        auto adr = string::to_u64( cli.params["pid"] );
        if( rid.null() ) /*-----*/ { return; } 
        if( rid->peer.is_closed() ){
        rid->invk.emit( adr, 0, cli.message ); return; }
        rid->invk.emit( adr, 1, cli.message );
    });

    /*.......................................................................*/

    app.on([=]( apify_t<ws_t> cli ){
        auto dir= path::join( "/prms", cli.params["pid"], "/reject" );
        cli.emit( nullptr, dir, "not found" );
    });

    /*.......................................................................*/

    return app;

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace controller { void ws_server( int port, int cpuid ){

    auto srv = ws   ::server   (); 
    auto app = apify::add<ws_t>();

    app.add( "/tnnl/:pid", ws_tunnel_api  ( cpuid ) );
    app.add( "/room/:pid", ws_resolver_api( cpuid ) );
    app.on ([=]( apify_t<ws_t> cli ){ cli.close(); });

    srv.onConnect([=]( ws_t cli ){

        cli.set_buffer_size (1024); int fd = cli.get_fd();
        cli.set_no_delay_mode( 1 );

        cli.onData ([=]( string_t data ){ 
            if( app.next( cli, data )==0 ){ cli.free(); }
        });

        cli.onClose([=](){
            console::log( "disconnected", fd );
        }); console::log( "-->connected", fd );

    });

    srv.listen( "[::0]", port, [=]( socket_t cli ){
        console::log( string::format( "-> ws://[::1]:%d", port ) );
    });

}}

/*────────────────────────────────────────────────────────────────────────────*/