#pragma once

namespace ungine { namespace controller { 
    
    node_t modal_quick_room() { return node_t([=]( ptr_t<node_t> self ){

        do{ auto data = fs::read_file( "./assets/html/modal/quick_room.html" ).await().value();

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                document.querySelector( '[room-modal]' ).innerHTML=tm;
            ),  encoder::base64::atob( data ) );

            self->onClose([=](){ EM_EVAL( _STRING_(
                document.querySelector( '[room-modal]' )
                .innerHTML='<input type="radio" hidden modal-close checked >';
            )); /* process::revoke( addr ); */ });

        } while(0);

        auto raw = self->get_root()->get_attribute<string_t>( "room_id" );
        if ( raw.null() ){ try {

            auto reg = EM_EVAL( _STRING_( 
            if   ( document.querySelector("[rg-room]").value.length==0 )
                 { document.querySelector("[rg-room]").value= "US"; }
            return document.querySelector("[rg-room]").value;
            )).as<EM_STRING>();

            auto dir = path::join( "/room/${0}/list", reg );
            auto out = ws_fetch( nullptr, dir, nullptr ).await();

            if( !out.has_value() ){ 
            EM_EVAL( _STRING_( document.querySelector("[room-mode]").setAttribute( "room-mode", "1" ) ) );
            timer::timeout([=](){ self->free(); },3000); return; }
            
            auto obj = json::parse( out.value() )[0];

            self->get_root()->set_attribute( "room_id", obj["room"].as<string_t>() );
            EM_EVAL( _STRING_( setTimeout (()=>{
                document.querySelector( "[rg-button]:nth-of-type(1)" ).click();
            }, 1 ); ));

        } catch( except_t ) {} return; }
        
        console::log( ">>", *raw );
        auto dir = path::join( "/room", "${0}", "connect", *raw );
                        
        ws_fetch( nullptr, dir, "hello world" )

        .then([=]( string_t message ){
            console::log( message );
        })

        .fail([=]( except_t err ){
            console::log( err.what() );
        });

        self->get_root()->remove_attribute( "room_id" );

    }); }
    
    /*─······································································─*/

    node_t modal_search_room() { return node_t([=]( ptr_t<node_t> self ){

        auto addr = process::invoke([=]( any_t ){
        auto raw = EM_EVAL( _STRING_( 
            return document.querySelector("input[room-code]").value; 
        ))  .as<EM_STRING>();

            self->get_root()->set_attribute( "room_id", string_t(raw) );
            EM_EVAL( _STRING_(
                document.querySelector( "[rg-button]:nth-of-type(1)" ).click();
            ));

        return 1; });

        self->onClose([=](){ EM_EVAL( _STRING_(
            document.querySelector( '[room-modal]' )
            .innerHTML='<input type="radio" hidden modal-close checked >';
        )); process::revoke( addr ); });

        fs::read_file( "./assets/html/modal/search_room.html" )
        .then([=]( string_t data ){

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                document.querySelector( '[room-modal]' ).innerHTML=tm;
            ),  encoder::base64::atob( processed ) );

        }).fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t modal_select_region() { return node_t([=]( ptr_t<node_t> self ){

        auto data = fs::read_file( "./assets/html/modal/region_room.html" ).await().value();
        auto flag = *self->get_root()->get_attribute<object_t>( "flags" );
        auto out  = queue_t<string_t>();

        auto addr = process::invoke([=]( any_t raw ){
        auto val  = raw.as<EM_VAL>().as<EM_STRING>();
            
            EM_EVAL( _STRING_(
                var obj  = ${0}    ; var name = obj.name; 
                var flag = obj.flag; var code = obj.code;
                document.querySelector( '[rg-room]' ).value     = code;
                document.querySelector( '[rg-room]' ).innerHTML = `${flag} - ${name}`;
                document.querySelector( "[rg-button]:nth-of-type(2)" ).click();
            ),  json::stringify( flag[val] ) );

        return 1; });

        for( auto x: flag.keys() ){ 
             auto obj  = flag[x];
             auto emoj = obj ["flag"].as<string_t>();
             auto code = obj ["code"].as<string_t>(); 
             auto name = obj ["name"].as<string_t>();    
        out.push( regex::format( R"(
            <button class="uk-border uk-rounded uk-border-light uk-background-none 
                           uk-h1 uk-tooltip" tooltip="${4}-(${5}) "
                    onclick=" ${1}._nodepp_invoke_('${2}','${3}') "
            > ${0} </button>
        )", emoj, NODEPP_MODULE_NAME, addr, code, name, x ) ); }

        auto processed = regex::format( data, array_t<string_t>( out.data() ).join("\n") );

        self->onClose([=](){ EM_EVAL( _STRING_(
            document.querySelector( '[room-modal]' )
           .innerHTML='<input type="radio" hidden modal-close checked >';
        )); process::revoke( addr ); });

        EM_EVAL( _STRING_(
            var tm = decodeURIComponent(escape(window.atob( '${0}' )));
            document.querySelector( '[room-modal]' ).innerHTML=tm;
        ),  encoder::base64::atob ( processed ) );

    }); }

    /*─······································································─*/

    node_t modal_create_room(){ return node_t([=]( ptr_t<node_t> self ){
        
        auto addr = process::invoke([=]( any_t ){

            auto raw = EM_EVAL( _STRING_( 
            if   ( document.querySelector("[rg-room]").value.length==0 )
                 { document.querySelector("[rg-room]").value= "US"; }

            var mid = Number(document.querySelector( "input[name='map-selector']:checked" ).value);
            var map_name = ( val )=>{ switch(val) {
                case 0: return "space station"; break;
                case 1: return "mars colony";   break;
                case 2: return "aeroship";      break;
                case 3: return "frostpoint";    break;
            }   /*---*/ return null; };

            return Object({
                "reg": document.querySelector( "[rg-room]" ).value,
                "imp": 2, "max": 10, "map": map_name( mid ),
                "mid": mid,
            });

            )).as<EM_VAL>();

            auto dir = path::join( "/room/${0}/create", raw["reg"].as<EM_STRING>() );
            auto obj = object_t({ 
                { "map" , string_t(raw["map"].as<EM_STRING>()) },
                { "mid" , raw["mid"].as<int>() },
                { "imp" , raw["imp"].as<int>() }, 
                { "max" , raw["max"].as<int>() }, { "user", "007" }
            });

            ws_fetch( nullptr, dir, json::stringify(obj) )

            .then([=]( string_t message ){
                console::log( "->", message );
            })

            .fail([=]( except_t err ){
                console::log( "err:", err.what() );
            });

            self->free();

        return 1; });

        self->onClose([=](){ EM_EVAL( _STRING_(
            document.querySelector( '[room-modal]' )
           .innerHTML='<input type="radio" hidden modal-close checked >';
        )); process::revoke( addr ); });

        auto data = fs::read_file( "./assets/html/modal/create_room.html" ).await().value();
        auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );

        EM_EVAL( _STRING_(
            var tm = decodeURIComponent(escape(window.atob( '${0}' )));
            document.querySelector( '[room-modal]' ).innerHTML=tm;
        ),  encoder::base64::atob ( processed ) );

    }); }

}}
