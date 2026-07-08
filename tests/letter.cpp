#include <catch2/catch_test_macros.hpp>
#include <bugle/core/letter.h>
#include <bugle/core/postoffice.h>

#include <thread>


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "letters serialize to json", "[letter]" )
{
    SECTION( "empty fields are omitted" )
    {
        const bugle::Letter letter;
        const nlohmann::json json = letter;

        REQUIRE_FALSE( json.contains( "message" ) );
        REQUIRE_FALSE( json.contains( "tags" ) );
        REQUIRE_FALSE( json.contains( "attributes" ) );

        REQUIRE( json.contains( "timestamp" ) );
        REQUIRE( json.contains( "thread" ) );
        REQUIRE( json.contains( "file" ) );
    }

    SECTION( "non-empty fields are written" )
    {
        const bugle::Letter letter( "hello", { "info" }, {{ "value", 1 }} );
        const nlohmann::json json = letter;

        REQUIRE( json.at( "message" ) == "hello" );
        REQUIRE( json.at( "tags" ).size() == 1 );
        REQUIRE( json.at( "attributes" ).at( "value" ) == 1 );
    }
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "thread names", "[letter][postoffice]" )
{
    //  runs on a spawned thread so the main thread never gets a name,
    //  keeping the serialization tests above deterministic

    SECTION( "set and get by id" )
    {
        std::thread::id id;
        std::thread( [ &id ](){ id = std::this_thread::get_id(); } ).join();

        REQUIRE_FALSE( bugle::PostOffice::threadName( id ).has_value() );

        bugle::PostOffice::setThreadName( "worker", id );
        REQUIRE( bugle::PostOffice::threadName( id ) == "worker" );

        //  explicit set overwrites
        bugle::PostOffice::setThreadName( "renamed", id );
        REQUIRE( bugle::PostOffice::threadName( id ) == "renamed" );
    }

    SECTION( "register keeps an existing name" )
    {
        std::thread( [](){
            auto& po = bugle::PostOffice::instance();

            po.registerThread();
            const auto registered = bugle::PostOffice::threadName();
            REQUIRE( registered.has_value() );

            //  a second register must not rename
            bugle::PostOffice::setThreadName( "explicit" );
            po.registerThread();
            REQUIRE( bugle::PostOffice::threadName() == "explicit" );
        }).join();
    }

    SECTION( "serialized into letters" )
    {
        nlohmann::json json;

        std::thread( [ &json ](){
            bugle::PostOffice::setThreadName( "serializer" );
            json = bugle::Letter( "named" );
        }).join();

        REQUIRE( json.at( "threadName" ) == "serializer" );
    }
}
