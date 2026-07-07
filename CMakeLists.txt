cmake_minimum_required( VERSION 3.28 FATAL_ERROR )

option( BUGLE_ENABLE "Enable bugle 📯" OFF )
option( BUGLE_PROFILE "Activate profiling envelope macro" OFF )
option( BUGLE_BUILD_EXAMPLES "Build examples" OFF )
option( BUGLE_BUILD_TESTS "Build tests" OFF )

option( BUGLE_MODULE_CONSOLE "Build the console logger module" ON )
option( BUGLE_MODULE_JSON "Build the json logger module" ON )
option( BUGLE_MODULE_PROFILER "Build the profiler module" ON )
option( BUGLE_MODULE_SYSINFO "Build the build/session info module" ON )
option( BUGLE_MODULE_GPU "Build the gpu info module (fetches webgpu)" OFF )


project( bugle
    VERSION 1.2.0
    DESCRIPTION "A tag-based, filterable, colorful and thread-safe cpp logging library"
    HOMEPAGE_URL "https://github.com/gurki/bugle"
    LANGUAGES CXX
)

add_subdirectory( dependencies )

include( GNUInstallDirs )
include( CMakePackageConfigHelpers )


################################################################################
#  core

add_library( bugle-core STATIC
    src/core/envelope.cpp
    src/core/filter.cpp
    src/core/letter.cpp
    src/core/postoffice.cpp
    src/utility/timestamp.cpp
)

add_library( bugle::core ALIAS bugle-core )

target_compile_features( bugle-core PUBLIC cxx_std_23 )

target_include_directories( bugle-core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries( bugle-core PUBLIC
    nlohmann_json::nlohmann_json
)

if ( BUGLE_ENABLE )
    target_compile_definitions( bugle-core PUBLIC BUGLE_ENABLE )
endif()

if ( BUGLE_PROFILE )
    target_compile_definitions( bugle-core PUBLIC BUGLE_PROFILE )
endif()

install( TARGETS bugle-core EXPORT bugleTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


################################################################################
#  umbrella target, links core and every enabled module

add_library( bugle INTERFACE )
add_library( bugle::bugle ALIAS bugle )
target_link_libraries( bugle INTERFACE bugle-core )
install( TARGETS bugle EXPORT bugleTargets )


################################################################################
#  helper: define an optional module target

function( bugle_add_module name )
    add_library( bugle-${name} STATIC ${ARGN} )
    add_library( bugle::${name} ALIAS bugle-${name} )
    target_link_libraries( bugle-${name} PUBLIC bugle-core )
    target_link_libraries( bugle INTERFACE bugle-${name} )

    install( TARGETS bugle-${name} EXPORT bugleTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endfunction()


################################################################################
#  console module

if ( BUGLE_MODULE_CONSOLE )
    bugle_add_module( console
        src/format/colortable.cpp
        src/format/formatter.cpp
        src/format/theme.cpp
        src/recipients/consolelogger.cpp
    )
endif()


################################################################################
#  json module

if ( BUGLE_MODULE_JSON )
    bugle_add_module( json
        src/recipients/jsonlogger.cpp
    )
endif()


################################################################################
#  profiler module

if ( BUGLE_MODULE_PROFILER )
    bugle_add_module( profiler
        src/recipients/profiler.cpp
    )
endif()


################################################################################
#  sysinfo module (build + session info banners)

if ( BUGLE_MODULE_SYSINFO )

    bugle_add_module( sysinfo
        src/utility/buildinfo.cpp
        src/utility/sessioninfo.cpp
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

    #  only buildinfo.cpp / sessioninfo.cpp read these
    target_compile_definitions( bugle-sysinfo PRIVATE
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
        $<$<CONFIG:Release>:BUGLE_BUILD_TYPE="Release">
        $<$<CONFIG:Profile>:BUGLE_BUILD_TYPE="Profile">
        $<$<CONFIG:MinSizeRel>:BUGLE_BUILD_TYPE="MinSizeRel">
        $<$<CONFIG:RelWithDebInfo>:BUGLE_BUILD_TYPE="RelWithDebInfo">
    )

endif()


################################################################################
#  gpu module (webgpu backend, not installed — consume via FetchContent)

if ( BUGLE_MODULE_GPU )
    add_library( bugle-gpu STATIC
        src/utility/gpuinfo.cpp
    )

    add_library( bugle::gpu ALIAS bugle-gpu )
    target_link_libraries( bugle-gpu PUBLIC bugle-core webgpu )

    #  build-interface only: bugle-gpu is not installed, so the exported
    #  umbrella must not reference it
    target_link_libraries( bugle INTERFACE $<BUILD_INTERFACE:bugle-gpu> )
endif()


################################################################################
#  install package config

install( DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install( EXPORT bugleTargets
    FILE bugleTargets.cmake
    NAMESPACE bugle::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bugle
)

configure_package_config_file(
    cmake/bugleConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/bugleConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bugle
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/bugleConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/bugleConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/bugleConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bugle
)


################################################################################
#  examples and tests

if ( BUGLE_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()

if ( BUGLE_BUILD_TESTS )
    add_subdirectory( tests )
endif()

#  examples export APP_* to parent scope, so this must come after them
if ( APP_NAME AND TARGET bugle-sysinfo )
    target_compile_definitions( bugle-sysinfo PRIVATE
        APP_NAME=${APP_NAME}
        APP_VERSION=${APP_VERSION}
        APP_COMMIT=${APP_COMMIT}
    )
endif()
