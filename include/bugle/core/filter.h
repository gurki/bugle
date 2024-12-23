#pragma once

#include <functional>
#include <string_view>

namespace bugle {

struct Letter;


struct Filter {
    std::function<bool(const Letter&)> matches = {};
    static Filter fromString( std::string_view expression );
};


}   //  ::bugle