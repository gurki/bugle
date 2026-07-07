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
        REQUIRE( std::string( literals[ 2 ].variable ) == "value" );
        REQUIRE( std::string( literals[ 2 ].op ) == ">" );
        REQUIRE( std::string( literals[ 2 ].value ) == "100" );
    }

    SECTION( "comparison operators" )
    {
        auto con = bugle::parseConjunction( "attribute:count>=2 attribute:name=core attribute:state!=idle" );
        const auto literals = std::ranges::to<bugle::Conjunction>( con );

        REQUIRE( literals.size() == 3 );

        REQUIRE( std::string( literals[ 0 ].variable ) == "count" );
        REQUIRE( std::string( literals[ 0 ].op ) == ">=" );
        REQUIRE( std::string( literals[ 0 ].value ) == "2" );

        REQUIRE( std::string( literals[ 1 ].variable ) == "name" );
        REQUIRE( std::string( literals[ 1 ].op ) == "=" );
        REQUIRE( std::string( literals[ 1 ].value ) == "core" );

        REQUIRE( std::string( literals[ 2 ].variable ) == "state" );
        REQUIRE( std::string( literals[ 2 ].op ) == "!=" );
        REQUIRE( std::string( literals[ 2 ].value ) == "idle" );
    }

    SECTION( "literal without colon is invalid" )
    {
        const auto literal = bugle::parseLiteral( "loose" );
        REQUIRE( bugle::typeFromString( literal.type ) == bugle::VariableType::Invalid );
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

    SECTION( "value comparison" )
    {
        const bugle::Filter filter = bugle::Filter::fromString( "attribute:value>100" );

        bugle::Letter letter;

        letter.attributes = { { "value", 101 } };
        REQUIRE( filter.matches( letter ) );

        letter.attributes = { { "value", 100 } };
        REQUIRE_FALSE( filter.matches( letter ) );

        letter.attributes = { { "other", 200 } };
        REQUIRE_FALSE( filter.matches( letter ) );
    }

    SECTION( "negated value comparison" )
    {
        //  the canonical example expression: info, not debug, and not value > 100
        const bugle::Filter filter = bugle::Filter::fromString( "tag:info !attribute:value>100" );

        bugle::Letter letter;
        letter.tags = { "info" };

        letter.attributes = { { "value", 50 } };
        REQUIRE( filter.matches( letter ) );

        letter.attributes = { { "value", 150 } };
        REQUIRE_FALSE( filter.matches( letter ) );

        letter.attributes = {};     //  missing attribute is not > 100
        REQUIRE( filter.matches( letter ) );
    }

    SECTION( "comparison operator variants" )
    {
        bugle::Letter letter;
        letter.attributes = { { "count", 2 }, { "name", "core" } };

        REQUIRE( bugle::Filter::fromString( "attribute:count>=2" ).matches( letter ) );
        REQUIRE( bugle::Filter::fromString( "attribute:count<=2" ).matches( letter ) );
        REQUIRE( bugle::Filter::fromString( "attribute:count<3" ).matches( letter ) );
        REQUIRE( bugle::Filter::fromString( "attribute:count=2" ).matches( letter ) );
        REQUIRE( bugle::Filter::fromString( "attribute:count!=3" ).matches( letter ) );
        REQUIRE_FALSE( bugle::Filter::fromString( "attribute:count!=2" ).matches( letter ) );

        //  string operands
        REQUIRE( bugle::Filter::fromString( "attribute:name=core" ).matches( letter ) );
        REQUIRE_FALSE( bugle::Filter::fromString( "attribute:name=shell" ).matches( letter ) );
    }
}