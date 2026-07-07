//  sysinfo demo: build and session banners, plus a custom banner struct.
//  any letter whose attributes carry a "_title" renders as a banner —
//  "_icon" decorates headings, nested objects become sections, and an
//  optional "_order" array pins section order.

#include <bugle/bugle.h>


////////////////////////////////////////////////////////////////////////////////
//  custom structs render as banners too — no coupling to bugle required
struct ServerInfo
{
    std::string _title = "Server";
    std::string _icon = "🌐";
    std::vector<std::string> _order = { "endpoint", "limits" };

    struct Endpoint {
        std::string _icon = "🔌";
        std::string host = "localhost";
        int port = 8080;
    } endpoint;

    struct Limits {
        std::string _icon = "🚦";
        int maxConnections = 512;
        float timeoutS = 2.5f;
    } limits;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( ServerInfo::Endpoint, _icon, host, port );
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( ServerInfo::Limits, _icon, maxConnections, timeoutS );
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( ServerInfo, _title, _icon, _order, endpoint, limits );


////////////////////////////////////////////////////////////////////////////////
int main()
{
    auto& po = bugle::PostOffice::instance();

    auto console = std::make_shared<bugle::ConsoleLogger>();
    po.addObserver( console );

    //  compile-time build environment, captured by cmake
    const nlohmann::json buildInfo = bugle::BuildInfo::current();
    po.post( {}, { "build", "system" }, buildInfo );

    //  runtime session: app identity, paths, os and hardware
    const nlohmann::json sessionInfo = bugle::SessionInfo::current();
    po.post( {}, { "session", "system" }, sessionInfo );

    //  custom banner
    const nlohmann::json serverInfo = ServerInfo {};
    po.post( {}, { "server", "system" }, serverInfo );

    po.flush();
}
