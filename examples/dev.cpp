#include <bugle/core/filteritem.h>
#include <bugle/core/message.h>
#include <bugle/core/messagecenter.h>
#include <bugle/core/scope.h>
#include <bugle/core/tags.h>
#include <bugle/format/colortable.h>
#include <bugle/format/formatter.h>
#include <bugle/format/theme.h>
#include <bugle/sinks/consolelogger.h>
#include <bugle/utility/datetime.h>

#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

using namespace bugle;
using namespace std::chrono_literals;
using namespace std::string_literals;

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    auto clog = std::make_shared<ConsoleLogger>();
    bgli.addObserver( clog );

    static const nlohmann::json obj = {
        { "pi", 3.141 },
        { "happy", true },
        { "nothing", nullptr },
        { "list", { 1, 0, 2 } },
        { "object", {
            { "name", "Niels" },
            { "value", 42.99 },
            { "currency", "USD" },
            { "answer", {
                { "everything", 42 }
            }}
        }}
    };

    //  different parameters
    bglp( "message only" );
    bglp( "single tag", "awesometag" );
    bglp( "multiple tags", { "imatag", "metoo" } );
    bglp( "array", { "a", 4 } );
    bglp( "object", {{ "a", 4 }} );

    bglf(
        { "debug", "info", "warning" },
        "this is for testing only {} {} {}",
        1.f, 7, true
    );

    bgln( "this is for testing only {} {} {}", 1.f, 7, true );
    std::this_thread::sleep_for( 10ms );
}