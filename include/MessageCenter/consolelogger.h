#pragma once

#include "messageobserver.h"
//#include "messageformatter.h"

namespace mc {


class ConsoleLogger : public MessageObserver
{
    public:

        ConsoleLogger();

//        void setFormatter( MessageFormatter* formatter );

        virtual void notify( const Message& message );

    private:

        void printSystemInfo() const;

//        MessageFormatter* formatter_ = nullptr;
};


}   //  mc::
