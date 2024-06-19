#pragma once

#include "bugle/core/observer.h"
#include <mutex>

namespace bugle {


struct Profiler : public Recipient
{
    virtual void receive( const Letter& message ) override;

    private:

        std::mutex mutex_;  //  avoid mingling text by writing from multiple threads

};


}   //  ::bugle
