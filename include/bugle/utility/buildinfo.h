#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct BuildInfo
{
    //  environment
    std::string timestamp;
    std::string host;
    std::string directory;

    //  compilation
    std::string type;
    std::string cmakeVersion;
    std::string cmakeGenerator;
    std::string compilerName;
    std::string compilerVersion;

    //  system
    std::string systemName;
    std::string systemVersion;
    std::string systemArchitecture;

    //  hardware
    int cpuCoresLogical;
    int cpuCoresPhysical;
    int ramTotalMb;
    int ramAvailableMb;
    int vramTotalMb;
    int vramAvailableMb;

    static BuildInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( BuildInfo,
    timestamp,
    host,
    type,
    directory,

    cmakeVersion,
    cmakeGenerator,
    compilerName,
    compilerVersion,

    systemName,
    systemVersion,
    systemArchitecture,

    cpuCoresLogical,
    cpuCoresPhysical,
    ramTotalMb,
    ramAvailableMb,
    vramTotalMb,
    vramAvailableMb
);


}   //  ::bugle
