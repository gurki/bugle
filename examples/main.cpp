//  kitchen-sink overview: console + json + profiler recipients, system
//  banners, tagged and structured messages, and nested envelopes.
//  see the sibling examples for focused demos of each feature.

#include <bugle/bugle.h>

#include <future>
#include <thread>


////////////////////////////////////////////////////////////////////////////////
void child() {
    bugle::Envelope scope( PO );
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
}


////////////////////////////////////////////////////////////////////////////////
int main()
{
    auto& po = bugle::PostOffice::instance();

    auto console = std::make_shared<bugle::ConsoleLogger>();
    auto json = std::make_shared<bugle::JsonLogger>();
    auto profiler = std::make_shared<bugle::Profiler>();

    json->open();
    profiler->open();

    po.addObserver( console );
    po.addObserver( json );
    po.addObserver( profiler, bugle::Filter::fromString( "tag:envelope" ) );

    //  system banners
    po.post( {}, { "session", "system" }, nlohmann::json( bugle::SessionInfo::current() ) );
    po.post( {}, { "build", "system" }, nlohmann::json( bugle::BuildInfo::current() ) );

#ifdef BUGLE_HAS_GPU
    po.post( {}, { "gpu", "system" }, nlohmann::json( bugle::GpuInfo::current() ) );
#endif

    bugle::Envelope scope( po );

    //  nested scopes with durations
    {
        bugle::Envelope outer( po, "outer" );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        bugle::Envelope inner( po, "inner" );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    //  tagged messages with structured attributes
    po.post( "hallo", { "info", "debug" }, {
        { "value", 5 },
        { "position", { 0.5f, 0.2f, 0.1f } }
    });

    po.post( "incoming", { "measurement" }, {
        { "temperature", 10 }
    });

    //  per-thread indentation
    auto fn = [ &po ]( int ms, const std::string& name ) {
        bugle::Envelope scope( po, name );
        std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
        po.post( "go deeper", { "info" }, {{ "value", ms }} );
    };

    fn( 100, "sync" );

    const auto future = std::async( fn, 200, "async" );
    future.wait();

    child();

    scope.close();
    po.flush();
}
