#pragma once

#include "bugle/core/filter.h"
#include "bugle/core/letter.h"
#include <vector>

namespace bugle {


struct Line {
    Filter variable;
    bool negate = false;
};


struct Address : public Filter
{
    Address( const std::vector<Line>& _lines )
    {
        matches = [ _lines ]( const Letter& letter )
        {
            for ( const auto& line : _lines ) {
                if ( line.variable.matches( letter ) == line.negate ) {
                    return false;
                }
            }

            return true;
        };
    }
};


}   //  ::bugle