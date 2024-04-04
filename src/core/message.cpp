#include "bugle/core/message.h"
#include <format>
#include <sstream>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Message::Message() :
    datetime_( DateTime::now() ),
    threadId_( std::hash<std::thread::id>{}( std::this_thread::get_id() ))
{}


////////////////////////////////////////////////////////////////////////////////
Message::Message(
    const char* file,
    const char* function,
    const int line,
    const int level,
    const std::thread::id& threadId,
    const std::string& content,
    const tags_t& tags ) :
    Message()
{
    file_ = file;
    function_ = function;
    line_ = line;
    level_ = level;
    threadId_ = std::hash<std::thread::id>{}( threadId );
    content_ = content;
    tags_ = tags;

    //  convert absolute to single node relative path

    if ( file_.find( "..\\" ) == 0 ) {
        file_ = file_.substr( 3 );
    }

    std::replace( file_.begin(), file_.end(), '\\', '/' );

    //  file name boundary
    if ( size_t boundary = file_.find_last_of( '/' ) )
    {
        //  node boundary
        if ( boundary = file_.find_last_of( '/', boundary - 1 ) ) {
            file_ = file_.substr( boundary + 1 );
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
std::string Message::info() const
{
   if ( ! isIndexed() ) {
       return {};
   }

   std::stringstream info;
   info << "[" << function_;
   info << "@" << file_;
   info << ":" << line_;
   info << "]  [" << std::format( "{:x}", threadId_ & 0x0fffffff ) << "]";

   return info.str();
}


//////////////////////////////////////////////////////////////////////////////////
const std::vector<uint8_t>& Message::binary() const
{
    if ( ! binary_.empty() ) {
        return binary_;
    }

    binary_ = nlohmann::json::to_cbor( *this );
    return binary_;
}


//////////////////////////////////////////////////////////////////////////////////
size_t Message::binarySize() const {
    return binary().size();
}


//////////////////////////////////////////////////////////////////////////////////
void to_json( nlohmann::json& json, const Message& message )
{
    json[ "timestamp" ] = message.timestamp();
    json[ "thread" ] = uint64_t( message.threadId() );

    if ( message.isIndexed() )
    {
        json[ "file" ] = message.file();
        json[ "function" ] = message.function();
        json[ "line" ] = message.line();
        json[ "level" ] = message.level();
    }

    json[ "content" ] = message.content();
    json[ "tags" ] = message.tags();
}


//////////////////////////////////////////////////////////////////////////////////
void from_json( const nlohmann::json& json, Message& message )
{
    try
    {
        message.datetime_ = json.at( "timestamp" ).get<DateTime>();
        message.threadId_ = json.at( "thread" ).get<uint64_t>();

        if ( json.find( "file" ) != json.end() )
        {
            message.file_ = json.at( "file" ).get<std::string>();
            message.function_ = json.at( "function" ).get<std::string>();
            message.line_ = json.at( "line" ).get<int>();
            message.level_ = json.at( "level" ).get<int>();
        }

        message.content_ = json.at( "content" ).get<std::string>();
        message.tags_ = json.at( "tags" ).get<tags_t>();
    }
    catch ( const std::exception& ) {
        message = {};
        std::cerr << "couldn't parse message from json" << std::endl;
    }
}


}   //  ::bugle
