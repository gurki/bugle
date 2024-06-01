#pragma once

#include "bugle/core/observer.h"
#include <mutex>

namespace bugle {


class ConsoleLogger : public Observer
{
    public:

        ConsoleLogger();

        virtual void notify( const Message& message );

    private:

        std::mutex mutex_;  //  avoid mingling text by writing from multiple threads
};


}   //  ::bugle
