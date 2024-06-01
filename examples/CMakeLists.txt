cmake_minimum_required( VERSION 3.28 )

project(
    bugle-examples
    VERSION 0.0.0
    DESCRIPTION "A tag-based, filterable, colorful and thread-safe cpp logging library"
    HOMEPAGE_URL "https://github.com/gurki/bugle"
    LANGUAGES CXX
)

add_compile_definitions( BUGLE_ENABLE )
add_compile_definitions( BUGLE_LOWERCASE_DEFINES )

add_executable( ${PROJECT_NAME}
    main.cpp
)

target_include_directories( ${PROJECT_NAME} PUBLIC 
    include
)

target_link_libraries( ${PROJECT_NAME} PUBLIC 
    bugle 
)