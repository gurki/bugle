#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct BuildInfo
{
    //  environment
    std::string timestamp;
    std::string bugle;
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
    std::string cpuName;
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
    bugle,
    host,
    directory,

    type,
    cmakeVersion,
    cmakeGenerator,
    compilerName,
    compilerVersion,

    systemName,
    systemVersion,
    systemArchitecture,

    cpuName,
    cpuCoresLogical,
    cpuCoresPhysical,
    ramTotalMb,
    ramAvailableMb,
    vramTotalMb,
    vramAvailableMb
);


}   //  ::bugle
