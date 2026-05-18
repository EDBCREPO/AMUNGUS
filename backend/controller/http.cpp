#pragma once

namespace controller { void http_server( uint port, uint id ){

    auto app = express::http::add();

    app.USE([=]( express_http_t cli, function_t<void> next ){
        cli.header( "Cross-Origin-Opener-Policy"  , "same-origin"  );
        cli.header( "Cross-Origin-Embedder-Policy", "require-corp" );
        cli.header( "Access-Control-Allow-Origin",  "*" ); // <- dominio de CrazyGames
        cli.header( "Access-Control-Allow-Methods", "GET, POST, OPTIONS" );
        cli.header( "Access-Control-Allow-Headers", "Content-Type, Authorization" );
    if( cli.method == "OPTIONS" ) { cli.status(204).send(""); return; } next(); });

    app.GET( "/payload/:gameID", [=]( express_http_t cli ){

        auto raw = string::to_string( process::now( ) )+
                   cli.params["gameID"]                +
                   string::format( "%d-%lu",id,rand() );

        auto hash= crypto::hash::SHA256();
             hash.update( raw );

        int  prt = port + 1; //rand() % os::cpus() + port + 1;
        int  cpu = os::cpus();

    cli.send_json( object_t({
        { "endp", regex::format( "ws://localhost:${0}", prt ) },
        { "rnge", ptr_t<int>({ port+1, port+1+cpu }) },
        { "rand", prt }, { "clid", hash.get() }
    }) ); });

    app.USE( express::http::file( "www" ) );

    app.listen( "[::0]", port, [=]( socket_t cli ){
        console::log( string::format( "-> http://[::1]:%d", port ) );
    });

}}