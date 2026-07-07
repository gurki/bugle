#include <catch2/catch_test_macros.hpp>
#include <bugle/utility/timestamp.h>


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "parse timestamps", "[timestamp]" )
{
    SECTION( "roundtrip" )
    {
        const std::string iso = "2024-03-15T12:34:56";
        const auto ts = bugle::Timestamp::parse( iso );

        REQUIRE( ts.valid() );
        REQUIRE( ts.info<std::chrono::seconds>() == iso );
    }

    SECTION( "roundtrip with milliseconds" )
    {
        const std::string iso = "2024-03-15T12:34:56.789";
        const auto ts = bugle::Timestamp::parse( iso );

        REQUIRE( ts.valid() );
        REQUIRE( ts.milliseconds() == 789 );
        REQUIRE( ts.info<std::chrono::milliseconds>() == iso );
    }

    SECTION( "fraction scales by digit count" )
    {
        REQUIRE( bugle::Timestamp::parse( "2024-03-15T12:34:56.5" ).milliseconds() == 500 );
        REQUIRE( bugle::Timestamp::parse( "2024-03-15T12:34:56.050" ).milliseconds() == 50 );
        REQUIRE( bugle::Timestamp::parse( "2024-03-15T12:34:56.000123" ).microseconds() == 123 );
        REQUIRE( bugle::Timestamp::parse( "2024-03-15T12:34:56.123456" ).microseconds() == 123456 );
    }

    SECTION( "invalid input" )
    {
        REQUIRE_FALSE( bugle::Timestamp::parse( "" ).valid() );
        REQUIRE_FALSE( bugle::Timestamp::parse( "not a timestamp" ).valid() );
        REQUIRE_FALSE( bugle::Timestamp::parse( "12:34:56" ).valid() );
    }

    SECTION( "json roundtrip" )
    {
        const auto ts = bugle::Timestamp::parse( "2024-03-15T12:34:56.123456" );
        const nlohmann::json json = ts;
        const auto back = json.get<bugle::Timestamp>();

        REQUIRE( back == ts );
    }
}
