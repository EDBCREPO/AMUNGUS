#include <nodepp/nodepp.h>
#include <nodepp/worker.h>
#include <nodepp/os.h>
#include <nodepp/ws.h>
#include <nodepp/timer.h>
#include <nodepp/optional.h>

/*────────────────────────────────────────────────────────────────────────────*/

#include <express/http.h>
#include <apify/apify.h>

/*────────────────────────────────────────────────────────────────────────────*/

using namespace nodepp;

/*────────────────────────────────────────────────────────────────────────────*/

#include "controller/http.cpp"
#include "controller/ws.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

void onMain(){ 

    limit::set_process_priority( limit::PRIORITY::HIGH_PRIORITY );
    limit::set_hard_fileno/*-*/( os::cpus() * (uint)-1 );
    limit::set_soft_fileno/*-*/( os::cpus() * (uint)-1 );

    for( int x=os::cpus(); x-->0; ){    
    worker::add([=](){
        int port = 8000 + x + 1;
        controller::http_server( 8000, x );
        controller::ws_server  ( port, x );
    process::wait(); return -1; });
    }

}

/*────────────────────────────────────────────────────────────────────────────*/