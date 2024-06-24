#include "bugle/core/filter.h"
#include "bugle/filter/lines.h"
#include "bugle/filter/address.h"
#include "bugle/filter/route.h"

#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace bugle {


// bugle::Route route = R"(
//     message:main
//     tag:info !tag:debug !attribute:value>100
//     attribute:position
// )";


////////////////////////////////////////////////////////////////////////////////
FilterPtr Filter::fromString( const std::string& expression )
{
    constexpr auto splitAddresses = std::views::split( '\n' );
    constexpr auto splitLines = std::views::split( ' ' );

    const auto addresses = expression | splitAddresses;
    // const auto some = addresses | std::views::transform( splitLines );
    return nullptr;
}


}   //  ::bugle