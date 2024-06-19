#pragma once

#include <chrono>
#include <format>

namespace bugle {


inline std::string durationInfo( const int64_t us )
{
    if ( us < 1000 ) {
        return std::format( "{:3d} us", us );
    }

    if ( us < 1000000 ) {
        return std::format( "{:3d} ms", us / 1000 );
    }

    const float s = us / 1000000.f;

    if ( s < 10.f ) {
        return std::format( "{0:.1f} s", s );
    }

    return std::format( "{:.0f} s", s );
}


}   //  ::bugle