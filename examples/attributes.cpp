#include <bugle/core/postoffice.h>
#include <bugle/sinks/consolelogger.h>

#include <future>


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    auto& po = bugle::PostOffice::instance();
    auto cl = std::make_shared<bugle::ConsoleLogger>();
    po.addObserver( cl );

    bugle::Envelope scope( po, "main" );

    po.post( "hallo", { "info", "debug" }, {
        { "value", 5 },
        { "position", { 0.5f, 0.2f, 0.1f } }
    });

    auto fn = [&]( int val ) {
        bugle::Envelope scope( po, "lambda" );
        std::this_thread::sleep_for( std::chrono::milliseconds( val ) );
        po.post( "yo, lambda", {}, {{ "value", val }} );
    };

    fn( 100 );

    const auto future = std::async( fn, 200 );
    future.wait();

    scope.close();
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
}