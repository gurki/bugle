#include "bugle/filter/route.h"

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
bool Route::matches( const Letter& letter ) const {
    return std::ranges::any_of( addresses, [ &letter ]( const auto& address ) {
        return address->matches( letter );
    });
}


}   //  ::bugle