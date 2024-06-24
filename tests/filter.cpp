#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>

#define CATCH_CONFIG_MAIN


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