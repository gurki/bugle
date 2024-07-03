#pragma once

#include <string>
#include <memory>
#include <functional>

namespace bugle {

class Letter;


struct Filter {
    std::function<bool(const Letter&)> matches = {};
};


}   //  ::bugle