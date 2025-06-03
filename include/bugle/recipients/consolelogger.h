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

        void h1( const std::string& title );
        void h2( const std::string& title );
        void kv( const std::string& key, const auto& val, const bool closeInner = false, const bool closeOuter = false );

        static const uint8_t tx1_ = 254;
        static const uint8_t tx2_ = 250;
        static const uint8_t tx3_ = 246;
        
        void logBuild( const Letter& );    
        void logSession( const Letter& );    
        void logAttributes( const attributes_t& );    
        void logEnvelope( const Letter& );
        FormatterPtr formatter_ = nullptr;
};


}   //  ::bugle
