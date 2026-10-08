#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>
#include <filesystem>
#include <fstream>

TEST_CASE( "profiler subtracts child durations without 32 bit overflow", "[profiler]" ) {
    //  Isolate the timestamp-named output from user profiles.
    const auto previous = std::filesystem::current_path();
    const auto folder = previous / "test-profiler";
    std::filesystem::create_directories( folder );
    struct Restore {
        std::filesystem::path path;
        ~Restore() { std::filesystem::current_path( path ); }
    } restore { previous };
    std::filesystem::current_path( folder );
    {
        bugle::Profiler profiler;
        REQUIRE( profiler.open() );
        profiler.receive( bugle::Letter( "unmatched", { "envelope" }, {{ "open", false }} ) );
        profiler.receive( bugle::Letter( "outer", { "envelope" }, {{ "open", true }} ) );
        profiler.receive( bugle::Letter( "inner", { "envelope" }, {{ "open", true }} ) );
        profiler.receive( bugle::Letter( "inner", { "envelope" }, {{ "open", false }, { "duration", 3000000000LL }} ) );
        profiler.receive( bugle::Letter( "outer", { "envelope" }, {{ "open", false }, { "duration", 4000000000LL }} ) );
    }
    size_t count = 0;
    for ( const auto& entry : std::filesystem::directory_iterator( "profiles" ) ) {
        std::ifstream input( entry.path() );
        std::string line;
        REQUIRE( bool( std::getline( input, line ) ) );
        REQUIRE( line.ends_with( " 3000000000" ) );
        REQUIRE( line.find( "outer" ) < line.find( "inner" ) );
        REQUIRE( bool( std::getline( input, line ) ) );
        REQUIRE( line.ends_with( " 1000000000" ) );
        REQUIRE_FALSE( bool( std::getline( input, line ) ) );
        ++count;
    }
    REQUIRE( count == 1 );
    std::filesystem::current_path( previous );
    std::filesystem::remove_all( folder );
}
