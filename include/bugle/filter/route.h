#pragma once

#include "bugle/core/filter.h"
#include <vector>

namespace bugle {


class Letter;

struct Route : public Filter {
    std::vector<FilterPtr> addresses;
    virtual bool matches( const Letter& ) const override;
};


}   //  ::bugle