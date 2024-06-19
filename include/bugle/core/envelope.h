#pragma once

#include "bugle/utility/timestamp.h"
#include <source_location>
#include <thread>

namespace bugle {

class PostOffice;


struct Envelope
{
    const std::reference_wrapper<PostOffice> office;
    std::string title;
    std::source_location location;
    std::thread::id thread;
    bool open;
    Timestamp openedAt;
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
