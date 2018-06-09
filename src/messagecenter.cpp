#include "messagecenter/messagecenter.h"
#include "messagecenter/message.h"
#include "messagecenter/messageobserver.h"

#include <algorithm>


////////////////////////////////////////////////////////////////////////////////
MessageCenterPtr MessageCenter::instance_ = std::make_shared<MessageCenter>();
//QMap<Qt::HANDLE, int> MessageCenter::levels_;
//QMutex MessageCenter::levelMutex_;


////////////////////////////////////////////////////////////////////////////////
MessageCenter::MessageCenter()
{}


////////////////////////////////////////////////////////////////////////////////
MessageCenter::~MessageCenter()
{
#ifndef MC_DISABLE_POST

#endif
}


//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::setTrace( const bool enabled ) {
//    setTrace( enabled, enabled );
//}


//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::setTrace( const bool enter, const bool leave ) {
//    setEnter( enter );
//    setLeave( leave );
//}


//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::setEnter( const bool enabled )
//{
//    if ( enter_ == enabled ) {
//        return;
//    }

//    enter_ = enabled;
//}


//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::setLeave( const bool enabled )
//{
//    if ( leave_ == enabled ) {
//        return;
//    }

//    leave_ = enabled;
//}


//////////////////////////////////////////////////////////////////////////////////
//int MessageCenter::level( const Qt::HANDLE threadId )
//{
//    levelMutex_.lock();
//    const int level = levels_[ threadId ];
//    levelMutex_.unlock();

//    return level;
//}


//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::increaseLevel( const Qt::HANDLE threadId ) {
//    levelMutex_.lock();
//    levels_[ threadId]++;
//    levelMutex_.unlock();
//}


//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::decreaseLevel( const Qt::HANDLE threadId ) {
//    levelMutex_.lock();
//    levels_[ threadId ]--;
//    levelMutex_.unlock();
//}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::addObserver(
    const MessageObserverRef& observer,
    const std::string& filter )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    observers_.insert( observer );
//    if ( ! observers_.contains( observer ) )
//    {
//        auto filter = new BooleanFilter();
//        filter->moveToThread( this->thread() );

//        observers_[ observer ] = filter;
//    }

//    observers_[ observer ]->unite( filter );
}


//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::posts(
//    const QString& text,
//    const QString& tag,
//    MC_INFO_DEFINE ) const
//{
//#ifndef MC_DISABLE_POST
//    postm( text, {{ tag, QVariant() }}, MC_INFO_NAMES );
//#endif
//}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post(
    const std::string& text,
    MC_INFO_DEFINE ) const
{
#ifdef MC_DISABLE_POST
    return;
#endif

    Message message( MC_INFO_NAMES, text );
    post( message );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post(
    const std::string& text,
    const nlohmann::json& tags,
    MC_INFO_DEFINE ) const
{
#ifdef MC_DISABLE_POST
    return;
#endif

    Message message( MC_INFO_NAMES, text, tags );
    post( message );
}



//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::postl(
//    const QString& text,
//    const QVariantList& tags,
//    MC_INFO_DEFINE ) const
//{
//#ifndef MC_DISABLE_POST

//    QVariantMap richTags;

//    for ( const auto& tag : tags )
//    {
//        if ( tag.type() == QVariant::String ) {
//            richTags[ tag.toString() ] = QVariant();
//        }
//        else if ( tag.type() == QVariant::Map )
//        {
//            const QVariantMap map = tag.toMap();
//            QMapIterator<QString, QVariant> iter( map );

//            while ( iter.hasNext() )
//            {
//                const auto item = iter.next();
//                const auto& key = item.key();
//                const QVariant& value = item.value();

//                richTags[ key ] = mc::parseVariant( value );
//            }
//        }
//    }

//    postm( text, richTags, MC_INFO_NAMES );

//#endif
//}


//////////////////////////////////////////////////////////////////////////////////
//void MessageCenter::postm(
//    const QString& text,
//    const QVariantMap& tags,
//    MC_INFO_DEFINE ) const
//{
//#ifndef MC_DISABLE_POST
//    Message msg( MC_INFO_NAMES );
//    msg.set( text, tags );
//    post( msg );
//#endif
//}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post( const Message& message ) const
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! enabled_ ) {
        return;
    }

//    #ifndef MC_DISABLE_SCOPE
//        if ( message.tags().contains( "__scope" ) )
//        {
//            if ( ! enter_ && message.tags()[ "__scope" ] == "enter" ) {
//                return;
//            }

//            if ( ! leave_ && message.tags()[ "__scope" ] == "leave" ) {
//                return;
//            }
//        };
//    #endif

//    QMapIterator<MessageObserver*, BooleanFilter*> iter( observers_ );

    for ( const auto& observer : observers_ )
    {
//        if ( filter->passes( message.tags() ) ) {
//            observer->notify( message );
//        }
        if ( observer.expired() ) {
            observers_.erase( observer );
            continue;
        }

        observer.lock()->notify( message );
    }
}
