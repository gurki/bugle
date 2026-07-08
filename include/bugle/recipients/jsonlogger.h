#pragma once

#include "bugle/core/recipient.h"
#include "bugle/core/letter.h"
#include <fstream>

namespace bugle {

struct Letter;


struct JsonLogger : public Recipient
{
    enum class Format {
        Lines,      //  newline-delimited json (jsonl)
        Cbor        //  binary cbor sequence
    };

    ~JsonLogger();

    bool open(
        const std::string& filename = {},
        const Format format = Format::Lines
    );

    virtual void receive( const Letter& ) override;

    private:

        Format format_ = Format::Lines;
        std::ofstream fout_;
};


}   //  ::bugle
