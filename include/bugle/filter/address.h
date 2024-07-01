#pragma once

#include "bugle/core/filter.h"
#include "bugle/core/letter.h"
#include <vector>

namespace bugle {


struct AddressLine {
    Filter variable;
    bool negate = false;
};

struct LineInfo {
    std::string_view type;
    std::string_view variable;
    bool negate;
};



struct Address : public Filter
{
    std::vector<AddressLine> lines;

    Address()
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

    static constexpr auto splitConjuncts( std::string_view expression )
    {
        auto lines = expression | std::views::split( ' ' );

        constexpr auto parseLine = []( auto line ) -> LineInfo
        {
            std::string_view sv( line );
            const size_t id = sv.find_first_of( ':' );

            // static_assert( id != std::string_view::npos );

            std::string_view type = sv.substr( 0, id );
            std::string_view variable = sv.substr( id + 1 );

            // static_assert( ! type.empty() );
            // static_assert( ! variable.empty() );

            bool negate = false;

            if ( type.starts_with( '!' ) ) {
                negate = true;
                type = type.substr( 1 );
            }

            return { type, variable, negate };
        };

        return lines | std::views::transform( parseLine );
    }

};


}   //  ::bugle