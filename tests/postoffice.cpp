#include <catch2/catch_test_macros.hpp>
#include <bugle/bugle.h>

#include <atomic>
#include <thread>
#include <vector>

#ifdef BUGLE_ENABLE

namespace {

struct CountingRecipient : bugle::Recipient
{
    std::atomic_int count = 0;
    void receive( const bugle::Letter& ) override { count++; }
};

}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "flush delivers all posted letters", "[postoffice]" )
{
    bugle::PostOffice po;
    auto recipient = std::make_shared<CountingRecipient>();
    po.addObserver( recipient );

    constexpr int kThreads = 4;
    constexpr int kLettersPerThread = 256;

    std::vector<std::thread> threads;

    for ( int t = 0; t < kThreads; t++ ) {
        threads.emplace_back( [ &po ](){
            for ( int i = 0; i < kLettersPerThread; i++ ) {
                po.post( "letter", { "test" } );
            }
        });
    }

    for ( auto& thread : threads ) {
        thread.join();
    }

    po.flush();

    REQUIRE( recipient->count == kThreads * kLettersPerThread );
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "queue drains on destruction", "[postoffice]" )
{
    auto recipient = std::make_shared<CountingRecipient>();
    constexpr int kLetters = 512;

    {
        bugle::PostOffice po;
        po.addObserver( recipient );

        for ( int i = 0; i < kLetters; i++ ) {
            po.post( "letter", { "test" } );
        }
    }   //  destructor joins the worker without an explicit flush

    REQUIRE( recipient->count == kLetters );
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "filtered observer only receives matches", "[postoffice]" )
{
    bugle::PostOffice po;

    auto all = std::make_shared<CountingRecipient>();
    auto filtered = std::make_shared<CountingRecipient>();

    po.addObserver( all );
    po.addObserver( filtered, bugle::Filter::fromString( "tag:info" ) );

    po.post( "one", { "info" } );
    po.post( "two", { "debug" } );
    po.post( "three", { "info", "debug" } );
    po.flush();

    REQUIRE( all->count == 3 );
    REQUIRE( filtered->count == 2 );
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "expired observers are dropped", "[postoffice]" )
{
    bugle::PostOffice po;
    auto recipient = std::make_shared<CountingRecipient>();

    po.addObserver( recipient );
    po.post( "before", { "test" } );
    po.flush();

    REQUIRE( recipient->count == 1 );

    recipient.reset();

    //  dispatch to an expired observer must not crash
    po.post( "after", { "test" } );
    po.flush();
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "concurrent observer changes during dispatch", "[postoffice]" )
{
    bugle::PostOffice po;
    auto recipient = std::make_shared<CountingRecipient>();
    po.addObserver( recipient );

    std::atomic_bool done = false;

    std::thread churn( [ & ](){
        while ( ! done ) {
            auto transient = std::make_shared<CountingRecipient>();
            po.addObserver( transient );
            po.removeObserver( transient );
        }
    });

    constexpr int kLetters = 1024;

    for ( int i = 0; i < kLetters; i++ ) {
        po.post( "letter", { "test" } );
    }

    po.flush();
    done = true;
    churn.join();

    REQUIRE( recipient->count == kLetters );
}


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "envelope levels reset per thread", "[postoffice]" )
{
    bugle::PostOffice po;
    const auto thread = std::this_thread::get_id();

    REQUIRE( po.level( thread ) == 0 );

    {
        bugle::Envelope outer( po, "outer" );
        REQUIRE( po.level( thread ) == 1 );

        {
            bugle::Envelope inner( po, "inner" );
            REQUIRE( po.level( thread ) == 2 );
        }

        REQUIRE( po.level( thread ) == 1 );
    }

    REQUIRE( po.level( thread ) == 0 );
    po.flush();
}

#endif  //  BUGLE_ENABLE
