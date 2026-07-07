cmake_minimum_required( VERSION 3.28 FATAL_ERROR )


set( BUGLE_ENABLE ON CACHE BOOL "Enable bugle 📯" FORCE )


project(
    bugle-examples
    VERSION 1.0.0
    DESCRIPTION "A tag-based, filterable, colorful and thread-safe cpp logging library"
    HOMEPAGE_URL "https://github.com/gurki/bugle"
    LANGUAGES CXX
)


################################################################################
#  helper: one executable per demo, linking only the modules it needs

function( bugle_add_example name )
    add_executable( bugle-example-${name} ${name}.cpp )
    target_compile_features( bugle-example-${name} PRIVATE cxx_std_23 )
    target_link_libraries( bugle-example-${name} PRIVATE ${ARGN} )
endfunction()


################################################################################
#  per-module demos

if ( BUGLE_MODULE_CONSOLE )
    bugle_add_example( console bugle::console )
    bugle_add_example( filter bugle::console )
endif()

if ( BUGLE_MODULE_JSON )
    bugle_add_example( json bugle::json )
endif()

if ( BUGLE_MODULE_PROFILER )
    bugle_add_example( profiler bugle::profiler )
endif()

if ( BUGLE_MODULE_SYSINFO AND BUGLE_MODULE_CONSOLE )
    bugle_add_example( sysinfo bugle::sysinfo bugle::console )
endif()

if ( BUGLE_MODULE_GPU AND BUGLE_MODULE_CONSOLE )
    bugle_add_example( gpu bugle::gpu bugle::console )
    target_copy_webgpu_binaries( bugle-example-gpu )
endif()


################################################################################
#  kitchen-sink overview, requires all default modules

if ( BUGLE_MODULE_CONSOLE AND BUGLE_MODULE_JSON AND BUGLE_MODULE_PROFILER AND BUGLE_MODULE_SYSINFO )

    add_executable( ${PROJECT_NAME}
        main.cpp
    )

    target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_23 )

    target_link_libraries( ${PROJECT_NAME} PUBLIC
        bugle::bugle
    )

    if ( BUGLE_MODULE_GPU )
        target_compile_definitions( ${PROJECT_NAME} PRIVATE BUGLE_HAS_GPU )
        target_copy_webgpu_binaries( ${PROJECT_NAME} )
    endif()

endif()


################################################################################
#  app metadata, consumed by the sysinfo module (see root CMakeLists.txt)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set( APP_NAME \"${PROJECT_NAME}\" PARENT_SCOPE )
set( APP_VERSION \"${PROJECT_VERSION}\" PARENT_SCOPE )
set( APP_COMMIT \"${PROJECT_COMMIT}\" PARENT_SCOPE )
