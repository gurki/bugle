#pragma once

#include "bugle/core/filter.h"
#include "bugle/core/letter.h"

#include <vector>
#include <ranges>

namespace bugle {


struct Route : public Filter
{
    Route( const std::vector<Filter>& _addresses )
    {
        matches = [ _addresses ]( const Letter& letter ) {
            return std::ranges::any_of( _addresses,
                [ &letter ]( const auto& address ) {
                    return address.matches( letter );
                }
            );
        };
    }
};


}   //  ::bugle