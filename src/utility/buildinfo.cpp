#include "bugle/utility/buildinfo.h"

namespace bugle {


BuildInfo BuildInfo::current()
{
    static const BuildInfo info
    {
        .environment = {
            .timestamp = BUGLE_BUILD_TIME,
            .bugle = BUGLE_VERSION,
            .host = BUGLE_BUILD_HOST,
            .directory = BUGLE_BUILD_DIRECTORY,
        },
        .compilation = {
            .type = BUGLE_BUILD_TYPE,
            .cmakeVersion = BUGLE_BUILD_CMAKE_VERSION,
            .cmakeGenerator = BUGLE_BUILD_CMAKE_GENERATOR,
            .compilerName = BUGLE_BUILD_COMPILER_NAME,
            .compilerVersion = BUGLE_BUILD_COMPILER_VERSION
        },
        .system = {
            .systemName = BUGLE_BUILD_SYSTEM_NAME,
            .systemVersion = BUGLE_BUILD_SYSTEM_VERSION,
            .systemArchitecture = BUGLE_BUILD_SYSTEM_ARCHITECTURE,
        },        
        .hardware = {
            .cpuName = BUGLE_BUILD_CPU_NAME,
            .cpuCoresLogical = BUGLE_BUILD_CPU_CORES_LOGICAL,
            .cpuCoresPhysical = BUGLE_BUILD_CPU_CORES_PHYSICAL,
            .ramTotalGiB = BUGLE_BUILD_RAM_TOTAL / 1024.f,
            .ramAvailableGiB = BUGLE_BUILD_RAM_AVAILABLE / 1024.f,
            .vramTotalGiB = BUGLE_BUILD_VRAM_TOTAL / 1024.f,
            .vramAvailableGiB = BUGLE_BUILD_VRAM_AVAILABLE / 1024.f,
        }
    };

    return info;
}


}   //  ::bugle
