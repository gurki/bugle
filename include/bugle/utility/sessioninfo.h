#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct SessionInfo
{
    std::string timestamp;
    std::string systemName;
    std::string systemVersion;
    std::string systemArchitecture;
    std::string cpuModel;
    int cpuCores;
    int ramTotalMb;
    int ramAvailableMb;

    static SessionInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo,
    timestamp,
    systemName,
    systemVersion,
    systemArchitecture,
    cpuModel,
    cpuCores,
    ramTotalMb,
    ramAvailableMb
);


}   //  ::bugle
