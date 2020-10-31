#include "bugle/core/message.h"
#include <sstream>


namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Message::Message() :
    datetime_( DateTime::now() ),
    threadId_( std::this_thread::get_id() )
{}


////////////////////////////////////////////////////////////////////////////////
Message::Message(
    const char* file,
    const char* function,
    const int line,
    const int level,
    const std::thread::id threadId,
    const nlohmann::json& object,
    const nlohmann::json& tags ) :
    Message()
{
    file_ = file;
    function_ = function;
    line_ = line;
    level_ = level;
    threadId_ = threadId;
    content_ = object;
    tags_ = filterTags( tags );

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
   info << function_;
   info << " - " << file_;
   info << " l." << line_;
   info << " [" << threadId_ << "]";

   return info.str();
}


//////////////////////////////////////////////////////////////////////////////////
const std::vector<uint8_t>& Message::binary() const
{
    if ( ! binary_.empty() ) {
        return binary_;
    }

    binary_ = nlohmann::json::to_msgpack( *this );
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
    // json[ "thread_id" ] = message.threadId();

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
        message.datetime_ = json[ "timestamp" ];
        // message.threadId_ = json[ "threadId" ];

        if ( json.find( "file" ) != json.end() )
        {
            message.file_ = json[ "file" ].get<std::string>();
            message.function_ = json[ "function" ].get<std::string>();
            message.line_ = json[ "line" ];
            message.level_ = json[ "level" ];
        }

        message.content_ = json[ "content" ];
        message.tags_ = json[ "tags" ].get<tags_t>();
    }
    catch ( const std::exception& ) {
        message = {};
        std::cerr << "couldn't parse message from json" << std::endl;
    }
}


}   //  ::bugle
