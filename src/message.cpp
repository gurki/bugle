#include "messagecenter/message.h"

namespace mc {


////////////////////////////////////////////////////////////////////////////////
Message::Message() :
    datetime_( DateTime::now() ),
    threadId_( std::this_thread::get_id() )
{
//    level_ = MessageCenter::level( threadId_ );
}


////////////////////////////////////////////////////////////////////////////////
Message::Message(
    const char* file,
    const char* function,
    const int line,
    const nlohmann::json& object,
    const jmap_t& tags ):
    Message()
{
    if ( file ) {
        file_ = file;
    }

    if ( function ) {
        function_ = function;
    }

    line_ = line;
    payload_ = object;
    tags_ = tags;

//    file_.remove( "..\\" );
}


//////////////////////////////////////////////////////////////////////////////////
//void Message::set(
//    const QString& text,
//    const QVariantMap& tags )
//{
//    text_ = text;
//    tags_ = tags;

//    addSystemTags();
//}


//////////////////////////////////////////////////////////////////////////////////
//void Message::addSystemTags()
//{
//    if ( line_ >= 0 ) {
//        tags_[ "__line" ] = line_;
//    }

//    if ( ! file_.isEmpty() ) {
//        tags_[ "__file" ] = file_;
//    }

//    if ( ! function_.isEmpty() ) {
//        tags_[ "__func" ] = function_;
//    }

//    tags_[ "__date" ] = datetime_.date().toString( Qt::ISODate );
//    tags_[ "__time" ] = datetime_.time().toString( Qt::ISODateWithMs );
//    tags_[ "__tid" ] = (qint64)threadId_;
//    tags_[ "__level" ] = level_;
//}


//////////////////////////////////////////////////////////////////////////////////
//void Message::updateTimestamp() {
//    datetime_ = QDateTime::currentDateTime();
//    tags_[ "__date" ] = datetime_.date().toString( Qt::ISODate );
//    tags_[ "__time" ] = datetime_.time().toString( Qt::ISODateWithMs );
//}


//////////////////////////////////////////////////////////////////////////////////
//QString Message::info() const
//{
//    if ( ! isIndexed() ) {
//        return QString();
//    }

//    QString info;
//    info += function_;
//    info += " - " + file_;
//    info += " l." + QString::number( line_ );
//    info += " [" + QString::number( (qint64)threadId_ ) + "]";

//    return info;
//}


//////////////////////////////////////////////////////////////////////////////////
//QString Message::timeInfo( const Qt::DateFormat format ) const {
//    return datetime_.time().toString( format );
//}


//////////////////////////////////////////////////////////////////////////////////
nlohmann::json filterTags( const nlohmann::json& tags ) 
{
    static auto insert = []( nlohmann::json& jobj, const nlohmann::json& tag ) -> void
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

    nlohmann::json jobj( nlohmann::json::value_t::object );
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
