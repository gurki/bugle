cmake_minimum_required( VERSION 3.28 FATAL_ERROR )

option( BUGLE_ENABLE "Enable bugle 📯" OFF )
option( BUGLE_BUILD_EXAMPLES "Build examples" OFF )
option( BUGLE_BUILD_TESTS "Build tests" OFF )


project(
    bugle
    VERSION 1.1.0
    DESCRIPTION "A tag-based, filterable, colorful and thread-safe cpp logging library"
    HOMEPAGE_URL "https://github.com/gurki/bugle"
    LANGUAGES CXX
)

add_subdirectory( dependencies )


add_library( ${PROJECT_NAME} STATIC
    src/core/envelope.cpp
    src/core/filter.cpp
    src/core/letter.cpp
    src/core/postoffice.cpp
    src/filter/address.cpp
    src/filter/lines.cpp
    src/filter/route.cpp
    src/format/colortable.cpp
    src/format/formatter.cpp
    src/format/theme.cpp
    src/recipients/consolelogger.cpp
    src/recipients/jsonlogger.cpp
    src/recipients/profiler.cpp
    src/utility/timestamp.cpp
    src/utility/utility.cpp
    src/utility/gpuinfo.cpp
    src/utility/buildinfo.cpp
    src/utility/sessioninfo.cpp
)

target_compile_features( ${PROJECT_NAME} PUBLIC
    cxx_std_23
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries( ${PROJECT_NAME} PUBLIC
    nlohmann_json
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_BINARY_DIR}
)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE BUGLE_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string( TIMESTAMP BUGLE_BUILD_TIME "%Y-%m-%dT%H:%M:%S" )

cmake_host_system_information( RESULT BUGLE_BUILD_HOST QUERY HOSTNAME )
cmake_host_system_information( RESULT BUGLE_BUILD_CPU_CORES_LOGICAL QUERY NUMBER_OF_LOGICAL_CORES )
cmake_host_system_information( RESULT BUGLE_BUILD_CPU_CORES_PHYSICAL QUERY NUMBER_OF_PHYSICAL_CORES )
cmake_host_system_information( RESULT BUGLE_BUILD_CPU_NAME QUERY PROCESSOR_DESCRIPTION )
cmake_host_system_information( RESULT BUGLE_BUILD_VRAM_TOTAL QUERY TOTAL_VIRTUAL_MEMORY )
cmake_host_system_information( RESULT BUGLE_BUILD_VRAM_AVAILABLE QUERY AVAILABLE_VIRTUAL_MEMORY )
cmake_host_system_information( RESULT BUGLE_BUILD_RAM_TOTAL QUERY TOTAL_PHYSICAL_MEMORY )
cmake_host_system_information( RESULT BUGLE_BUILD_RAM_AVAILABLE QUERY AVAILABLE_PHYSICAL_MEMORY )

target_compile_definitions( ${PROJECT_NAME}
    PUBLIC
        BUGLE_VERSION=\"${PROJECT_VERSION}\"
        BUGLE_COMMIT=\"${BUGLE_COMMIT}\"

        BUGLE_BUILD_TIME=\"${BUGLE_BUILD_TIME}\"
        BUGLE_BUILD_HOST=\"${BUGLE_BUILD_HOST}\"
        BUGLE_BUILD_DIRECTORY=\"${CMAKE_SOURCE_DIR}\"

        BUGLE_BUILD_CMAKE_VERSION=\"${CMAKE_VERSION}\"
        BUGLE_BUILD_CMAKE_GENERATOR=\"${CMAKE_GENERATOR}\"
        BUGLE_BUILD_COMPILER_NAME=\"${CMAKE_CXX_COMPILER_ID}\"
        BUGLE_BUILD_COMPILER_VERSION=\"${CMAKE_CXX_COMPILER_VERSION}\"

        BUGLE_BUILD_SYSTEM_NAME=\"${CMAKE_SYSTEM_NAME}\"
        BUGLE_BUILD_SYSTEM_VERSION=\"${CMAKE_SYSTEM_VERSION}\"
        BUGLE_BUILD_SYSTEM_ARCHITECTURE=\"${CMAKE_SYSTEM_PROCESSOR}\"

        BUGLE_BUILD_CPU_NAME=\"${BUGLE_BUILD_CPU_NAME}\"
        BUGLE_BUILD_CPU_CORES_LOGICAL=${BUGLE_BUILD_CPU_CORES_LOGICAL}
        BUGLE_BUILD_CPU_CORES_PHYSICAL=${BUGLE_BUILD_CPU_CORES_PHYSICAL}
        BUGLE_BUILD_RAM_TOTAL=${BUGLE_BUILD_RAM_TOTAL}
        BUGLE_BUILD_RAM_AVAILABLE=${BUGLE_BUILD_RAM_AVAILABLE}
        BUGLE_BUILD_VRAM_TOTAL=${BUGLE_BUILD_VRAM_TOTAL}
        BUGLE_BUILD_VRAM_AVAILABLE=${BUGLE_BUILD_VRAM_AVAILABLE}

        $<$<CONFIG:Debug>:BUGLE_BUILD_TYPE="Debug">
        $<$<CONFIG:Release>:BUGLE_BUILD_TYPE="Relase">
        $<$<CONFIG:Profile>:BUGLE_BUILD_TYPE="Profile">
        $<$<CONFIG:MinSizeRel>:BUGLE_BUILD_TYPE="MinSizeRel">
        $<$<CONFIG:RelWithDebInfo>:BUGLE_BUILD_TYPE="RelWithDebInfo">
)

# if ( BUGLE_BUILD_EXAMPLES )
    add_subdirectory( examples )
# endif()

# if ( BUGLE_BUILD_TESTS )
    add_subdirectory( tests )
# endif()

if ( BUGLE_ENABLE )
    target_compile_definitions( ${PROJECT_NAME}
        PUBLIC
            BUGLE_ENABLE
    )
endif()

if ( APP_NAME )
    target_compile_definitions( ${PROJECT_NAME}
        PUBLIC
            APP_NAME=${APP_NAME}
            APP_VERSION=${APP_VERSION}
            APP_COMMIT=${APP_COMMIT}
    )
endif()
