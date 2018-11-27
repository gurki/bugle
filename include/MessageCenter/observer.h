#pragma once

#include <memory>


namespace mc {


using MessageRef = std::weak_ptr<class Message>;
using MessagePtr = std::shared_ptr<class Message>;
using ObserverRef = std::weak_ptr<class Observer>;
using ObserverPtr = std::shared_ptr<class Observer>;


class Observer
{
    public:

        virtual ~Observer() {}
        virtual void notify( const MessagePtr& messagePtr ) = 0;
};


}   //  mc::

