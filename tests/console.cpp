#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>

TEST_CASE( "plain formatting and ordered banners", "[console]" ) {
    bugle::Formatter formatter;
    const auto line = formatter.format( bugle::Letter( "ready", { "info" }, {{ "count", 3 }} ) );
    REQUIRE( line.find( "ready" ) != std::string::npos );
    REQUIRE( line.find( "count" ) != std::string::npos );
    REQUIRE( line.find( '\x1b' ) == std::string::npos );
    const auto banner = formatter.banner( {
        { "_title", "Server" }, { "_order", { "zebra", "alpha" } },
        { "zebra", 2 }, { "alpha", 1 }, { "_secret", "hidden" }
    } );
    REQUIRE( banner.find( "zebra" ) < banner.find( "alpha" ) );
    REQUIRE( banner.find( "hidden" ) == std::string::npos );
    REQUIRE( banner.find( '\x1b' ) == std::string::npos );
}
