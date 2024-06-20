#pragma once

#include "bugle/core/filter.h"
#include <vector>

namespace bugle {


struct AddressLine {
    FilterPtr variable = nullptr;
    bool negate = false;
};


struct Address : public Filter {
    std::vector<AddressLine> lines;
    virtual bool matches( const Letter& ) const override;
};


}   //  ::bugle