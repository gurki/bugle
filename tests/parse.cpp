#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "parse filter", "[filter]" )
{
    SECTION( "trim" ) {
        std::string_view res = bugle::trim( "  wh a t? " );
        REQUIRE( std::string( res ) == "wh a t?" );
    }

    SECTION( "notIsEmpty" ) {
        REQUIRE( bugle::notIsEmpty( "     a      " ) );
        REQUIRE_FALSE( bugle::notIsEmpty( "\n\t\r" ) );
    }

    SECTION( "conjunction" )
    {
        auto con = bugle::parseConjunction( "tag:info !tag:debug !attribute:value>100" );
        const auto literals = std::ranges::to<bugle::Conjunction>( con );

        REQUIRE( literals.size() == 3 );

        REQUIRE( literals[ 0 ].negate == false );
        REQUIRE( std::string( literals[ 0 ].type ) == "tag" );
        REQUIRE( std::string( literals[ 0 ].variable ) == "info" );

        REQUIRE( literals[ 1 ].negate == true );
        REQUIRE( std::string( literals[ 1 ].type ) == "tag" );
        REQUIRE( std::string( literals[ 1 ].variable ) == "debug" );

        REQUIRE( literals[ 2 ].negate == true );
        REQUIRE( std::string( literals[ 2 ].type ) == "attribute" );
        REQUIRE( std::string( literals[ 2 ].variable ) == "value>100" );
    }

    SECTION( "disjunction" )
    {
        auto dis = bugle::parseDisjunction( R"(
            !file:main.cpp tag:debug
            line:31

        )");

        const auto cons = std::ranges::to<bugle::Disjunction>( dis );

        REQUIRE( cons.size() == 2 );
        REQUIRE( cons[ 0 ].size() == 2 );
        REQUIRE( cons[ 1 ].size() == 1 );

        REQUIRE( cons[ 0 ][ 0 ].negate == true );
        REQUIRE( std::string( cons[ 0 ][ 0 ].type ) == "file" );
        REQUIRE( std::string( cons[ 0 ][ 0 ].variable ) == "main.cpp" );

        REQUIRE( cons[ 0 ][ 1 ].negate == false );
        REQUIRE( std::string( cons[ 0 ][ 1 ].type ) == "tag" );
        REQUIRE( std::string( cons[ 0 ][ 1 ].variable ) == "debug" );

        REQUIRE( cons[ 1 ][ 0 ].negate == false );
        REQUIRE( std::string( cons[ 1 ][ 0 ].type ) == "line" );
        REQUIRE( std::string( cons[ 1 ][ 0 ].variable ) == "31" );
    }

    SECTION( "disjunction from tag" )
    {
        auto dis = bugle::parseDisjunction( "tag:debug" );

        const auto cons = std::ranges::to<bugle::Disjunction>( dis );

        REQUIRE( cons.size() == 1 );
        REQUIRE( cons[ 0 ].size() == 1 );

        REQUIRE( cons[ 0 ][ 0 ].negate == false );
        REQUIRE( std::string( cons[ 0 ][ 0 ].type ) == "tag" );
        REQUIRE( std::string( cons[ 0 ][ 0 ].variable ) == "debug" );
    }
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "build filter", "[filter]" )
{
    SECTION( "conjunction" )
    {
        const bugle::Filter filter = bugle::Filter::fromString( R"(
            tag:info !tag:debug !attribute:value>100
        )" );

        bugle::Letter letter;

        letter.tags = { "info", "warning" };
        REQUIRE( filter.matches( letter ) );

        letter.tags = { "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );
    }

    SECTION( "disjunction" )
    {
        const bugle::Filter filter = bugle::Filter::fromString( R"(
            tag:info !tag:debug !attribute:value>100
            message:hi
        )" );

        bugle::Letter letter;

        letter.tags = { "info", "warning" };
        REQUIRE( filter.matches( letter ) );

        letter.tags = { "debug" };
        REQUIRE_FALSE( filter.matches( letter ) );

        letter.message = "hi";
        REQUIRE( filter.matches( letter ) );
    }
}