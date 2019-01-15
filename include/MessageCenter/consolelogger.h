#pragma once

#include "messagecenter/observer.h"
#include "messagecenter/formatter.h"
#include <mutex>


namespace mc {


class ConsoleLogger : public Observer
{
    public:

        ConsoleLogger();

        void setFormatter( const FormatterPtr& formatter );

        virtual void notify( const MessagePtr& messagePtr );

    private:

        void printSystemInfo() const;

        FormatterPtr formatter_ = nullptr;

        static std::mutex ostreamMutex_;
};


}   //  mc::
