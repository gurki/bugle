#include "messagecenter/messagecenter.h"
#include "messagecenter/message.h"
#include "messagecenter/messageobserver.h"

#include <algorithm>
#include <iostream>
#include <thread>
#include <future>


namespace mc {


MessageCenterPtr MessageCenter::instance_ = std::make_shared<MessageCenter>();


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
    MC_INFO_DEFINE,
    const nlohmann::json& tags )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    Message message( MC_INFO_NAMES, object, tags );
    std::async( &MessageCenter::postMessage, this, std::move( message ) );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postMessage( const Message& message )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! enabled_ ) {
        return;
    }

    std::lock_guard<std::mutex> guard( observerMutex_ );

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


}   //  mc::
