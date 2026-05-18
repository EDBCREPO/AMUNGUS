#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace controller {

    node_t menu_find_room() { return node_t([=]( ptr_t<node_t> self ){

        auto addr = process::invoke([=]( any_t raw ){ 
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case  0: do { self->get_root()->append_child( "modal", modal_search_room() ); } while(0); break;
            case  1: do { self->get_root()->append_child( "modal", modal_create_room() ); } while(0); break;
            default: do { 

                auto raw = EM_EVAL( _STRING_( 
                    return document.querySelector("[room-list] div:nth-child(1)").getAttribute("room"); 
                ),  value.as<int>() - 1 ).as<EM_STRING>();

                self->get_root()->set_attribute( "room_id", string_t(raw) );
                EM_EVAL( _STRING_(
                    document.querySelector( "[rg-button]:nth-of-type(1)" ).click();
                ));
        
            } while(0); break;

        } return 1; });

        self->onClose([=](){ process::revoke( addr ); });

        fs::read_file( "assets/html/menu/findRoom.html" )
        .then([=]( string_t data ){

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[menu-modal]' ).innerHTML="";
                document.querySelector( '[menu-modal]' ).append( ol );
            ),  encoder::base64::atob ( processed ) );

        }).fail([=]( except_t err ){
            
            console::log( err.what() );

        }).finally([=](){ self->onNext.add( coroutine::add( COROUTINE(){
        coBegin do {

            auto raw = EM_EVAL( _STRING_( 
            if   ( document.querySelector("[rg-room]").value.length==0 )
                 { document.querySelector("[rg-room]").value= "US"; }
            return document.querySelector("[rg-room]").value;
            )).as<EM_STRING>();

            auto flg = ( *self->get_root()->get_attribute<object_t>( "flags" ) )[raw];
            auto dir = path::join( "/room/${0}/list", flg["code"].as<string_t>() );

            ws_fetch( nullptr, dir, nullptr )

            .then([=]( string_t message ){

                auto obj = json::parse( message ); 
                auto out = queue_t<string_t>   (); int c=2;

                for( auto x: obj.as<array_t<object_t>>() ){ try {
                out.push( regex::format( R"(
                    <div class="uk-width-1-1 uk-rounded uk-border uk-height-xsmall uk-padding-xsmall uk-inline 
                                uk-flex uk-flex-center uk-flex-column uk-button uk-button-light-hover"
                    room="${6}" map="${4}" onclick="${7}._nodepp_invoke_('${8}',${9});" >
                        <h2> ${0} ( ${6} ) ${10} </h2> 
                        <h4> ${1} connected | ${2} impostors | ${3} max </h4>
                        <div class="uk-badge uk-badge-primary uk-position-bottom-right"
                    room-id="${4}" > ${5} </div> </div>
                )", 
                    x["data"]["user"].as<string_t>(), 
                    x["size"] /*--*/ .as<int>(),
                    x["data"]["imp"] .as<int>(),
                    x["data"]["max"] .as<int>(),
                    x["data"]["mid"] .as<int>(),
                    x["data"]["map"] .as<string_t>(),
                    x["room"] /*--*/ .as<string_t>(),
                    NODEPP_MODULE_NAME, addr, c, flg["flag"].as<string_t>()
                )); c++; } catch( except_t ) { continue; } }
                
                EM_EVAL( _STRING_(
                    var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                    document.querySelector( '[room-list]' ).innerHTML=tm;
                ),  encoder::base64::atob( array_t<string_t>( out.data() ).join("") ));

            })

            .fail([=]( except_t err ){ EM_EVAL( _STRING_(
                document.querySelector( '[room-list]' ).innerHTML="";
            )); });
            
        } while(0); coDelay(10000); coGoto(0); coFinish
        })); });

    }); }

    /*─······································································─*/

    node_t menu_static_file( string_t file ) { return node_t([=]( ptr_t<node_t> self ){

        fs::read_file( file ).then([=]( string_t data ){
            
            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[menu-modal]' ).innerHTML="";
                document.querySelector( '[menu-modal]' ).append( ol );
            ),  encoder::base64::atob ( data ) );

        }).fail([=]( except_t err ){
            console::log( err.what() );
        }); self->free();

    }); }

}}

/*────────────────────────────────────────────────────────────────────────────*/