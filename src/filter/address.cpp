#include "bugle/filter/address.h"
#include "bugle/core/letter.h"

#include <ranges>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
bool Address::matches( const Letter& letter ) const {
    return std::ranges::all_of( lines, [ &letter ]( const FilterPtr& line ) {
        return line->matches( letter );
    });
}


}   //  ::bugle