#pragma once 

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace controller { node_t main_scene( int mode=0 ) {

    node_t tmp; switch( mode ) {
    case 0: do { tmp = controller::menu_scene(); } while(0); break;
    case 1: do { tmp = controller::game_scene(); } while(0); break; }

    auto addr = process::invoke([=]( any_t raw ){
    auto val  = raw.as<EM_VAL>().as<int>();
         tmp.free(); main_scene(val);
    return 1; });

    auto data = regex::format( _STRING_(
        <button onclick="${0}._nodepp_invoke_('${1}',0);" ct-button ></button>
        <button onclick="${0}._nodepp_invoke_('${1}',1);" ct-button ></button>
    ),  NODEPP_MODULE_NAME, addr );

    EM_EVAL( _STRING_(
        var tm = decodeURIComponent(escape(window.atob( '${0}' )));
        document.querySelector("[control-ui]").innerHTML = tm;
    ),  encoder::base64::atob( data ) );

    tmp.onClose([=](){ process::revoke(addr); });
    return tmp;

}}}

/*────────────────────────────────────────────────────────────────────────────*/