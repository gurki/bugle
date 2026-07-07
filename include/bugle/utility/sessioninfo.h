#pragma once

#include <nlohmann/json.hpp>
#include <vector>

namespace bugle {


struct SessionInfo
{
    std::string _title = "Session";
    std::string _icon = "💡";
    std::vector<std::string> _order = { "application", "paths", "system", "hardware" };

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
        std::string name;
        std::string version;
        std::string architecture;
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo::Paths,
    _icon,
    current,
    temp
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( SessionInfo::System,
    _icon,
    name,
    version,
    architecture
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
    _order,
    application,
    paths,
    system,
    hardware
);


}   //  ::bugle
