cmake_minimum_required(VERSION 3.16)
project( AMG_CLIENT CXX) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	nodepp
	GIT_REPOSITORY   https://github.com/NodeppOfficial/nodepp-wasm
	GIT_TAG          origin/main
	GIT_PROGRESS     ON
)
FetchContent_MakeAvailable(nodepp)

include(FetchContent)
FetchContent_Declare(
	nodepp-apify
	GIT_REPOSITORY   https://github.com/NodeppOfficial/nodepp-apify
	GIT_TAG          origin/main
	GIT_PROGRESS     ON
)
FetchContent_MakeAvailable(nodepp-apify)

include(FetchContent)
FetchContent_Declare(
	nodepp-raylib
	GIT_REPOSITORY   https://github.com/NodeppOfficial/nodepp-raylib
	GIT_TAG          origin/main
	GIT_PROGRESS     ON
)
FetchContent_MakeAvailable(nodepp-raylib)

include(FetchContent)
FetchContent_Declare(
	nodepp-ungine
	GIT_REPOSITORY   https://github.com/NodeppOfficial/nodepp-ungine
	GIT_TAG          origin/main
	GIT_PROGRESS     ON
)
FetchContent_MakeAvailable(nodepp-ungine)

set(EMCC_LINK_FLAGS
#	"SHELL:-O3"
    "SHELL:-s ASSERTIONS=0"
	"SHELL:-s MODULARIZE=1"
    "SHELL:-s FORCE_FILESYSTEM=1"
	"SHELL:-s MAX_WEBGL_VERSION=2"
    "SHELL:-s MIN_WEBGL_VERSION=2"
	"SHELL:-s ALLOW_MEMORY_GROWTH"
	"SHELL:-s ASYNCIFY_STACK_SIZE=65536"
	"SHELL:--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets"
)

add_executable( main ./main.cpp )
target_link_options  ( main PRIVATE ${EMCC_LINK_FLAGS})
target_link_libraries( main PUBLIC nodepp nodepp-raylib nodepp-ungine nodepp-apify )
target_include_directories( main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include" )

set_target_properties( main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../backend/www" )
set_target_properties( main PROPERTIES SUFFIX ".js" )