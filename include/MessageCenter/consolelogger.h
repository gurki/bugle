#pragma once

#include "messagecenter/observer.h"
#include "messagecenter/formatter.h"


namespace mc {


class ConsoleLogger : public Observer
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
