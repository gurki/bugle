#pragma once

#include "bugle/utility/timestamp.h"
#include "bugle/core/types.h"

#include <source_location>
#include <thread>

namespace bugle {

class PostOffice;


struct Envelope
{
    const std::reference_wrapper<PostOffice> office;

    std::thread::id thread;
    std::source_location location;
    tags_t tags;

    bool open;
    std::string title;
    Timestamp openedAt;
    Timestamp closedAt;

    Envelope(
        const std::reference_wrapper<PostOffice>& office,
        const std::string& title = {},
        const tags_t& tags = {},
        const std::source_location& location = std::source_location::current()
    );

    ~Envelope();

    void close();
    uint64_t durationUs() const;
};


}   //  ::bugle
