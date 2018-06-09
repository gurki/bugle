#pragma once

#include "message.h"
#include <memory>


using MessageObserverRef = std::weak_ptr<class MessageObserver>;
using MessageObserverPtr = std::shared_ptr<class MessageObserver>;


class MessageObserver
{
    public:

        virtual ~MessageObserver() {}
        virtual void notify( const Message& message ) = 0;
};


