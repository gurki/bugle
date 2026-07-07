#include "bugle/core/letter.h"

#include <format>
#include <functional>   //  std::hash
#include <regex>
#include <filesystem>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Letter::Letter(
    const std::string& _message,
    const tags_t& _tags,
    const attributes_t& _attributes,
    const std::source_location& _location,
    const int _level ) :
    timestamp( Timestamp::now() ),
    thread( std::this_thread::get_id() ),
    level( _level ),
    location( _location ),
    message( _message ),
    tags( _tags ),
    attributes( _attributes )
{}


////////////////////////////////////////////////////////////////////////////////
std::string Letter::locationInfo() const
{
    return std::format( "[{}@{}:{}.{}]",
        functionInfo(),
        fileInfo(),
        line(),
        column()
    );
}


////////////////////////////////////////////////////////////////////////////////
std::string Letter::threadInfo() const {
    return std::format( "{:04x}", std::hash<std::thread::id>{}( thread ) & 0xffff );
}


////////////////////////////////////////////////////////////////////////////////
std::string Letter::functionInfo() const {
    const std::regex re( R"((.+ )?(?:__cdecl )?([\w\d_<>:~]+)(?:::operator \(\))?(\(.*\))(?: const)?)" );
    std::smatch match;
    const std::string& fn = function();
    std::regex_match( fn, match, re );
    return std::format( "{}", match[ 2 ].str() );
}


////////////////////////////////////////////////////////////////////////////////
std::string Letter::fileInfo() const {
    std::filesystem::path path( file() );
    return path.filename().string();
}


//////////////////////////////////////////////////////////////////////////////////
void to_json( nlohmann::json& json, const Letter& letter )
{
    json[ "timestamp" ] = letter.timestamp;
    json[ "thread" ] = letter.threadInfo();
    json[ "level" ] = letter.level;

    json[ "file" ] = letter.file();
    json[ "function" ] = letter.function();
    json[ "line" ] = letter.line();
    json[ "column" ] = letter.column();

    json[ "message" ] = letter.message;
    json[ "tags" ] = letter.tags;
    json[ "attributes" ] = letter.attributes;
}


}   //  ::bugle
