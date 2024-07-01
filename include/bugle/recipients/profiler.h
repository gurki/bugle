#pragma once

#include "bugle/core/recipient.h"
#include "bugle/core/letter.h"

#include <unordered_map>
#include <fstream>
#include <queue>

namespace bugle {


struct Profiler : public Recipient
{
    ~Profiler();
    virtual void receive( const Letter& ) override;

    bool open();
    void append( const std::string& line );

    private:

        std::unordered_map< std::thread::id, std::vector<std::string> > stacks_;
        std::unordered_map< std::thread::id, std::vector<int> > offloads_;
        std::queue<std::string> buffer_;
        std::ofstream fout_;
};


}   //  ::bugle
