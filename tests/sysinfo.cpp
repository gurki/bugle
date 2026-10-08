#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>
#include <filesystem>
TEST_CASE( "system info follows the banner schema", "[sysinfo]" ) {
    const nlohmann::json build = bugle::BuildInfo::current();
    const nlohmann::json session = bugle::SessionInfo::current();
    REQUIRE( build.contains( "_title" ) );
    REQUIRE( build.at( "system" ).contains( "architecture" ) );
    REQUIRE( session.contains( "_title" ) );
    REQUIRE( session.at( "paths" ).at( "current" ) == std::filesystem::current_path().string() );
}
