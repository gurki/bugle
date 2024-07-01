#include "bugle/core/letter.h"

#include <format>
#include <unordered_set>
#include <functional>
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
    static const bool useEmojis = false;
    static const std::string locOpen = ( useEmojis ? "📁 " : "[" );
    static const std::string locClose = ( useEmojis ? "" : "]" );
    static const std::string thrOpen = ( useEmojis ? "🧵 " : "" );
    static const std::string thrClose = ( useEmojis ? "" : "]" );

    return std::format( "{}{}@{}:{}.{}{}",
        locOpen,
        functionInfo(),
        fileInfo(),
        line(),
        column(),
        locClose
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


//////////////////////////////////////////////////////////////////////////////////
void from_json( const nlohmann::json& json, Letter& message )
{
    //  TODO

    // try
    // {
    //     message.timestamp_ = json.at( "timestamp" ).get<Timestamp>();
    //     message.thread_ = json.at( "thread" ).get<uint64_t>();

    //     if ( json.find( "file" ) != json.end() )
    //     {
    //         message.file_ = json.at( "file" ).get<std::string>();
    //         message.function_ = json.at( "function" ).get<std::string>();
    //         message.line_ = json.at( "line" ).get<int>();
    //         message.level_ = json.at( "level" ).get<int>();
    //     }

    //     message.content_ = json.at( "content" ).get<std::string>();
    //     message.tags_ = json.at( "tags" ).get<tags_t>();
    // }
    // catch ( const std::exception& ) {
    //     message = {};
    //     std::cerr << "couldn't parse message from json" << std::endl;
    // }
}


}   //  ::bugle
