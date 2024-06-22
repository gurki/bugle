#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct SessionInfo 
{
    std::string localTime;
    std::string systemName;
    std::string systemVersion;
    std::string systemArchitecture;
    std::string cpuModel;
    int cpuCores;
    uint64_t ramTotal;
    uint64_t ramAvailable;

    static SessionInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo,
    localTime,
    systemName,
    systemVersion,
    systemArchitecture,
    cpuModel,
    ramTotal,
    ramAvailable
);


}   //  ::bugle