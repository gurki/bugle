#pragma once

#include "message.h"


class MessageObserver
{
    public:

        virtual ~MessageObserver() {}
        virtual void notify( const Message& message ) = 0;
};
