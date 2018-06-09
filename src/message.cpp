#include "messagecenter/message.h"
//#include "messagecenter.h"


////////////////////////////////////////////////////////////////////////////////
Message::Message() :
    datetime_( std::chrono::system_clock::now() ),
    threadId_( std::this_thread::get_id() )
{
//    level_ = MessageCenter::level( threadId_ );
}


////////////////////////////////////////////////////////////////////////////////
Message::Message(
    const char* file,
    const char* function,
    const int line,
    const std::string& text,
    const nlohmann::json& tags ):
    Message()
{
    file_ = file;
    function_ = function;
    line_ = line;
    text_ = text;
    tags_ = tags;

//    file_.remove( "..\\" );
}


////////////////////////////////////////////////////////////////////////////////
Message::Message(
    const std::string& text,
    const nlohmann::json& tags ):
    Message()
{
    text_ = text;
    tags_ = tags;
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
