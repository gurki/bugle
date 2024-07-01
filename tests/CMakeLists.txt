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
)

target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_23 )

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries( ${PROJECT_NAME} PRIVATE
    bugle
    Catch2::Catch2WithMain
)

list( APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras )
include( CTest )
include( Catch )
catch_discover_tests( ${PROJECT_NAME} TEST_SPEC --success )