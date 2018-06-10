#pragma once

#include "messagecenter/datetime.h"

#include <nlohmann/json.hpp>

#include <string>
#include <thread>
#include <chrono>


namespace mc {


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

//        const std::chrono::time_point& timestamp() const { return datetime_; }
//        const std::string& text() const { return text_; }
//        const nlohmann::json& tags() const { return tags_; }
//        const std::string& file() const { return file_; }
//        const std::string& function() const { return function_; }
//        int line() const { return line_; }
//        int level() const { return level_; }
//        std::thread::id threadId() const { return threadId_; }

//        std::string info() const;
//        std::string timeInfo( const Qt::DateFormat format = Qt::ISODateWithMs ) const;
        bool hasTags() const { return ! tags_.empty(); }
        bool isIndexed() const { return line_ >= 0; }

//    private:

//        void addSystemTags();

        std::string file_ = {};
        std::string function_ = {};
        int line_ = -1;
        int level_ = -1;

        DateTime datetime_ = {};
        nlohmann::json object_ = {};
        nlohmann::json tags_ = {};
        std::thread::id threadId_ = {};
};


}   //  mc::
