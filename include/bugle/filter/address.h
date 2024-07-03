#pragma once

#include "bugle/core/filter.h"
#include "bugle/core/letter.h"
#include <vector>

namespace bugle {


struct AddressLine {
    Filter variable;
    bool negate = false;
};


struct Address : public Filter
{
    std::vector<AddressLine> lines;

    Address( const std::vector<AddressLine>& _lines ) :
        lines( _lines )
    {
        matches = [ this ]( const Letter& letter )
        {
            for ( const auto& line : lines ) {
                if ( line.variable.matches( letter ) == line.negate ) {
                    return false;
                }
            }

            return true;
        };
    }
};


}   //  ::bugle