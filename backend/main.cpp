#include <nodepp/nodepp.h>
#include <express/http.h>
#include <apify/apify.h>
#include <nodepp/ws.h>

using namespace nodepp;

void onMain(){

    auto app = express::http::add();
    auto apf = apify::add<ws_t>();

    app.USE([=]( express_http_t cli, function_t<void> next ){
        cli.header( "Cross-Origin-Opener-Policy"  , "same-origin"  );
        cli.header( "Cross-Origin-Embedder-Policy", "require-corp" );
        console::log( cli.path, cli.get_fd() );
    next(); });

    apf.on( "/app/connect", [=]( apify_t<ws_t> cli ){

        cli.emit( "/app/connect", json::stringify( object_t({
            { "id", string::to_string( &cli ) }
        }) ));

    });

    apf.on( [=]( apify_t<ws_t> cli ){
        console::log( "404 error" );
    });

    app.USE( express::http::file( "www" ) );

    auto server = app.listen( "[::0]", 8000, [=]( socket_t cli ){
        console::log( "-> http://[::1]:8000" );
    });

    ws::server( server );

    server.onConnect([=]( ws_t cli ){

        cli.onData([=]( string_t data ){
            console::log( ">>", data );
            apf.next( cli, data );
        });

        cli.onClose([=](){
            console::log( "disconnected" );
        }); console::log( "connected" );

    });

}