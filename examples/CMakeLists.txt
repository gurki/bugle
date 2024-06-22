cmake_minimum_required( VERSION 3.28 )


set( BUGLE_ENABLE ON CACHE BOOL "Enable bugle 📯" FORCE )


project(
    bugle-examples
    VERSION 0.0.0
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

target_compile_definitions( ${PROJECT_NAME}
    PUBLIC
        APP_NAME=\"${PROJECT_NAME}\"
        APP_VERSION=\"${PROJECT_VERSION}\"
        APP_COMMIT=\"${PROJECT_COMMIT}\"
)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)