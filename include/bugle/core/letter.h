#pragma once

#include "bugle/utility/timestamp.h"
#include "bugle/core/types.h"

#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#include <memory>
#include <source_location>

namespace bugle {


using LetterUPtr = std::unique_ptr<struct Letter>;
using LetterPtr = std::shared_ptr<struct Letter>;


struct Letter
{
    Timestamp timestamp;
    std::thread::id thread;
    int level;
    std::source_location location;
    std::string message;
    tags_t tags;
    attributes_t attributes;

    Letter(
        const std::string& message = {},
        const tags_t& tags = {},
        const attributes_t& attributes = {},
        const std::source_location& location = std::source_location::current(),
        const int level = -1
    );

    std::string file() const { return location.file_name(); }
    std::string function() const { return location.function_name(); }
    int line() const { return location.line(); }
    int column() const { return location.column(); }

    std::string locationInfo() const;
    std::string threadInfo() const;
    std::string functionInfo() const;
    std::string fileInfo() const;
    bool hasTags() const { return ! tags.empty(); }
};


void to_json( nlohmann::json&, const Letter& );
void from_json( const nlohmann::json&, Letter& );



}   //  ::bugle
