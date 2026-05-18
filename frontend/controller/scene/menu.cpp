#pragma once 

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace controller { node_t menu_scene() {

    return node_t( [=]( ptr_t<node_t> self ){

        auto addr = process::invoke([=]( any_t raw ){
             self->get_root()->remove_child( "modal" );
        auto obj  = raw.as<EM_VAL>(); switch( obj.as<int>() ){

            case 0: do { self->get_root()->append_child( "modal", modal_quick_room   () ); } while(0); break;
            case 1: do { self->get_root()->append_child( "room" , menu_find_room     () ); } while(0); break;
            case 2: do { self->get_root()->append_child( "modal", modal_select_region() ); } while(0); break;
            
            case 3: do {
                 self->get_root()->append_child( "room" , menu_static_file( "assets/html/menu/soon.html" ) );
            } while(0); break; case 5: do {
                 self->get_root()->append_child( "room" , menu_static_file( "assets/html/menu/credit.html" ) );
            } while(0); break; case 4: do {
                 self->get_root()->append_child( "room" , menu_static_file( "assets/html/menu/howtoplay.html" ) );
            } while(0); break;

        }   return 1; });

        self->onClose([=](){ process::revoke( addr ); });

    do {

        fetch_t args; 
                args.method = "GET";
                args.url    = "http://localhost:8000/payload/mojon"; 

        http::fetch( args ).then([=]( http_t cli ){

            auto ep = json::parse ( cli.read() )["endp"].as<string_t>();
            controller::ws_endpoint() = ep;

        }).fail([=]( except_t err ){
           EM_EVAL( _STRING_( alert( "could connect to broker" ); ));
        });

    } while(0); do {
        
        auto raw = fs::read_file( "./assets/html/gameui/menu.html" ).await().value();
        auto emj = fs::read_file( "./assets/json/country.json"     ).await().value();
        auto obj = json::parse(emj); auto tmp = object_t();

        self->get_root()->set_attribute( "flags", obj );

        auto processed = regex::format( raw, NODEPP_MODULE_NAME, addr );
        auto region    = EM_EVAL( _STRING_(
            var ra = navigator.languages[0].split("-").slice(-1)[0].toUpperCase();
            var rb = navigator.languages[0].split("-").slice( 0)[0].toUpperCase();
            var tm = decodeURIComponent(escape(window.atob( '${0}' )));
            document.querySelector( '[menu-ui]' ).innerHTML = tm;
            return Object({ "ra": ra, "rb": rb });
        ),  encoder::base64::atob ( processed ) );

        if( obj.has( region["ra"].as<EM_STRING>() ) ){ tmp = obj[region["ra"].as<EM_STRING>()]; }
        if( obj.has( region["rb"].as<EM_STRING>() ) ){ tmp = obj[region["rb"].as<EM_STRING>()]; }

        EM_EVAL( _STRING_(
            
            var obj  = ${0}    ; var name = obj.name; 
            var flag = obj.flag; var code = obj.code;

            document.querySelector( '[rg-room]' ).value     = code;
            document.querySelector( '[rg-room]' ).innerHTML = `${flag} - ${name}`;

        ),  json::stringify( tmp ) );

    } while(0); do {

        auto raw = fs::read_file( "./assets/image/background.jpg" ).await().value();

        EM_EVAL( _STRING_(
            document.querySelector( "body" ).style.background/**/="url(data:image/jpg;base64,${0})";
            document.querySelector( "body" ).style.backgroundSize="230px";  
            document.querySelector("body").setAttribute( "mode", "menu" );          
            document.querySelector( "[rg-button]:nth-of-type(2)" ).click();
        ),  encoder::base64::atob ( raw ) );

    } while(0);

    });

}}}

/*────────────────────────────────────────────────────────────────────────────*/