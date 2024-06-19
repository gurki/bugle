#pragma once

#include "bugle/core/filter.h"

namespace bugle {


class Letter;

struct Route : public Filter {
    virtual bool matches( const Letter& ) const override;
};


}   //  ::bugle