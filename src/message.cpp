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
    content_ = object;
    tags_ = filterTags( tags );

    if ( file_.find( "..\\" ) == 0 ) {
        file_ = file_.substr( 3 );
    }

    std::replace( file_.begin(), file_.end(), '\\', '/' );
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
    insert( jobj, tags );
    
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

    return jobj;
}


}   //  mc::
