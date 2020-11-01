#pragma once

#include <bugle/core/messagecenter.h>
#include <bugle/core/scope.h>
#include <iostream>

namespace bugle::st {


void simpleEnter() {
    BUGLE_SCOPE();
    using namespace std::chrono_literals;
    // std::this_thread::sleep_for( 1s );
    bglp( "much text, such wow" );
}


void taggedEnter()  {
    bgls( "⭐" );
    using namespace std::chrono_literals;
    // std::this_thread::sleep_for( 1s );
    bglp( "a message" );
}


void nestedEnter()  {
    bgls( "🚩" );
    simpleEnter();
    bglp( "i love tags", "seriously" );
}


void deeperNesting() {
    bgls();
    nestedEnter();
    bglp( "overkill", { "triplekill", "quadrakill" } );
}


}   //  ::bugle::st