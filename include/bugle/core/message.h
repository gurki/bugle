#pragma once

#include "bugle/utility/datetime.h"
#include "bugle/core/tags.h"

#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#include <memory>

namespace bugle {


using MessageUPtr = std::unique_ptr<class Message>;
using MessagePtr = std::shared_ptr<class Message>;


class Message
{
    public:

        Message();

        Message(
            const char* file,
            const char* function,
            const int line,
            const int level,
            const std::thread::id& threadId,
            const std::string& content,
            const tags_t& tags = {}
        );

        const DateTime& timestamp() const { return datetime_; }
        const uint64_t& threadId() const { return threadId_; }
        const std::string& file() const { return file_; }
        const std::string& function() const { return function_; }
        int line() const { return line_; }
        int level() const { return level_; }
        const std::string& content() const { return content_; }
        const tags_t& tags() const { return tags_; }

        std::string info() const;
        bool hasTags() const { return ! tags_.empty(); }
        bool isIndexed() const { return line_ >= 0; }

        const std::vector<uint8_t>& binary() const;
        size_t binarySize() const;

        friend void to_json( nlohmann::json& json, const Message& message );
        friend void from_json( const nlohmann::json& json, Message& message );

   private:

        DateTime datetime_ = {};
        uint64_t threadId_ = {};
        std::string file_ = {};
        std::string function_ = {};
        int line_ = -1;
        int level_ = -1;
        std::string content_ = {};
        tags_t tags_ = {};

        mutable std::vector<uint8_t> binary_ = {}; //  mutable for lazy evaluation
};


}   //  ::bugle
