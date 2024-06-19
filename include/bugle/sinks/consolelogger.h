#pragma once

#include "bugle/core/recipient.h"
#include "bugle/format/formatter.h"
#include <mutex>

namespace bugle {


class ConsoleLogger : public Recipient
{
    public:

        ConsoleLogger();

        void setFormatter( const FormatterPtr& formatter );
        virtual void receive( const Letter& messagePtr );

    private:

        std::mutex mutex_;  //  avoid mingling text by writing from multiple threads
        FormatterPtr formatter_ = nullptr;
};


}   //  ::bugle
