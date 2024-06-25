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

        void logBuild( const Letter& );    
        void logSession( const Letter& );    
        void logEnvelope( const Letter& );
        FormatterPtr formatter_ = nullptr;
};


}   //  ::bugle
