#pragma once

#include <memory>

namespace bugle {


class Letter;
using ObserverRef = std::weak_ptr<class Observer>;
using ObserverPtr = std::shared_ptr<class Observer>;


struct Observer {
    virtual ~Observer() {}
    virtual void notify( const Letter& message ) = 0;
};


}   //  ::bugle

