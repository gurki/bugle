#include "bugle/sinks/profiler.h"
#include "bugle/core/message.h"
#include <print>

namespace bugle {


void Profiler::receive( const Letter& message ) {
    std::scoped_lock lock( mutex_ );
    std::println( "{}", message.locationInfo() );
}


}   //  ::bugle