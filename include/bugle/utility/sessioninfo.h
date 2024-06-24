#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct SessionInfo
{
    //  app
    std::string timestamp;
    std::string appName;
    std::string appVersion;
    std::string appCommit;

    //  system
    std::string systemName;
    std::string systemVersion;
    std::string systemArchitecture;

    //  hardware
    std::string cpuModel;
    int cpuCores;
    int ramTotalMb;
    int ramAvailableMb;

    static SessionInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo,
    timestamp,
    appName,
    appVersion,
    appCommit,
    systemName,
    systemVersion,
    systemArchitecture,
    cpuModel,
    cpuCores,
    ramTotalMb,
    ramAvailableMb
);


}   //  ::bugle
