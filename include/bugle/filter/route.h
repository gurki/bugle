#pragma once

#include "bugle/core/filter.h"
#include "bugle/core/letter.h"

#include <vector>
#include <ranges>

namespace bugle {


class Letter;

struct Route : public Filter
{
    std::vector<Filter> addresses;

    Route() {
        matches = [ this ]( const Letter& letter ) {
            return std::ranges::any_of( addresses, [ &letter ]( const auto& address ) {
                return address.matches( letter );
            });
        };
    }
};


}   //  ::bugle