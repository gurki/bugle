//  profiler demo: envelopes double as profiling scopes. the profiler
//  recipient turns matching open/close pairs into collapsed-stack lines
//  ('profiles/<timestamp>_collapsed.txt'), ready for flamegraph tools
//  like speedscope.app

#include <bugle/bugle.h>

#include <future>
#include <print>
#include <thread>


////////////////////////////////////////////////////////////////////////////////
static void simulate( bugle::PostOffice& po, const int ms ) {
    bugle::Envelope scope( po );
    std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
}


////////////////////////////////////////////////////////////////////////////////
int main()
{
    auto& po = bugle::PostOffice::instance();

    auto profiler = std::make_shared<bugle::Profiler>();

    if ( ! profiler->open() ) {
        std::println( "couldn't open profile output file" );
        return 1;
    }

    //  the profiler only cares about envelope letters
    po.addObserver( profiler, bugle::TagFilter( "envelope" ) );

    {
        bugle::Envelope frame( po, "frame" );

        simulate( po, 30 );     //  nested scope

        {
            bugle::Envelope physics( po, "physics" );
            simulate( po, 20 );
            simulate( po, 10 );
        }

        //  parallel scopes profile per thread
        auto future = std::async( [ &po ]() {
            bugle::Envelope upload( po, "upload" );
            std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
        });

        future.wait();
    }

    po.flush();
    std::println( "wrote collapsed stacks to 'profiles/'" );
}
