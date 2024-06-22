#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct BuildInfo 
{
    //  environment
    std::string time;
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
    std::string cpuCoresLogical;
    std::string cpuCoresPhysical;
    std::string ramTotal;
    std::string ramAvailable;
    std::string vramTotal;
    std::string vramAvailable;

    static BuildInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( BuildInfo,
    time,
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
    ramTotal,
    ramAvailable,
    vramTotal,
    vramAvailable
);


}   //  ::bugle