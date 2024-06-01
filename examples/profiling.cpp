#include <bugle/core/defines.h>
#include <bugle/core/postoffice.h>
#include <bugle/sinks/profiler.h>

#include <chrono>
#include <future>
#include <print>

using namespace std::chrono_literals;


////////////////////////////////////////////////////////////////////////////////
void workerA() {
    bgls();
    BUGLE_POST( "workerA" );
    std::println( "workerA" );
    std::this_thread::sleep_for( 100ms );
}


////////////////////////////////////////////////////////////////////////////////
void workerB() {
    bgls();
    BUGLE_POST( "workerB" );
    std::println( "workerB" );
    std::this_thread::sleep_for( 50ms );
}


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    std::println( "main" );

    auto prof = std::make_shared<bugle::Profiler>();
    bgli.addObserver( prof );

    bgls();
    BUGLE_POST( "main" );
    
    auto futureA = std::async( workerA );
    workerB();
    futureA.wait();
    
    std::this_thread::sleep_for( 100ms );
}