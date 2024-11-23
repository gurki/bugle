#pragma once

#include <memory>

namespace bugle {


struct Letter;
using RecipientRef = std::weak_ptr<struct Recipient>;
using RecipientPtr = std::shared_ptr<struct Recipient>;


struct Recipient {
    virtual ~Recipient() {}
    virtual void receive( const Letter& ) = 0;
};


}   //  ::bugle

