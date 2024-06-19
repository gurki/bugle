#pragma once

#include <string>
#include <memory>

namespace bugle {

using FilterPtr = std::shared_ptr<class Filter>;
using FilterUPtr = std::unique_ptr<class Filter>;

class Letter;

struct Filter
{
    virtual ~Filter() {};

    //  empty default (matches any letter)
    virtual bool matches( const Letter& ) const = 0;
};


}   //  ::bugle