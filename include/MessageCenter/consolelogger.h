#pragma once

#include "messagecenter/messageobserver.h"
#include "messagecenter/messageformatter.h"


namespace mc {


class ConsoleLogger : public MessageObserver
{
    public:

        ConsoleLogger();

        void setFormatter( const MessageFormatterPtr& formatter );

        virtual void notify( const Message& message );

    private:

        void printSystemInfo() const;

        MessageFormatterPtr formatter_ = nullptr;
};


}   //  mc::
