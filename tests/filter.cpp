#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "tag filter", "[filter]" )
{
    bugle::TagFilter filter( "info" );
    bugle::Letter letter;

    SECTION( "match single" ) {
        letter.tags = { "info" };
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "mismatch single" ) {
        letter.tags = { "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }

    SECTION( "match multiple" ) {
        letter.tags = { "info", "debug" };
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "mismatch multiple" ) {
        letter.tags = { "success", "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "filter polymorphism - tags", "[filter]" )
{
    bugle::TagFilter tagFilter( "info" );
    bugle::Filter filter = tagFilter;
    bugle::Letter letter;

    SECTION( "match single" ) {
        letter.tags = { "info" };
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "mismatch single" ) {
        letter.tags = { "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }

    SECTION( "match multiple" ) {
        letter.tags = { "info", "debug" };
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "mismatch multiple" ) {
        letter.tags = { "success", "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "filter polymorphism - route", "[filter]" )
{
    bugle::TagFilter infoFilter( "info" );
    bugle::TagFilter warningFilter( "warning" );
    bugle::TagFilter debugFilter( "debug" );

    bugle::Address address({
        { infoFilter, false },
        { debugFilter, true }
    });

    bugle::Route route( { address, warningFilter } );

    bugle::Filter filter = route;
    bugle::Letter letter;

    SECTION( "match info" ) {
        letter.tags = { "info" };
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "mismatch debug" ) {
        letter.tags = { "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }

    SECTION( "match multiple" ) {
        letter.tags = { "info", "warning" };
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "mismatch multiple" ) {
        letter.tags = { "success", "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "filter polymorphism - scoped", "[filter]" )
{
    bugle::Filter filter;

    {
        bugle::TagFilter infoFilter( "info" );
        bugle::TagFilter warningFilter( "warning" );
        bugle::TagFilter debugFilter( "debug" );

        bugle::Address address({
            { infoFilter, false },
            { debugFilter, true }
        });

        bugle::Route route( { address, warningFilter } );

        filter = route;
    }

    bugle::Letter letter;

    SECTION( "match info" ) {
        letter.tags = { "info" };
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "mismatch debug" ) {
        letter.tags = { "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }

    SECTION( "match multiple" ) {
        letter.tags = { "info", "warning" };
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "mismatch multiple" ) {
        letter.tags = { "success", "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }
}