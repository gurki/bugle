#pragma once

#include "bugle/core/observer.h"
#include "bugle/format/formatter.h"
#include <mutex>

namespace bugle {


class ConsoleLogger : public Observer
{
    public:

        ConsoleLogger();

        void setFormatter( const FormatterPtr& formatter );
        virtual void notify( const Message& messagePtr );

    private:

        std::mutex mutex_;  //  avoid mingling text by writing from multiple threads
        FormatterPtr formatter_ = nullptr;
};


}   //  ::bugle
