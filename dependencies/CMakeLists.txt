cmake_minimum_required( VERSION 3.28 FATAL_ERROR )

#  prefer a system-provided package, fall back to FetchContent
find_package( nlohmann_json 3.11 QUIET )

if ( NOT nlohmann_json_FOUND )

    include( FetchContent )

    #  install alongside bugle so the exported bugle package can resolve
    #  its nlohmann_json::nlohmann_json dependency via find_dependency
    set( JSON_Install ON CACHE INTERNAL "" )

    FetchContent_Declare( json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.11.3
    )

    FetchContent_MakeAvailable( json )

endif()


#  webgpu backend for the gpu info module
if ( BUGLE_MODULE_GPU )

    include( FetchContent )

    set( WEBGPU_BACKEND WGPU )

    FetchContent_Declare( webgpu
        GIT_REPOSITORY      https://github.com/eliemichel/WebGPU-distribution
        GIT_TAG             fc0e32a
        UPDATE_DISCONNECTED TRUE
    )

    FetchContent_MakeAvailable( webgpu )

endif()
