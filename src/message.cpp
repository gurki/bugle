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
    const nlohmann::json& tags ):
    Message()
{
    if ( file )
        file_ = file;

    if ( function )
        function_ = function;

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


}   //  mc::
