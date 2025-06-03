#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct BuildInfo
{
    std::string _title = "Build";
    std::string _icon = "🚧";

    struct Environment {
        std::string _icon = "🌳";
        std::string timestamp;
        std::string bugle;
        std::string host;
        std::string directory;
    } environment;
    
    struct Compilation {
        std::string _icon = "🏭";
        std::string type;
        std::string cmakeVersion;
        std::string cmakeGenerator;
        std::string compilerName;
        std::string compilerVersion;
    } compilation;
    
    struct System {
        std::string _icon = "💾";
        std::string systemName;
        std::string systemVersion;
        std::string systemArchitecture;
    } system;
    
    struct Hardware {
        std::string _icon = "💻";
        std::string cpuName;
        int cpuCoresLogical;
        int cpuCoresPhysical;
        float ramTotalGiB;
        float ramAvailableGiB;
        float vramTotalGiB;
        float vramAvailableGiB;
    } hardware;

    static BuildInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( BuildInfo::Environment,
    _icon,
    timestamp,
    bugle,
    host,
    directory
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( BuildInfo::Compilation,
    _icon,
    type,
    cmakeVersion,
    cmakeGenerator,
    compilerName,
    compilerVersion
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( BuildInfo::System,
    _icon,
    systemName,
    systemVersion,
    systemArchitecture
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( BuildInfo::Hardware,
    _icon,
    cpuName,
    cpuCoresLogical,
    cpuCoresPhysical,
    ramTotalGiB,
    ramAvailableGiB,
    vramTotalGiB,
    vramAvailableGiB
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( BuildInfo,
    _title,
    _icon,
    environment,
    compilation,
    system,
    hardware
);


}   //  ::bugle
