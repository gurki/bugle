#include "messagecenter/messagecenter.h"
#include "messagecenter/message.h"
#include "messagecenter/observer.h"

#include <future>   //  std::async


namespace mc {


MessageCenterPtr MessageCenter::instance_ = std::make_shared<MessageCenter>();


////////////////////////////////////////////////////////////////////////////////
MessageCenter::MessageCenter() 
{}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::addObserver(
    const ObserverRef& observer,
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
    const nlohmann::json& content,
    MC_INFO_DECLARE,
    const nlohmann::json& tags )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! enabled_ ) {
        return;
    }

    std::cout << DateTime::now().timeInfo( DateTime::Microseconds ) << " -- enter post " << std::this_thread::get_id() << std::endl;

    auto f = std::async( std::launch::async, &MessageCenter::postAsync, this, content, MC_INFO_NAMES, tags );
    // std::thread( std::launch::async, &MessageCenter::postAsync, this, content, MC_INFO_NAMES, tags );
    
    std::cout << DateTime::now().timeInfo( DateTime::Microseconds ) << " -- exit post " << std::this_thread::get_id() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postAsync(
    nlohmann::json content,
    MC_INFO_DECLARE,
    nlohmann::json tags )
{
    std::cout << DateTime::now().timeInfo( DateTime::Microseconds ) << " -- enter postAsync " << std::this_thread::get_id() << std::endl;

    using namespace std::chrono_literals;
    std::this_thread::sleep_for( 2s );

    const auto message = Message( MC_INFO_NAMES, content, tags );

    std::lock_guard<std::mutex> guard( observerMutex_ );

    for ( const auto& observer : observers_ )
    {
        if ( observer.expired() ) {
            observers_.erase( observer );
            filter_.erase( observer );
            continue;
        }

        const auto& filter = filter_[ observer ];

        if ( ! filter.passes( message.tags() ) ) {
            continue;
        }

        observer.lock()->notify( message );
    }

    std::cout << DateTime::now().timeInfo( DateTime::Microseconds ) << " -- exit postAsync " << std::this_thread::get_id() << std::endl;
}


}   //  ::mc
