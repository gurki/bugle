#include "messagecenter/message.h"
#include <sstream>


namespace mc {


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
    const std::thread::id threadId,
    const nlohmann::json& object,
    const nlohmann::json& tags ) :
    Message()
{
    if ( file ) {
        file_ = file;
    }

    if ( function ) {
        function_ = function;
    }

    line_ = line;
    threadId_ = threadId;
    content_ = object;
    tags_ = filterTags( tags );

    if ( file_.find( "..\\" ) == 0 ) {
        file_ = file_.substr( 3 );
    }

    std::replace( file_.begin(), file_.end(), '\\', '/' );

    if ( size_t boundary = file_.find_last_of( '/' ) ) {
        file_ = file_.substr( boundary + 1 );
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
    // std::lock_guard<std::mutex> lock( binaryMutex_ );

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
tags_t filterTags( const nlohmann::json& tags ) 
{
    static auto insert = []( tags_t& jobj, const nlohmann::json& tag ) -> void
    {
        //  ignores arrays, numbers and bool

        if ( tag.is_string() ) {
            jobj[ tag.get<std::string>() ] = {};
        }

        if ( tag.is_object() ) {
            for ( auto it = tag.begin(); it != tag.end(); ++it ) {
                jobj[ it.key() ] = it.value();
            }
        }            
    };
    
    //  only considers first level strings, objects and convertible arrays
    //  ignores all numbers, bools and second-level non-convertible arrays

    tags_t jobj;
    
    if ( tags.is_array() ) 
    {
        for ( const auto& tag : tags ) 
        {
            if ( tag.is_array() && tag.size() == 2 ) {
                if ( tag[0].is_string() ) {
                    jobj[ tag[0].get<std::string>() ] = tag[1];
                }
            }
            else {
                insert( jobj, tag );
            }
        }
    }
    else {
        insert( jobj, tags );
    }

    return jobj;
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
    message.datetime_ = json[ "timestamp" ];
    // message.threadId_ = json[ "threadId" ];

    if ( json.find( "file" ) != json.end() ) 
    {
        message.file_ = json[ "file" ];
        message.function_ = json[ "function" ];
        message.line_ = json[ "line" ];
        message.level_ = json[ "level" ];
    }

    message.content_ = json[ "content" ];
    message.tags_ = json[ "tags" ].get<tags_t>();
}


}   //  mc::
