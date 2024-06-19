#pragma once

#include "bugle/core/filter.h"
#include <vector>

namespace bugle {


class Letter;

struct Address : public Filter {
    std::vector<FilterPtr> lines;
    virtual bool matches( const Letter& ) const override;
};


}   //  ::bugle