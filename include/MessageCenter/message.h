#pragma once

#include "messagecenter/datetime.h"

#include <nlohmann/json.hpp>

#include <string>
#include <thread>
#include <chrono>
#include <iostream>


namespace mc {
    
    
typedef nlohmann::json::object_t tags_t;
tags_t filterTags( const nlohmann::json& tags );


class Message
{
    public:

        Message();

        Message(
            const char* file,
            const char* function,
            const int line,
            const nlohmann::json& object,
            const nlohmann::json& tags = {}
        );

//        void set(
//            const std::string& text,
//            const nlohmann::json& tags
//        );
//        void updateTimestamp();

        const DateTime& timestamp() const { return datetime_; }
        const nlohmann::json& content() const { return content_; }
        const tags_t& tags() const { return tags_; }
        const std::string& file() const { return file_; }
        const std::string& function() const { return function_; }
        const std::thread::id& threadId() const { return threadId_; }
        int line() const { return line_; }
        int level() const { return level_; }

        std::string info() const;
        bool hasTags() const { return ! tags_.empty(); }
        bool isIndexed() const { return line_ >= 0; }

   private:

//        void addSystemTags();

        std::string file_ = {};
        std::string function_ = {};
        int line_ = -1;
        int level_ = -1;

        DateTime datetime_ = {};
        nlohmann::json content_ = {};
        tags_t tags_ = {};
        std::thread::id threadId_ = {};
};


}   //  mc::
