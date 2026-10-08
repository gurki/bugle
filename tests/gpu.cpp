#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>
#include <future>
TEST_CASE( "gpu info initialization is safe across threads", "[gpu]" ) {
    auto first = std::async( std::launch::async, [] { return nlohmann::json( bugle::GpuInfo::current() ); } );
    auto second = std::async( std::launch::async, [] { return nlohmann::json( bugle::GpuInfo::current() ); } );
    const auto value = first.get();
    REQUIRE( value == second.get() );
    REQUIRE( value.contains( "_title" ) );
}
