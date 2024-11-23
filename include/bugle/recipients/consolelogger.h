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

        void h1( const std::string& title );
        void h2( const std::string& title );
        void kv( const std::string& key, const auto& val, const bool closeInner = false, const bool closeOuter = false );

    private:

        void logBuild( const Letter& );    
        void logSession( const Letter& );    
        void logGpu( const Letter& );    
        void logEnvelope( const Letter& );
        FormatterPtr formatter_ = nullptr;    
        
        static const auto tx = 254;
        static const auto tx2 = 250;
        static const auto tx3 = 246;
};


}   //  ::bugle
