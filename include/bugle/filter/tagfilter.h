#pragma once

#include "bugle/core/filter.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace bugle {


struct TagFilter : public Filter
{
    std::unordered_map<std::string, bool> tags; //  key -> included

    TagFilter(
        const std::vector<std::string>& include = {},
        const std::vector<std::string>& exclude = {}
    );

    virtual bool matches( const Letter& ) const override;
};


}   //  ::bugle