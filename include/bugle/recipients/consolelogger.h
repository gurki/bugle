#pragma once

#include "bugle/core/recipient.h"
#include "bugle/format/formatter.h"

namespace bugle {


class ConsoleLogger : public Recipient
{
    public:

        ConsoleLogger();

        void setFormatter( const FormatterPtr& );
        virtual void receive( const Letter& );

    private:

        void logEnvelope( const Letter& );
        void logBanner( const Letter& );

        FormatterPtr formatter_ = nullptr;
};


}   //  ::bugle
