#pragma once

#include "bugle/core/recipient.h"
#include "bugle/core/letter.h"
#include <fstream>

namespace bugle {

struct Letter;


struct JsonLogger : public Recipient
{
    ~JsonLogger();
    bool open( const std::string& filename = {} );

    virtual void receive( const Letter& ) override;

    private:

        std::ofstream fout_;
};


}   //  ::bugle
