#include "bugle/filter/address.h"

#include <ranges>
#include <print>
#include <string_view>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
bool Address::matches( const Letter& letter ) const
{
    for ( const auto& line : lines ) {
        if ( line.variable->matches( letter ) == line.negate ) {
            return false;
        }
    }

    return true;
}


}   //  ::bugle