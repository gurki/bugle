#include "bugle/filter/address.h"
#include "bugle/core/letter.h"

#include <ranges>

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