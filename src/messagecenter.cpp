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

    std::cout << DateTime::now().timeInfo() << " -- enter post " << std::this_thread::get_id() << std::endl;

    //  thread::detach instead of async, to allow non-blocking continuation from caller thread
    //  arguments are copied to ensure thread-safety
    //
    //  TODO(tgurdan): 
    //    provide rvalue reference implementation to avoid copies if not necessary
    std::thread( &MessageCenter::postAsync, this, content, MC_INFO_NAMES, tags ).detach();
    
    std::cout << DateTime::now().timeInfo() << " -- exit post " << std::this_thread::get_id() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postAsync(
    const nlohmann::json& content,
    MC_INFO_DECLARE,
    const nlohmann::json& tags )
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for( 1s );
    
    std::cout << DateTime::now().timeInfo() << " -- enter postAsync " << std::this_thread::get_id() << std::endl;
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

    std::cout << DateTime::now().timeInfo() << " -- exit postAsync " << std::this_thread::get_id() << std::endl;
}


}   //  ::mc
