#include <bugle/bugle.h>
#include <future>


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    auto& po = bugle::PostOffice::instance();
    auto cl = std::make_shared<bugle::ConsoleLogger>();
    auto filter = std::make_shared<bugle::TagFilter>( bugle::TagFilter( { "info" }, { "debug" } ) );

    //  (:?^\s*(:?([\w\d:<>=!.,-_]*)\s*)*\n?$)*
    //  (\S*)

    // bugle::Route route = R"(
    //     file:main.cpp
    //     tags:info,debug,!lambda,!scope
    //     attributes:envelope.duration>10 level>=1
    //     attributes:envelope
    //     timestamp>2024-04-25T00:00:00 timestamp<2024-04-25T23:59:59
    // )";
    po.addObserver( cl );

    bugle::Envelope scope( po, "main" );

    po.post( "hallo", { "info", "debug" }, {
        { "value", 5 },
        { "position", { 0.5f, 0.2f, 0.1f } }
    });

    auto fn = [&]( int val, const std::string& name ) -> float {
        bugle::Envelope scope( po, name );
        std::this_thread::sleep_for( std::chrono::milliseconds( val ) );
        po.post( "yo, we need to go deeper", { "info" }, {{ "value", val }} );
        return -1.f;
    };

    fn( 100, "sync" );

    const auto future = std::async( fn, 200, "async" );
    future.wait();

    scope.close();
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
}