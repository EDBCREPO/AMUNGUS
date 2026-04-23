#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace ungine { namespace controller {

//  minigame_chat
//  minigame_ad
//  minigame_alert
//  minigame_ad
//  minigame_map
//  minigame_custom
//  minigame_scann
//  minigame_vent

    node_t minigame_tmp() { return node_t([=]( ptr_t<node_t> self ){

        auto tmp = string_t(10UL);
        auto uif = fs::read_file( "assets/minigame/navigation.html" );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 2: do { 
            EM_EVAL( _STRING_( setTimeout(()=>{
                   document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
            },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
            ), tmp.get() ); } while(0); break;
            
        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        uif.then([=]( string_t data ){

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob ( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });


    }); }

    /*─······································································─*/

    node_t minigame_camera() { return node::node_3D([=]( ptr_t<node_t> self ){

        auto raw = fs::read_file( "assets/object/camera.json" ).await();
        if( !raw ){ throw except_t( "invalid json file" ); }
        auto obj = json::parse( raw.value() );
        auto vpt = self->get_root_viewport();
        auto len = obj["cam"].as<array_t<object_t>>().size();
        auto flg = matrix::get_pixel_float( vpt->matrix, 11, 0, 0 );

        for( auto &x: obj["cam"].as<array_t<object_t>>() ){
            vec3_t pos = vec3_t({
                x["pos"][0].as<float>(),
                x["pos"][1].as<float>(),
                x["pos"][2].as<float>()
            }); vec3_t rot = vec3_t({
                x["rot"][0].as<float>(),
                x["rot"][1].as<float>(),
                x["rot"][2].as<float>()
            }); x["pos"] = pos; x["rot"] = rot;
        }

        auto onName = event_t<int>(); onName.on([=]( int cam ){ EM_EVAL( _STRING_(
             document.querySelector( "[cam]" ).innerHTML = 'cam ${0}';
        ), cam ); });

        auto tmp = string_t( 11UL, 0x00 ); 
        auto cam = self->get_root_viewport()->camera3D;
        auto uif = fs::read_file( "assets/minigame/camera.html" );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 0: /*---*/ do { self->free(); } while(0); break;
            case 1: case 2: do {
            if( value.as<int>()==1 ) { tmp[11]--; } else { tmp[11]++; }
            if( tmp[11]<0 ){ tmp[11]=len-1; } elif ( tmp[11]>=len ) { tmp[11]=0; }
                auto pos = self->get_child("cam3D")->get_attribute<transform_3D_t>("transform");
                     pos->rotation = obj["cam"][tmp[11]]["rot"].as<vec3_t>(); onName.emit(tmp[11]);
                     pos->position = obj["cam"][tmp[11]]["pos"].as<vec3_t>()* vec3_t({3,3,3}); 
            } while(0); break;
            
        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); self->get_root_viewport()->camera3D = cam;
            matrix::set_pixel_float( vpt->matrix, 11, 0, 0, flg );
        });

        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );
        matrix::set_pixel_float( vpt->matrix, 11, 0, 0, 1.0f );

        uif.then([=]( string_t data ){

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob ( processed ) );

            self->append_child( "cam3D", node::node_camera_3D([=]( ptr_t<node_t> self ){

                auto pos = self->get_attribute<transform_3D_t>( "transform" );
                auto cam = self->get_attribute<camera_3D_t>   ( "camera" );
                auto stm = ptr_t<float>( 0UL );

                pos->position = obj["cam"][0]["pos"].as<vec3_t>()* vec3_t({3,3,3}); 
                pos->rotation = obj["cam"][0]["rot"].as<vec3_t>(); onName.emit(0);
                self->get_root_viewport()->camera3D = cam;

            }));

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_card() { return node_t([=]( ptr_t<node_t> self ){

        auto uif = fs::read_file( "assets/minigame/admin.html" );
        auto mpos= self->get_root()->get_attribute<vec3_t>("mousePosition");
        auto tmp = string_t(10UL); auto val = ptr_t<int>( 0UL, rand() % 17 * 100 + 300 );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 2: do { 
            EM_EVAL( _STRING_( setTimeout(()=>{
                   document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
            },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
            ), tmp.get() ); } while(0); break;

        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );
        auto co = coroutine_t( COROUTINE(){
        coBegin

        while(true){
            
            while( type::cast<int>(mpos->z) == 0 ){ coDelay(100); } EM_EVAL( _STRING_(
                var crd = document.querySelector("[_${0}_pow] [card]").getBoundingClientRect();
                var crs = ${1};

                if( crs.x >= crd.left && crs.x <= crd.right  &&
                    crs.y >= crd.top  && crs.y <= crd.bottom ){ 
                    window._card_offset_x = crs.x - crd.left ;
                    window._card_stamp    = Date.now();
                return true; } return false;

            ), addr, json::stringify(object_t({ {"x",mpos->x}, {"y",mpos->y} })) );

            while( type::cast<int>(mpos->z)!=0 ){ do { auto val = EM_EVAL( _STRING_(
            if   ( !window._card_offset_x )/*-*/{ return false; }
            
                var box = document.querySelector("[_${0}_pow]").getBoundingClientRect();
                var crs = ${1};
                    
                var newX= ( crs.x - window._card_offset_x ) - box.left;
                var card= document.querySelector("[_${0}_pow] [card]");

                if( newX < 0 ){ card.style.left = "0%"; return false; }

                card.style.left   = newX +"px";
                card.style.cursor = "grabbing";

                if( newX>240 ){ return true; } return false;
            ),  addr, json::stringify(object_t({ {"x",mpos->x}, {"y",mpos->y} })) ); 
            
            if( val.as<bool>() ){ coSet(3); return 1; }
            } while(0); coNext; }

        coYield(2);

            EM_EVAL( _STRING_(
                window._card_offset_x = null; window._card_stamp = null;
                document.querySelector("[_${0}_pow] [card]").style.left   = "0%";
                document.querySelector("[_${0}_pow] [card]").style.cursor = "grab";
            ), addr );

        continue; coYield(3);

            EM_EVAL( _STRING_(
                var dif = Date.now() - window._card_stamp;

                if( dif > ${1} + 100 ){
                    document.querySelector("[_${0}_pow]").setAttribute( "mode", "lock" );
                    document.querySelector("[_${0}_pow] [LCD]").innerHTML = "Too Slow";
                return; }

                if( dif < ${1} - 100 ){
                    document.querySelector("[_${0}_pow]").setAttribute( "mode", "lock" );
                    document.querySelector("[_${0}_pow] [LCD]").innerHTML = "Too Fast";
                return; }

                document.querySelector("[_${0}_pow]").setAttribute( "mode", "done" );
                document.querySelector("[_${0}_pow] [LCD]").innerHTML = "Perfect";

            ), addr, *val );

        coGoto(2); } coFinish });

        uif.then([=]( string_t data ){

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob ( processed ) );

            self->onNext([=](){ co.emit(); });

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });


    }); }

    /*─······································································─*/

    node_t minigame_password() { return node_t([=]( ptr_t<node_t> self ){

        auto tmp = string_t(10UL); auto que = queue_t<int>();
        auto uif = fs::read_file( "assets/minigame/password.html" );
        auto pss = array_t<int>(6UL); for( auto &x: pss ){ x=rand()%10; }

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){
            
            case 0: case 1: case 2: case 3: case 4: case 13:
            case 5: case 6: case 7: case 8: case 9: do {
                que.push( value.as<int>() ); 
                if( que.size() > 6 ) /*-*/ { que.shift(); }
                if( value.as<int>() == 13 ){ que.clear(); }
            EM_EVAL ( _STRING_( 
                if( "${1}" == "${0}" ){
                    document.querySelector("[_${2}_pow]").setAttribute("mode","done"); 
                } else {
                    document.querySelector("[_${2}_pow]").setAttribute("mode","lock"); 
                }   document.querySelector("[LCD]").innerHTML = "${0}";
            ),  array_t<int>( que.data() ).join(""), pss.join(""), tmp.get() ); 
            } while(0); break;

            case 11: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 10: do { 
                self->free(); 
            } while(0); break;
            
            case 12: do { EM_EVAL( _STRING_( setTimeout(()=>{
                   document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
            },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
            ), tmp.get() ); } while(0); break;
            
        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        uif.then([=]( string_t data ){

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, pss.join("") );
            
            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob ( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_wiring() { return node_t([=]( ptr_t<node_t> self ){

        auto mpos= self->get_root()->get_attribute<vec3_t>("mousePosition");
        auto tmp = string_t(10UL); auto spos = ptr_t<vec4_t>(0UL);
        auto uif = fs::read_file( "assets/minigame/wiring.html" );
        auto val = ptr_t<int>( 11UL, 0x00 );

        for( int x=2; x-->0; ){
             array_t<int> tmp ({ 0, 1, 2, 3, 4 });
             /*--*/ tmp = tmp.sort([=]( int, int ){ return rand()%2==0; });
             type::move ( tmp.begin(), tmp.end(), val.begin() + 5UL * x );
        }

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 0x00: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0x07: do { 
                self->free(); 
            } while(0); break;

            case 0x06: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

            case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: do {

                auto raw = value.as<int>(); auto has = EM_EVAL( _STRING_( 
                    return document.querySelector("[_${0}_pow] c${1}")==null; 
                ),  tmp.get(), val[val[10]] ).as<bool>(); if(!has) { break; }

                if( val[10] != 0x00 ){ EM_EVAL( _STRING_(
                var el = document.querySelector( "[_${0}_pow] svg line:nth-child(${1})" );
                if(!el){ return; }
                    el.setAttribute( "x2", el.getAttribute( "x1" ) );
                    el.setAttribute( "y2", el.getAttribute( "y1" ) );
                ),  tmp.get(), val[10] ); } val[10] = value.as<int>();

                EM_EVAL( _STRING_(
                    document.querySelector("[_${0}_pow]").removeAttribute("c${1}");
                ),  tmp.get(), val[val[10]-1] );
                    
            } while(0); break;


            case 0x10: case 0x20: case 0x30: case 0x40: case 0x50: do {

                auto raw = value.as<int>() >> 4;
                if( val[val[10]-1] != val[raw+4] ){ EM_EVAL( _STRING_(
                var el = document.querySelector( "[_${0}_pow] svg line:nth-child(${1})" );
                if(!el){ return; }
                    el.setAttribute( "x2", el.getAttribute( "x1" ) );
                    el.setAttribute( "y2", el.getAttribute( "y1" ) );
                ),  tmp.get(), val[10] ); val[10] = 0x00; break; }

                EM_EVAL( _STRING_(
                    document.querySelector("[_${0}_pow]").setAttribute( "c${1}", "" );
                ),  tmp.get(), val[raw+4] ); val[10] = 0x00;

            } while(0); break;

        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );
        auto co = coroutine_t( COROUTINE(){
        coBegin

            while( true ){ do {

                auto tmp = EM_EVAL( _STRING_(
                    return document.querySelector( "[_${0}_pow] svg" ).getBoundingClientRect();
                ),  addr );

                spos->x = tmp["x"     ].as<float>();
                spos->y = tmp["y"     ].as<float>();
                spos->z = tmp["width" ].as<float>() + spos->x;
                spos->w = tmp["height"].as<float>() + spos->y;

                if( mpos->x < spos->x || mpos->x > spos->z ||
                    mpos->y < spos->y || mpos->y > spos->w 
                ) { EM_EVAL( _STRING_(
                    var el = document.querySelector( "[_${0}_pow] svg line:nth-child(${1})" );
                    if(!el){ return; }
                        el.setAttribute( "x2", el.getAttribute( "x1" ) );
                        el.setAttribute( "y2", el.getAttribute( "y1" ) );
                ), addr, val[10] ); break; }

                vec2_t dpos ({ mpos->x - spos->x, mpos->y - spos->y });

                EM_EVAL( _STRING_(
                    var el = document.querySelector( "[_${0}_pow] svg line:nth-child(${1})" );
                    if(!el){ return; }
                        el.setAttribute( "x2", ${2} );
                        el.setAttribute( "y2", ${3} );
                ), addr, val[10], dpos.x, dpos.y );

            } while(0); coDelay(30); }

        coFinish });

        uif.then([=]( string_t data ){

            auto button = string_t(); for( auto x=0UL; x<5UL; x++ ){
                button+= regex::format( _STRING_(
                    [_${0}_] svg          line:nth-child(${1})  { stroke    : ${2}; }
                    [_${0}_] [left] .uk-button:nth-child(${1})  { background: ${2} !important; }
                ), addr, x+1, val[x]==0 ? "red"    : val[x]==1 ? "green" : 
                              val[x]==2 ? "blue"   : 
                              val[x]==3 ? "yellow" : "purple" );
            } for( auto y=5UL; y<val.size(); y++ ) { int x = y - 5UL;
                button+= regex::format( _STRING_(
                    [_${0}_] [right] .uk-button:nth-child(${1}) { background: ${2} !important; }
                ), addr, x+1, val[y]==0 ? "red"    : val[y]==1 ? "green" : 
                              val[y]==2 ? "blue"   : 
                              val[y]==3 ? "yellow" : "purple" );
            }

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button );
            
            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob ( processed ) );

            self->onNext.add([=](){ return co.emit(); });

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_reactor() { return node_t([=]( ptr_t<node_t> self ){

        auto tmp = string_t(10UL);
        auto uif = fs::read_file( "assets/minigame/reactor.html" );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        uif.then([=]( string_t data ){

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );
            
            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_gas() { return node_t([=]( ptr_t<node_t> self ){

        auto tmp = string_t(10UL); auto done = ptr_t<bool>(0UL,false);
        auto uif = fs::read_file( "assets/minigame/gasoline.html" );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 3: do { if( *done ){ break; } *done =! *done; EM_EVAL( _STRING_(
                var el = document.querySelector( "[_${0}_pow] .gasoline-progress" );
                var md = el.getAttribute( "mode" ) == "filled";
                         el.setAttribute( "mode", md ? "drain" : "fill" );
            ), tmp.get() ); self->onNext.add( coroutine::add( COROUTINE(){
            coBegin

                coDelay( 30000 ); EM_EVAL( _STRING_(
                    var el = document.querySelector( "[_${0}_pow] .gasoline-progress" );
                    var md = el.getAttribute( "mode" ) == "fill";
                             el.setAttribute( "mode", md ? "filled" : "drained" );
                    document.querySelector("[_${0}_pow]").setAttribute("mode","done");
                ), tmp.get() );

            coFinish
            })); } while(0); break;

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        uif.then([=]( string_t data ){
            
            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );
            
            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_simon() { return node_t([=]( ptr_t<node_t> self ){

        auto pss = string_t( 9UL); for( int x=9; x-->0; ){ pss[x] = rand()%9 + '0'; }
        auto tmp = string_t(10UL); auto enb = ptr_t<char>( 5UL, 0x00 );
        auto uif = fs::read_file( "assets/minigame/simon.html" );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 0: case 1: case 2: case 3: case 4: case 5:
            case 6: case 7: case 8: case 9: do {

                if( enb[4] == 0x00 ) /*--------*/ { break; }
                if( ( pss[enb[0]]-'0' ) == value.as<int>() )
                  { enb[0]++; enb[1]++; } else {
                    enb[0] =  enb[3]*3; enb[1]=-1;
                    enb[2] =  enb[3]*3; }

            } while(0); break;

            case 11: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 10: do { 
                self->free(); 
            } while(0); break;

            case 12: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );
        auto co = coroutine_t( COROUTINE(){
        coBegin
    
            coGoto(2); coYield(1); do { enb[4] = 0x00;

                EM_EVAL( _STRING_( document.querySelector('[_${0}_pow]').setAttribute( "state", "error"  ); ), addr );
                coDelay( 1000 );

                EM_EVAL( _STRING_( document.querySelector('[_${0}_pow]').setAttribute( "state", "idle" ); ), addr );
                coDelay( 1000 );

            } while(0); coYield(2); do { enb[1] = 0x00; enb[4] = 0x00;

                EM_EVAL( _STRING_( document.querySelector('[_${0}_pow]').setAttribute( "state", "idle" ); ), addr );
                coDelay( 1000 );

                while( enb[1] < 3 ){

                    EM_EVAL( _STRING_( 
                        document.querySelectorAll('[_${0}_pow] button')[${1}]
                                .setAttribute( "state", "on"  ); 
                    ), addr, pss[ enb[2] ] ); coDelay( 1000 ); 
                    
                    EM_EVAL( _STRING_( 
                        document.querySelectorAll('[_${0}_pow] button')[${1}]
                                .setAttribute( "state", "off" ); 
                    ), addr, pss[ enb[2] ] ); coDelay( 1000 ); 
                    
                enb[1]++; enb[2]++; }

                EM_EVAL( _STRING_( 
                    document.querySelector('[_${0}_pow]')
                            .setAttribute( "state", "user" ); 
                ), addr ); coDelay(300);

                EM_EVAL( _STRING_( 
                    document.querySelector('[_${0}_pow]')
                            .setAttribute( "state", "normal" ); 
                ), addr ); coDelay(300);

                enb[1] = 0; enb[4] = 0xff;

                while( enb[1] != -1 && enb[1] < 3 ){ coDelay(100); }
                if   ( enb[1] == -1 ) /*--------*/ { coGoto (1);   }

                enb[1] = 0; enb[4] = 0x00;

                EM_EVAL( _STRING_( 
                    document.querySelector('[_${0}_pow]')
                            .setAttribute( "state", "next" ); 
                ), addr ); coDelay( 1000 );

                EM_EVAL( _STRING_( 
                    document.querySelector('[_${0}_pow]')
                            .setAttribute( "state", "idle" ); 
                ), addr ); coDelay( 1000 );

            } while( enb[3]++<2 );

            EM_EVAL( _STRING_( 
                document.querySelector('[_${0}_pow]')
                        .setAttribute( "mode", "done" ); 
            ), addr ); coDelay( 1000 );

        coFinish
        });

        uif.then([=]( string_t data ){

            auto button  = string_t(); for( int x=0; x<9; x++ ){
                 button += regex::format( _STRING_( 
                    <button class  ="uk-button uk-button-light-outline" state="off"
                            onclick="${0}._nodepp_invoke_( '${1}', ${2} )"
                    ></button> 
                 ), NODEPP_MODULE_NAME, addr, x );
            } 

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button );
            
            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

            self->onLoop.add([=]( float ){ return co.emit(); });

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_nav( array_t<string_t> emj ) { return node_t([=]( ptr_t<node_t> self ){

        auto val = ptr_t<int>({ rand()%emj.size(), rand()%emj.size() });
        auto uif = fs::read_file( "assets/minigame/navigation.html" );
        auto tmp = string_t(10UL);

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 3: do { auto old = val[0]; val[0]++; if( val[0]>emj.size()-1 ){ val[0]=0; }
                EM_EVAL( _STRING_( 
                    document.querySelectorAll( "[_${0}_] h1" )[${1}].style.display = "none";
                    document.querySelectorAll( "[_${0}_] h1" )[${2}].style.display = "flex";
                if( ${3} == ${2} ){
                    document.querySelector( "[_${0}_pow]" ).setAttribute( "state", "done" );
                } else {
                    document.querySelector( "[_${0}_pow]" ).setAttribute( "state", "lock" );
                }), tmp.get(), old, val[0], val[1] );
            } while(0); break;

            case 4: do { auto old = val[0]; val[0]--; if( val[0]<0 ){ val[0]=emj.size()-1; }
                EM_EVAL( _STRING_( 
                    document.querySelectorAll( "[_${0}_] h1" )[${1}].style.display = "none";
                    document.querySelectorAll( "[_${0}_] h1" )[${2}].style.display = "flex";
                if( ${3} == ${2} ){
                    document.querySelector( "[_${0}_pow]" ).setAttribute( "state", "done" );
                } else {
                    document.querySelector( "[_${0}_pow]" ).setAttribute( "state", "lock" );
                }), tmp.get(), old, val[0], val[1] );
            } while(0); break;

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        uif.then([=]( string_t data ){

            auto button = string_t(); for( int x=0; x<emj.size(); x++ ){
                 button+= regex::format( _STRING_(
                    <h1 style="font-size: 200px !important; display:${1}; justify-content: center;"> ${0} </h1>
                 ), emj[x], x==val[0] ? "flex" : "none" ); 
            }

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button, emj[val[1]] );
            
            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_electricity() { return node_t([=]( ptr_t<node_t> self ){

        auto uif = fs::read_file( "assets/minigame/switch.html" );
        auto tmp = string_t( 10UL );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log ( "removed" );
        });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        uif.then([=]( string_t data ){

            auto ord    = array_t<bool>({ 0, 1, 1, 0, 0, 0, 0, 0, 1, 0 })
                .sort([=]( int a, int b ){ return rand()%2==0; });
            auto button = string_t();

            for( int x=10; x-->0; ){ button += regex::format( _STRING_(
                <label  class="uk-width-1-2 uk-child-width-1-1 uk-padding-2xsmall"> 
                    <div   class="uk-button uk-button-danger"></div>
                    <input type ="checkbox" ${0} hidden> 
                </label>
            ), ord[x] ? "checked" : "" ); }

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_radio() { return node_t([=]( ptr_t<node_t> self ){

        auto scl = 10. / 100.;
        auto val = ptr_t<int>({ rand()%97+3, rand()%97+3 });
        auto tmp = string_t( 10UL ); srand( process::now() );
        auto uif = fs::read_file( "assets/minigame/signal.html" );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

        } return 1; });

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log ( "removed" );
        });

        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );
        auto co = coroutine_t( COROUTINE(){
        coBegin

            while(true){ do {

                val[1] = EM_EVAL( _STRING_(
                    return Number( document.querySelector( '[_${0}_pow] .val' ).value );
                ),  addr ).as<int>();

                EM_EVAL( _STRING_(
                    document.querySelector( '[_${0}_pow] .hz2' ).style.transform="scaleX(${1}) scaleY(2)";
                ),  addr, val[1] * scl );

                if( abs(val[1] - val[0]) <= 1 ){ EM_EVAL( _STRING_(
                    document.querySelector( '[_${0}_pow]' ).setAttribute("val","done");
                ),  addr ); } else { EM_EVAL( _STRING_(
                    document.querySelector( '[_${0}_pow]' ).setAttribute("val","lock");
                ),  addr ); }

            } while(0); coDelay(100); }

        coFinish
        });

        uif.then([=]( string_t data ){
            
            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr );

            EM_EVAL( _STRING_(
                
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );

                document.querySelector( '[_${1}_pow] .hz1' ).style="transform: scaleX(${3}) scaleY(2); transform-origin: left";
                document.querySelector( '[_${1}_pow] .hz2' ).style="transform: scaleX(${4}) scaleY(2); transform-origin: left";
                document.querySelector( '[_${1}_pow] .val' ).value = '${2}';

            ),  encoder::base64::atob( processed ), addr, val[1], val[0] * scl , val[1] * scl );

            self->onLoop([=]( float delta ){ co.emit(); });

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_generic( ptr_t<string_t> emojis ) { return node_t([=]( ptr_t<node_t> self ){

        auto list= queue_t<ptr_t<float>>(); auto tmp = string_t( 10UL );
        auto uif = fs::read_file( "assets/minigame/generic.html" );

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

        } return 1; });

        for( int x=10; x-->0; ){ 
            float angle = type::cast<float>( rand()%360 ) * DEG2RAD;
            ptr_t<float> raw ( 5UL );
            raw[0] = type::cast<float>( rand()%90  );
            raw[1] = type::cast<float>( rand()%90  );
            raw[2] = type::cast<float>( sin(angle) );
            raw[3] = type::cast<float>( cos(angle) );
            raw[4] = type::cast<float>( rand()%20  )+ 30.;
        list.push(raw); } 
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log ( "removed" );
        });

        auto co = coroutine::add( COROUTINE(){
        coBegin

            while( true ){ do { int w=0; auto x=list.first(); while( x!=nullptr ){

                if  ( x->data[0] < 0 ) /*---------*/ { x->data[2] = 1.0f; }
                elif( x->data[0] >90 ) /*---------*/ { x->data[2] =-1.0f; }
                if  ( x->data[1] < 0 ) /*---------*/ { x->data[3] = 1.0f; }
                elif( x->data[1] >90 ) /*---------*/ { x->data[3] =-1.0f; }

                x->data[0] += x->data[2] * engine::get_delta() * x->data[4];
                x->data[1] += x->data[3] * engine::get_delta() * x->data[4];

                EM_EVAL(_STRING_(
                    var x = document.querySelectorAll( "[_${0}_pow] [asteroid]" )[${1}];
                        x.style.left="${2}%"; x.style.top="${3}%";
                ), addr, w, x->data[0], x->data[1] );

            w++; x=x->next; }} while(0); coNext; }

        coFinish
        });

        uif.then([=]( string_t data ){

            auto button = string_t(); for( uint x=10; x-->0; ){
                button += regex::format( _STRING_(
                    <label style="position:absolute;" asteroid> ${0}
                    <input type="radio" hidden ></label>
                ),  emojis[ rand() % emojis.size() ] );
            }

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

            self->onLoop([=]( float delta ){ co.emit(); });

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_motor() { return node_t([=]( ptr_t<node_t> self ){

        auto uif = fs::read_file( "assets/minigame/temperature.html" );
        auto tmp = string_t( 10UL ); ptr_t<uint> val ({ rand()%100, rand()%100 });

        auto addr = process::invoke([=]( any_t raw ){
        auto value= raw.as<EM_VAL>(); switch( value.as<int>() ){

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 3: do { 
            bool done = false;
                if( val[0] == 0 ){ val[0] = 100; break; } val[0] -= 1; 
                if( val[0] == val[1] )   { done = true; } EM_EVAL( _STRING_( 
                    document.querySelector( "[_${0}_pow] h1[VALUE]" ).innerHTML = "${1}°";
                    document.querySelector( "[_${0}_pow]" ).setAttribute( "val", ${2} ? "done" : "lock" );
                ), tmp.get(), val[0], done );

            } while(0); break;

            case 4: do { 
            bool done = false;
                if( val[0] >= 100 ){ val[0] = 0; break; } val[0] += 1; 
                if( val[0] == val[1] )   { done = true; } EM_EVAL( _STRING_( 
                    document.querySelector( "[_${0}_pow] h1[VALUE]" ).innerHTML = "${1}°";
                    document.querySelector( "[_${0}_pow]" ).setAttribute( "val", ${2} ? "done" : "lock" );
                ), tmp.get(), val[0], done );
            } while(0); break;
            
        } return 1; });

        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log( "removed" );
        });

        uif.then([=]( string_t data ){ 

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, val[0], val[1] );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_clasification(){ return node_t([=]( ptr_t<node_t> self ){

        auto list= array_t<string_t>({
            "👓","🕶️","💎","🔒","🤠",
            "🛡️","🧭","🚀","🌟",
            "🎁","🎮","🎲","🧩",
            "🎰","🎃","🍔","🐵",
            "🐱","🦝","🦊","🐺",
            "🦁","🐯","🐥","🐸",
            "🐼","🐨","🐻‍❄️","🐻",
            "🐷","🐮","💀","👻",
            "👽","🤖","💋","🤡","💩"
        });
        
        auto uif = fs::read_file( "assets/minigame/clasification.html" );
        auto tmp = string_t( 10UL );

        auto addr= process::invoke([=]( any_t raw ){
        auto val = raw.as<EM_VAL>(); switch( val.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

        } return 1; });

        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log( "removed" );
        });

        uif.then([=]( string_t data ){
            
            auto button = string_t(); ulong idx = rand() % list.size();
            auto rest   = queue_t<string_t>(); 

            for( int x=list.size(); x-->0; ){ 
            if ( x==idx ) /*---*/ { continue; }
                 rest.insert( rand()%2==0 ? nullptr : rest.first (), list[x]   ); }
            for( int x=5; x-->0; ){ rest.insert( rand()%rest.size(), list[idx] ); }

            rest.map([&]( string_t x ){
                button += regex::format( _STRING_(
                <label class="uk-button uk-button-light-outline" >
                <a>${0}</a><input type="checkbox" ${1} hidden />    
                </label>
                ), x, x==list[idx] ? "is" : "" );
            });

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button, list[idx] );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_circle(){ return node_t([=]( ptr_t<node_t> self ){

        auto uif = fs::read_file( "assets/minigame/circle.html" );
        auto tmp = string_t( 10UL );

        auto addr= process::invoke([=]( any_t raw ){
        auto val = raw.as<EM_VAL>(); switch( val.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;
            
            case 3: case 4: case 5: do {

                EM_VAL value = EM_EVAL( _STRING_( 

                    const el = document.querySelectorAll( "div>svg:first-child" )[${0}];
                    const tr = window.getComputedStyle( el ).transform;

                    if( tr !== "none" ){
                        const matrix = new DOMMatrix(tr);
                        const grados = Math.round( Math.atan2(matrix.b, matrix.a) * (180 / Math.PI) );
                    if( grados > 70 && grados < 110 ){
                        el.removeAttribute("rot${1}");
                        el.setAttribute   ("rot0","");
                    }}

                ), val.as<int>() - 3, val.as<int>() - 2 );

            } while(0); break;

            case 2: do { 
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;
        } return 1; });

        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log( "removed" );
        });

        uif.then([=]( string_t data ){ srand( process::now() );

            auto button = string_t(); int y = 0;

            for( int x=3; x-->0; ){ button += regex::format( _STRING_(
                <div class="uk-flex uk-child-width-auto uk-gap-2xsmall">
                            
                    <svg width="100" height="100" viewBox="0 0 100 100" rot${1} >

                        <circle
                            style="transform: rotate(-60deg); transform-origin: 50% 50%;"
                            cx="50" cy="50" r="40" fill="none"
                            stroke="var( --${0} )" 
                            stroke-dasharray="360" 
                            stroke-dashoffset="150" 
                            stroke-linecap="round" 
                            stroke-width="15" 
                        />

                        <circle cx="50" cy="10" r="7" fill="white" />
                        
                    </svg>
                                    
                    <svg width="100" height="100" viewBox="0 0 100 100"
                         style="transform: rotate(-90deg);">

                        <circle
                            style="transform: rotate(-60deg); transform-origin: 50% 50%;"
                            cx="50" cy="50" r="40" fill="none"
                            stroke="var( --${0} )" 
                            stroke-dasharray="360" 
                            stroke-dashoffset="150" 
                            stroke-linecap="round" 
                            stroke-width="15" 
                        />

                        <circle cx="50" cy="10" r="7" fill="white" />
                        
                    </svg>

                    <button onclick="${3}._nodepp_invoke_('${2}',${1}+2)" 
                            class="uk-button uk-button-light-outline"
                            style="--value: var( --${0} )">
                    <icon> lock </icon> <icon> lock_open </icon> </button>

                </div>
            ), y%3==0 ? "primary" : y%3==1 ? "warning" : "danger", y+1, addr, NODEPP_MODULE_NAME ); y++; }

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div');
                    el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/
    
    node_t minigame_data(){ return node_t([=]( ptr_t<node_t> self ){

        auto uif = fs::read_file( "assets/minigame/download.html" );
        auto tmp = string_t( 10UL ); auto time = 30; //rand()%10+10;

        auto addr= process::invoke([=]( any_t raw ){
        auto val = raw.as<EM_VAL>(); switch( val.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

        } return 1; });

        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );
        
        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log( "removed" );
        });

        self->onNext.add( coroutine::add( COROUTINE(){
        coBegin

            coDelay( time * 1000 ); EM_EVAL( _STRING_( 
                document.querySelector( '[_${1}_pow]' ).innerHTML = `
                    <input class  ="uk-button uk-button-success uk-h1 uk-width-1-1" 
                           onclick="${0}._nodepp_invoke_( '${1}', 1 )"
                    type="button" value="DONE" DONE >
                `;
            ), NODEPP_MODULE_NAME, addr );

        coFinish
        }));

        uif.then([=]( string_t data ){ srand( process::now() );

            auto tmp       = string_t( rand()%2==0 ? "Downloading": "Uploading" );
            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, tmp, time );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/
    
    node_t minigame_shield(){ return node_t([=]( ptr_t<node_t> self ){

        auto uif = fs::read_file( "assets/minigame/shield.html" );
        auto tmp = string_t( 10UL );

        auto addr= process::invoke([=]( any_t raw ){
        auto val = raw.as<EM_VAL>(); switch( val.as<int>() ){

            case 1: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 2: do {
                EM_EVAL( _STRING_( setTimeout(()=>{
                       document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                },10); document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                ), tmp.get() );
            } while(0); break;

        } return 1; });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );
        
        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log( "removed" );
        });

        uif.then([=]( string_t data ){ srand( process::now() );

            auto clr    = array_t<int> ({ 0, 1, 2, 1, 0, 1, 2, 1, 0 });
            auto ord    = array_t<bool>({ 0, 1, 1, 0, 0, 0, 0, 0, 1 })
                .sort([=]( int a, int b ){ return rand()%2==0; });
            auto button = string_t(); auto y=0;

            for( auto x: ord ){ 
                string_t color = clr[y]==0 ? "primary" : clr[y]==1 ? "warning" : "danger";
                button += regex::format( _STRING_(
                <label>
                    <a class="uk-button uk-button-light-outline uk-h1"
                       style="--value: var( --${1} )"
                    ></a><input type="radio" ${0} hidden>
                </label>
                ), x ? "checked" : "", color ); y++; 
            }

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });
        
    }); }

    /*─······································································─*/
    
    node_t minigame_number(){ return node_t([=]( ptr_t<node_t> self ){

        auto uif = fs::read_file( "assets/minigame/number.html" );
        auto pss = ptr_t<string_t>( 0UL ); string_t tmp (10UL);

        auto addr= process::invoke([=]( any_t raw ){
        auto val = raw.as<EM_VAL>(); switch( val.as<int>() ){

            case 1: case 2: case 3: case 4: case 5:
            case 6: case 7: case 8: case 9: do {
                auto tmp = string::to_string( val.as<int>() );
                if ( regex::test( *pss, tmp ) ){ break; }
            *pss += tmp; } while(0); break;

            case 11: do{ 
                self->get_root()->onSignal.emit( "tsk_done", nullptr );
            } while(0); case 0: do { 
                self->free(); 
            } while(0); break;

            case 10: do { 
                
                if( !pss->empty() && pss->size() == 9  && 
                    memcmp( pss->get(),"123456789", 9 )==0 
                ) {

                    console::log( "done" ); self->free(); break;
                    
                } else {

                    EM_EVAL( _STRING_( setTimeout(()=>{
                          document.querySelector( "[_${0}_]" ).setAttribute( "animation", "shake" );
                    },10);document.querySelector( "[_${0}_]" ).setAttribute( "animation", "" );
                    ), tmp.get() );

                }

            break; } while(0); break;

        } return 1; });
        
        memcpy( tmp.get(), addr.get(), min( tmp.size(), addr.size() ) );

        self->onClose([=]( node_t* ){ 
            EM_EVAL( _STRING_( document.querySelector('[_${0}_]').remove() ), addr );
            process::revoke( addr ); // console::log( "removed" );
        });

        uif.then([=]( string_t data ){ srand( process::now() );

            auto clr    = array_t<int> ({ 0, 1, 2, 1, 0, 1, 2, 1, 0 });
            auto ord    = array_t<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9 })
                .sort([=]( int a, int b ){ return rand()%2==0; });
            auto button = string_t(); int y=0;

            for( auto x: ord ){ 
                string_t color = clr[y]==0 ? "primary" : clr[y]==1 ? "warning" : "danger";
                button += regex::format( _STRING_(
                    <label>
                        <a class  ="uk-button uk-button-light-outline uk-h1"
                           onclick="${0}._nodepp_invoke_( '${1}', ${2} )"
                           style  ="--value: var(--${3})"
                        >${2}</a><input type="radio" hidden>
                    </label>
                ), NODEPP_MODULE_NAME, addr, x, color ); y++; 
            }

            auto processed = regex::format( data, NODEPP_MODULE_NAME, addr, button );

            EM_EVAL( _STRING_(
                var tm = decodeURIComponent(escape(window.atob( '${0}' )));
                var el = document.createElement('div'); el.innerHTML= tm;
                var ol = el.firstElementChild;
                document.querySelector( '[modal]' ).append( ol );
            ),  encoder::base64::atob( processed ) );

        });

        uif.fail([=]( except_t err ){
            console::log( err.what() );
            self->free();
        });

    }); }

    /*─······································································─*/

    node_t minigame_astrod(){ return minigame_generic( ptr_t<string_t>({ "⭐", "⭐", "⭐", "👽", "🛸" }) ); }
    node_t minigame_oxygen(){ return minigame_generic( ptr_t<string_t>({ "🌿", "🍁", "🍀", "🌸" }) ); }
    node_t minigame_trash (){ return minigame_generic( ptr_t<string_t>({ "📦", "📄", "🧻", "📒", "🪙" }) ); }

    node_t minigame_earth(){ return minigame_nav( ptr_t<string_t>({ "🌎", "🌏", "🌍" }) ); }
    node_t minigame_moon (){ return minigame_nav( ptr_t<string_t>({ "🌕","🌖","🌗","🌘","🌑","🌒","🌓","🌔" }) ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/