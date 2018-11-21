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
MessageCenter::MessageCenter() 
{}


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

    if ( filter_.find( observer ) == filter_.end() ) {
        filter_[ observer ] = BooleanFilter( filter );
    } else {
        filter_[ observer ].unite( filter );
    }
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post(
    const nlohmann::json& object,
    MC_INFO_DECLARE,
    const nlohmann::json& tags )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    Message message( MC_INFO_NAMES, object, tags );
    auto f = std::async( &MessageCenter::postMessage, this, std::move( message ) );
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
        if ( observer.expired() ) {
            observers_.erase( observer );
            continue;
        }

        const auto& filter = filter_[ observer ];
        auto& observerRef = observer.lock();

        if ( ! filter.passes( message.tags() ) ) {
            continue;
        }

        observerRef->notify( message );
    }
}


}   //  ::mc
