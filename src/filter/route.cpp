#include "bugle/filter/route.h"
#include "bugle/core/letter.h"

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
bool Route::matches( const Letter& letter ) const {
    return true;
}


}   //  ::bugle