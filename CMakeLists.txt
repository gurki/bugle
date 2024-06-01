cmake_minimum_required( VERSION 3.28 )

project(
    bugle
    VERSION 0.0.0
    DESCRIPTION "A tag-based, filterable, colorful and thread-safe cpp logging library"
    HOMEPAGE_URL "https://github.com/gurki/bugle"
    LANGUAGES CXX
)

add_subdirectory( dependencies )
add_compile_definitions( BUGLE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/" )
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/install/" )

option( BUGLE_ENABLE "" ON )
option( BUGLE_LOWERCASE_DEFINES "" ON )

if ( BUGLE_ENABLE )
    add_compile_definitions( BUGLE_ENABLE )
endif()

if ( BUGLE_LOWERCASE_DEFINES )
    add_compile_definitions( BUGLE_LOWERCASE_DEFINES )
endif()


add_library( ${PROJECT_NAME} STATIC
    src/core/booleanfilter.cpp
    src/core/filteritem.cpp
    src/core/message.cpp
    src/core/postoffice.cpp
    src/core/scope.cpp
    src/core/tags.cpp
    src/format/colortable.cpp
    src/format/formatter.cpp
    src/format/theme.cpp
    src/sinks/consolelogger.cpp
    src/sinks/profiler.cpp
    # src/sinks/htmllogger.cpp
    # src/sinks/jsonlogger.cpp
    src/utility/datetime.cpp
    src/utility/utility.cpp
)

add_library( ${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )

target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_23 )

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries( ${PROJECT_NAME}
    PUBLIC
        nlohmann_json
        vivid
)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

install(
    TARGETS bugle
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)


add_subdirectory( examples )