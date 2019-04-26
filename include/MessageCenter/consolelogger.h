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
        void printSystemInfo() const;
        virtual void notify( const Message& messagePtr );

    private:

        std::mutex mutex_;
        FormatterPtr formatter_ = nullptr;
};


}   //  ::mc
