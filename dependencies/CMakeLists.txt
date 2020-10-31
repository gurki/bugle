cmake_minimum_required( VERSION 3.14 FATAL_ERROR )

include( FetchContent )
set( FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR} )

FetchContent_Declare( fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 7.0.3
)

FetchContent_Declare( json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
    GIT_TAG v3.9.1
)

FetchContent_Declare( vivid
    GIT_REPOSITORY https://github.com/gurki/vivid.git
    GIT_TAG origin/deprecate-submodules
)

FetchContent_Declare( Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.2
)

FetchContent_MakeAvailable( fmt json vivid Catch2 )