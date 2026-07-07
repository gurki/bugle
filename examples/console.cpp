//  console logger demo: tags and colors, structured attributes,
//  and rai envelopes with per-thread indentation

#include <bugle/bugle.h>

#include <future>
#include <thread>


////////////////////////////////////////////////////////////////////////////////
int main()
{
    auto& po = bugle::PostOffice::instance();

    auto console = std::make_shared<bugle::ConsoleLogger>();
    po.addObserver( console );

    //  tags colorize and categorize; info, success, warning and error come
    //  with pre-defined colors
    po.post( "plain text" );
    po.post( "connection established", { "network", "success" } );
    po.post( "battery low", { "power", "warning" } );
    po.post( "sensor offline", { "sensor", "error" } );

    //  convenience shorthands
    po.memo( "just a message" );
    po.card({ "checkpoint", "info" });

    //  structured attributes render inline
    po.post( "incoming", { "measurement" }, {
        { "temperature", 21.4f },
        { "humidity", 0.63f },
        { "position", { 0.5f, 0.2f, 0.1f } }
    });

    //  envelopes indent everything in their scope and log their duration
    {
        bugle::Envelope startup( po, "startup" );

        po.post( "loading config", { "info" } );
        std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

        {
            bugle::Envelope inner( po, "warmup" );
            po.post( "priming caches", { "info" } );
            std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
        }
    }

    //  each thread tracks its own indentation
    const auto worker = [ &po ]( const std::string& name, const int ms ) {
        bugle::Envelope scope( po, name );
        std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
        po.post( "work done", { "info" }, {{ "tookMs", ms }} );
    };

    auto future = std::async( worker, "async", 100 );
    worker( "sync", 50 );
    future.wait();

    po.flush();
}
