cmake_minimum_required( VERSION 3.28 )


option( BUGLE_ENABLE OFF "Enable bugle 📯" )


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


add_library( ${PROJECT_NAME} STATIC
    src/core/envelope.cpp
    src/core/letter.cpp
    src/core/postoffice.cpp
    src/filter/address.cpp
    src/filter/route.cpp
    src/filter/tagfilter.cpp
    src/format/colortable.cpp
    src/format/formatter.cpp
    src/format/theme.cpp
    src/recipients/consolelogger.cpp
    src/recipients/jsonlogger.cpp
    src/utility/systeminfo.cpp
    src/utility/timestamp.cpp
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
)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE BUGLE_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_definitions( ${PROJECT_NAME}
    PUBLIC
        BUGLE_VERSION=\"${PROJECT_VERSION}\"
        BUGLE_COMMIT=\"${BUGLE_COMMIT}\"
)


add_subdirectory( examples )


if ( BUGLE_ENABLE )
    add_compile_definitions( BUGLE_ENABLE )
endif()