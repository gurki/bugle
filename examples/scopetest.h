#pragma once

#include <bugle/core/messagecenter.h>
#include <bugle/core/scope.h>
#include <iostream>

namespace bugle::st {


void simpleEnter() {
    MC_SCOPE();
    using namespace std::chrono_literals;
    // std::this_thread::sleep_for( 1s );
    mcp( "much text, such wow" );
}


void taggedEnter()  {
    mcs( "⭐" );
    using namespace std::chrono_literals;
    // std::this_thread::sleep_for( 1s );
    mcp( "a message" );
}


void nestedEnter()  {
    mcs( "🚩" );
    simpleEnter();
    mcp( "i love tags", "seriously" );
}


void deeperNesting() {
    mcs();
    nestedEnter();
    mcp( "overkill", { "triplekill", "quadrakill" } );
}


}   //  ::bugle::st