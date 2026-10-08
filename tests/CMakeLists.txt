cmake_minimum_required( VERSION 3.28 )

include( FetchContent )

FetchContent_Declare( Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.6.0
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable( Catch2 )

project( bugle-tests
    LANGUAGES CXX
)

add_executable( ${PROJECT_NAME}
    filter.cpp
    letter.cpp
    parse.cpp
    postoffice.cpp
    lifecycle.cpp
    timestamp.cpp
)

target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_23 )

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

#  Add module coverage only when the module is enabled.
foreach( module console json profiler sysinfo gpu )
    if( TARGET bugle-${module} )
        target_sources( ${PROJECT_NAME} PRIVATE ${module}.cpp )
        target_link_libraries( ${PROJECT_NAME} PRIVATE bugle::${module} )
    endif()
endforeach()
if( TARGET bugle-gpu )
    target_copy_webgpu_binaries( ${PROJECT_NAME} )
endif()

target_link_libraries( ${PROJECT_NAME} PRIVATE
    bugle::core
    Catch2::Catch2WithMain
)

list( APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras )
include( CTest )
include( Catch )
catch_discover_tests( ${PROJECT_NAME} TEST_SPEC --success PROPERTIES TIMEOUT 15 )
add_test( NAME installed-package
    COMMAND ${CMAKE_COMMAND}
        -DBUGLE_BUILD_DIR=${CMAKE_BINARY_DIR}
        -DBUGLE_TEST_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -DBUGLE_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -DBUGLE_CONFIG=$<CONFIG>
        -DBUGLE_GENERATOR=${CMAKE_GENERATOR}
        -DBUGLE_COMPILER=${CMAKE_CXX_COMPILER}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/package.cmake
)
set_tests_properties( installed-package PROPERTIES TIMEOUT 120 )
