#pragma once

#include "bugle/core/recipient.h"
#include "bugle/format/formatter.h"
#include <mutex>

namespace bugle {


class ConsoleLogger : public Recipient
{
    public:

        ConsoleLogger();

        void setFormatter( const FormatterPtr& );
        virtual void receive( const Letter& );

    private:

        FormatterPtr formatter_ = nullptr;
};


}   //  ::bugle
