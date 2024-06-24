cmake_minimum_required( VERSION 3.28 )


set( BUGLE_ENABLE ON CACHE BOOL "Enable bugle 📯" FORCE )


project(
    bugle-examples
    VERSION 1.0.0
    DESCRIPTION "A tag-based, filterable, colorful and thread-safe cpp logging library"
    HOMEPAGE_URL "https://github.com/gurki/bugle"
    LANGUAGES CXX
)

add_executable( ${PROJECT_NAME}
    main.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    include
)

target_link_libraries( ${PROJECT_NAME} PUBLIC
    bugle
)

target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_23 )

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set( APP_NAME \"${PROJECT_NAME}\" PARENT_SCOPE )
set( APP_VERSION \"${PROJECT_VERSION}\" PARENT_SCOPE )
set( APP_COMMIT \"${PROJECT_COMMIT}\" PARENT_SCOPE )
