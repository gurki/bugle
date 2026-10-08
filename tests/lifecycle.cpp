#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>
#include <future>
#include <stdexcept>
#include <type_traits>

#ifdef BUGLE_ENABLE
namespace {
struct Callback : bugle::Recipient {
    std::function<void( const bugle::Letter& )> action;
    explicit Callback( decltype(action) fn ) : action( std::move( fn ) ) {}
    void receive( const bugle::Letter& letter ) override { action( letter ); }
};
}

TEST_CASE( "callbacks can change observers and post recursively", "[postoffice]" ) {
    bugle::PostOffice office;
    int nextCount = 0;
    auto next = std::make_shared<Callback>( [&]( const auto& ) { ++nextCount; } );
    std::shared_ptr<Callback> first;
    first = std::make_shared<Callback>( [&]( const auto& ) {
        office.removeObserver( first );
        office.addObserver( next );
        office.post( "follow-up" );
    } );
    office.addObserver( first );
    office.post( "initial" );
    office.flush();
    REQUIRE( nextCount == 1 );
}

TEST_CASE( "callback and predicate exceptions do not stop delivery", "[postoffice]" ) {
    bugle::PostOffice office;
    auto flushing = std::make_shared<Callback>( [&]( const auto& ) { office.flush(); } );
    auto throwing = std::make_shared<Callback>( []( const auto& ) { throw std::runtime_error( "sink" ); } );
    auto filtered = std::make_shared<Callback>( []( const auto& ) {} );
    int count = 0;
    auto healthy = std::make_shared<Callback>( [&]( const auto& ) { ++count; } );
    office.addObserver( flushing );
    office.addObserver( throwing );
    office.addObserver( filtered, bugle::Filter { []( const auto& ) -> bool { throw std::runtime_error( "filter" ); } } );
    office.addObserver( healthy );
    office.post( "one" );
    office.post( "two" );
    office.flush();
    REQUIRE( count == 2 );
    REQUIRE( office.dispatchFailures() == 6 );
}

TEST_CASE( "reregistering an observer clears its previous filter", "[postoffice]" ) {
    bugle::PostOffice office;
    int count = 0;
    auto sink = std::make_shared<Callback>( [&]( const auto& ) { ++count; } );
    office.addObserver( sink, bugle::TagFilter( "info" ) );
    office.post( "filtered", { "debug" } );
    office.flush();
    REQUIRE( count == 0 );
    office.addObserver( sink );
    office.post( "all", { "debug" } );
    office.flush();
    REQUIRE( count == 1 );
    bugle::RecipientRef expired = sink;
    sink.reset();
    office.removeObserver( expired );
    office.post( "expired" );
    office.flush();
    REQUIRE( office.dispatchFailures() == 0 );
}

TEST_CASE( "dispatch preserves insertion order for out of order timestamps", "[postoffice]" ) {
    bugle::PostOffice office;
    std::vector<std::string> messages;
    auto sink = std::make_shared<Callback>( [&]( const auto& letter ) { messages.push_back( letter.message ); } );
    office.addObserver( sink );
    bugle::Letter first( "first" ), second( "second" );
    first.timestamp += std::chrono::hours( 1 );
    office.post( std::move( first ) );
    office.post( std::move( second ) );
    office.flush();
    REQUIRE( messages == std::vector<std::string> { "first", "second" } );
}
#endif
