#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


struct SessionInfo
{
    std::string _title = "Session";
    std::string _icon = "💡";
    
    struct Application {
        std::string _icon = "🍎";
        std::string timestamp;
        std::string appName;
        std::string appVersion;
        std::string appCommit;
    } application;
     
    struct Paths {
        std::string _icon = "🛣️";
        std::string current;
        std::string temp;
    } paths;
    
    struct System {
        std::string _icon = "💾";
        std::string systemName;
        std::string systemVersion;
        std::string systemArchitecture;
    } system;
    
    struct Hardware {
        std::string _icon = "💻";
        std::string cpuModel;
        int cpuCores;
        float ramTotalGiB;
        float ramAvailableGiB;
    } hardware;

    static SessionInfo current();
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo::Application,
    _icon,
    timestamp,
    appName,
    appVersion,
    appCommit
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo::System,
    _icon,
    systemName,
    systemVersion,
    systemArchitecture
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo::Paths,
    _icon,
    current,
    temp
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo::Hardware,
    _icon,
    cpuModel,
    cpuCores,
    ramTotalGiB,
    ramAvailableGiB
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo,
    _title,
    _icon,
    application,
    paths,
    system,
    hardware
);


}   //  ::bugle
