#pragma once

#include <memory>


namespace mc {


class Message;
using ObserverRef = std::weak_ptr<class Observer>;
using ObserverPtr = std::shared_ptr<class Observer>;


class Observer
{
    public:

        virtual ~Observer() {}
        virtual void notify( const Message& message ) = 0;
};


}   //  ::mc

