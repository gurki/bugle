#include "bugle/utility/buildinfo.h"

namespace bugle {


BuildInfo BuildInfo::current() 
{
    static const BuildInfo info 
    {
        .time = BUGLE_BUILD_TIME,
        .host = BUGLE_BUILD_HOST,
        .directory = BUGLE_BUILD_DIRECTORY,

        .type = BUGLE_BUILD_TYPE,
        .cmakeVersion = BUGLE_BUILD_CMAKE_VERSION,
        .cmakeGenerator = BUGLE_BUILD_CMAKE_GENERATOR,
        .compilerName = BUGLE_BUILD_COMPILER_NAME,
        .compilerVersion = BUGLE_BUILD_COMPILER_VERSION,
        
        .systemName = BUGLE_BUILD_SYSTEM_NAME,
        .systemVersion = BUGLE_BUILD_SYSTEM_VERSION,
        .systemArchitecture = BUGLE_BUILD_SYSTEM_ARCHITECTURE,

        .cpuCoresLogical = BUGLE_BUILD_CPU_CORES_LOGICAL,
        .cpuCoresPhysical = BUGLE_BUILD_CPU_CORES_PHYSICAL,
        .ramTotal = BUGLE_BUILD_RAM_TOTAL,
        .ramAvailable = BUGLE_BUILD_RAM_AVAILABLE,
        .vramTotal = BUGLE_BUILD_VRAM_TOTAL,
        .vramAvailable = BUGLE_BUILD_VRAM_AVAILABLE
    };

    return info;
}


}   //  ::bugle