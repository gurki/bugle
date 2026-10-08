#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>
#include <filesystem>
#include <fstream>
#include <sstream>

#ifdef BUGLE_ENABLE
TEST_CASE( "file logger writes jsonl and cbor sequences", "[json]" ) {
    for ( const auto format : { bugle::JsonLogger::Format::Lines, bugle::JsonLogger::Format::Cbor } ) {
        //  A filename without a directory used to throw in open().
        const std::filesystem::path path = format == bugle::JsonLogger::Format::Lines ? "test-letters.jsonl" : "test-letters.cborseq";
        {
            bugle::PostOffice office;
            auto sink = std::make_shared<bugle::JsonLogger>();
            REQUIRE( sink->open( path.string(), format ) );
            office.addObserver( sink );
            office.post( "first", { "info" }, {{ "answer", 42 }} );
            office.post( "second" );
            office.flush();
            office.removeObserver( sink );
        }
        std::ifstream input( path, std::ios::binary );
        const std::string bytes( (std::istreambuf_iterator<char>( input )), {} );
        if ( format == bugle::JsonLogger::Format::Lines ) {
            std::istringstream lines( bytes );
            std::string line;
            REQUIRE( bool( std::getline( lines, line ) ) );
            const auto first = nlohmann::json::parse( line );
            REQUIRE( first.at( "attributes" ).at( "answer" ) == 42 );
            REQUIRE( bool( std::getline( lines, line ) ) );
            REQUIRE( nlohmann::json::parse( line ).at( "message" ) == "second" );
            REQUIRE_FALSE( bool( std::getline( lines, line ) ) );
        } else {
            //  Locate the first object boundary by attempting strict decoding.
            size_t boundary = 0;
            for ( size_t i = 1; i < bytes.size(); ++i ) {
                const auto value = nlohmann::json::from_cbor( bytes.begin(), bytes.begin() + i, true, false );
                if ( ! value.is_discarded() ) {
                    REQUIRE( value.at( "message" ) == "first" );
                    REQUIRE( value.at( "attributes" ).at( "answer" ) == 42 );
                    boundary = i;
                    break;
                }
            }
            REQUIRE( boundary > 0 );
            REQUIRE( nlohmann::json::from_cbor( bytes.begin() + boundary, bytes.end() ).at( "message" ) == "second" );
        }
        input.close();
        std::filesystem::remove( path );
    }
}
#endif
