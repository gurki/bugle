#pragma once

#include <string>
#include <memory>
#include <functional>

namespace bugle {


using FilterPtr = std::shared_ptr<class Filter>;
using FilterUPtr = std::unique_ptr<class Filter>;

class Letter;


struct Filter
{
    std::function<bool(const Letter&)> matches {};

    static FilterPtr fromString( const std::string& expression );
};


}   //  ::bugle