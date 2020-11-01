#pragma once

#include "bugle/core/defines.h"
#include "bugle/core/tags.h"
#include "bugle/utility/datetime.h"

#include <thread>

namespace bugle {


class Scope
{
    public:

        Scope( BUGLE_INFO_DECLARE_DEFAULT );
        Scope( const nlohmann::json& tags, BUGLE_INFO_DECLARE_DEFAULT );
        ~Scope();

        void setLevel( const int level ) { level_ = level; };

        const DateTime& timestamp() const { return timestamp_; }
        const std::thread::id& threadId() const { return threadId_; }
        const std::string& file() const { return file_; }
        const std::string& function() const { return func_; }
        int line() const { return line_; }
        int level() const { return level_; }
        const tags_t& tags() const { return tags_; }

    private:

        const char* file_ = nullptr;
        const char* func_ = nullptr;
        int line_ = -1;
        int level_ = -1;
        DateTime timestamp_ = {};
        std::thread::id threadId_ = {};
        tags_t tags_ = {};
};


}   //  ::bugle
