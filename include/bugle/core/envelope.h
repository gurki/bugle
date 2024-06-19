#pragma once

#include "bugle/core/letter.h"

namespace bugle {

class PostOffice;


struct Envelope : public Letter
{
    const std::reference_wrapper<PostOffice> office;
    bool open;
    Timestamp closedAt;

    Envelope(
        const std::reference_wrapper<PostOffice>& office,
        const std::string& title = {},
        const std::source_location& location = std::source_location::current()
    );

    ~Envelope();

    void close();
    uint64_t durationUs() const;
};


}   //  ::bugle
