#include "messagecenter/messagecenter.h"
#include "messagecenter/message.h"
#include "messagecenter/messageobserver.h"

#include <algorithm>

namespace mc {


////////////////////////////////////////////////////////////////////////////////
MessageCenterPtr MessageCenter::instance_ = std::make_shared<MessageCenter>();
//QMap<Qt::HANDLE, int> MessageCenter::levels_;
//QMutex MessageCenter::levelMutex_;


////////////////////////////////////////////////////////////////////////////////
MessageCenter::MessageCenter()
{}


////////////////////////////////////////////////////////////////////////////////
MessageCenter::~MessageCenter()
{}


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

    std::lock_guard<std::mutex> guard( observerMutex_ );

    observers_.insert( observer );

    //  TODO: filter

//    if ( ! observers_.contains( observer ) )
//    {
//        auto filter = new BooleanFilter();
//        filter->moveToThread( this->thread() );

//        observers_[ observer ] = filter;
//    }

//    observers_[ observer ]->unite( filter );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post(
    const nlohmann::json& object,
    MC_INFO_DEFINE ) const
{
#ifdef MC_DISABLE_POST
    return;
#endif

    Message message( MC_INFO_NAMES, object );
    post( message );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post(
    const nlohmann::json& object,
    const nlohmann::json& tags,
    MC_INFO_DEFINE ) const
{
#ifdef MC_DISABLE_POST
    return;
#endif

    Message message( MC_INFO_NAMES, object, tags );
    post( message );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post( const Message& message ) const
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! enabled_ ) {
        return;
    }

    for ( const auto& observer : observers_ )
    {
        //  TODO: filter

        if ( observer.expired() ) {
            observers_.erase( observer );
            continue;
        }

        observer.lock()->notify( message );
    }
}


////////////////////////////////////////////////////////////////////////////////
const MessageCenter& operator << (
    const MessageCenter& msgcntr,
    const nlohmann::json& data )
{
    msgcntr.post( data.dump(), nullptr, nullptr, -1 );
    return msgcntr;
}


////////////////////////////////////////////////////////////////////////////////
const MessageCenter& operator << (
    const MessageCenter& msgcntr,
    const MessageCenter::Flag flag )
{
    msgcntr.post( "EOM", nullptr, nullptr, -1 );
    return msgcntr;
}


}   //  mc::
