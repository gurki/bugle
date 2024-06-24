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


////////////////////////////////////////////////////////////////////////////////
FilterPtr Address::fromString( const std::string& expression )
{
    const auto lines = expression | std::views::split( ' ' );

    // std::ranges::for_each( lines, []( auto&& line ) {
    //     std::string str_line( line.begin(), line.end() );
    //     std::println( "{}", str_line );
    // });

    return nullptr;
}


}   //  ::bugle