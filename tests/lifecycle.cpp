#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>
#include <future>
#include <stdexcept>
#include <type_traits>

TEST_CASE( "envelope duration uses a monotonic clock", "[envelope]" ) {
    STATIC_REQUIRE_FALSE( std::is_copy_constructible_v<bugle::Envelope> );
    bugle::PostOffice office;
    bugle::Envelope scope( office );
    scope.openedAt += std::chrono::hours( 24 );
    scope.close();
    const auto duration = scope.durationUs();
    REQUIRE( duration < 1000000 );
    scope.closedAt -= std::chrono::hours( 48 );
    REQUIRE( scope.durationUs() == duration );
    scope.close();
    REQUIRE( scope.durationUs() == duration );
}

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

TEST_CASE( "bounded queues block producers and count recursive overflow", "[postoffice]" ) {
    bugle::PostOffice office( 1 );
    std::promise<void> entered, release;
    auto released = release.get_future().share();
    int count = 0;
    auto sink = std::make_shared<Callback>( [&]( const bugle::Letter& letter ) {
        ++count;
        if ( letter.message == "first" ) {
            entered.set_value();
            released.wait();
            office.post( "recursive-overflow" );
        }
    } );
    office.addObserver( sink );
    office.post( "first" );
    entered.get_future().wait();
    office.post( "second" );
    auto producer = std::async( std::launch::async, [&] { office.post( "third" ); } );
    const auto status = producer.wait_for( std::chrono::milliseconds( 50 ) );
    release.set_value();
    producer.get();
    office.flush();
    REQUIRE( status == std::future_status::timeout );
    REQUIRE( count == 3 );
    REQUIRE( office.droppedLetters() == 1 );
}

TEST_CASE( "dispatch preserves insertion order for out of order timestamps", "[postoffice]" ) {
    bugle::PostOffice office( 0 );
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
#else
TEST_CASE( "disabled logging starts no dispatch and never fills the queue", "[postoffice]" ) {
    bugle::PostOffice office( 1 );
    for ( int i = 0; i < 100; ++i ) office.post( "disabled" );
    office.flush();
    REQUIRE( office.droppedLetters() == 0 );
    REQUIRE( office.dispatchFailures() == 0 );
}
#endif
