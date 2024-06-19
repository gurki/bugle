#pragma once

#include "bugle/core/letter.h"

namespace bugle {

class PostOffice;


struct Scope : public Letter
{
    const std::reference_wrapper<PostOffice> office;

    Scope(
        const std::reference_wrapper<PostOffice>& office,
        const std::string& title = {},
        const std::source_location& location = std::source_location::current()
    );

    ~Scope();

    void close();
    uint64_t durationUs() const;

    private:

        bool open_ = true;
        Timestamp end_;
};


}   //  ::bugle
