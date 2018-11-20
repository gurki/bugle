#pragma once

#include "messagecenter/messageobserver.h"
#include "messagecenter/formatter.h"


namespace mc {


class ConsoleLogger : public MessageObserver
{
    public:

        ConsoleLogger();

        void setFormatter( const FormatterPtr& formatter );

        virtual void notify( const Message& message );

    private:

        void printSystemInfo() const;

        FormatterPtr formatter_ = nullptr;
};


}   //  mc::
