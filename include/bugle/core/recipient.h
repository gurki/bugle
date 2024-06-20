#pragma once

#include <memory>

namespace bugle {


class Letter;
using RecipientRef = std::weak_ptr<class Recipient>;
using RecipientPtr = std::shared_ptr<class Recipient>;


struct Recipient {
    virtual ~Recipient() {}
    virtual void receive( const Letter& ) = 0;
};


}   //  ::bugle

