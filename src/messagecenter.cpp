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

    std::cout << "  " << DateTime::now().timeInfo() << " -- enter post " << std::this_thread::get_id() << std::endl;

    //  asynchronous construction of non-moveable Message in case of time-consuming tag parsing.
    //  use thread::detach instead of async, to allow non-blocking continuation from caller thread.
    //  arguments are copied to ensure thread-safety.
    //
    //  TODO(tgurdan): 
    //    provide rvalue reference implementation to avoid copies if not necessary
    std::thread( &MessageCenter::postAsync, this, content, MC_INFO_NAMES, tags ).detach();
    
    std::cout << "  " << DateTime::now().timeInfo() << " -- exit post " << std::this_thread::get_id() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postAsync(
    const nlohmann::json& content,
    MC_INFO_DECLARE,
    const nlohmann::json& tags )
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for( std::chrono::milliseconds( std::rand() % 500 ) );
    std::cout << "    " << DateTime::now().timeInfo() << " -- enter postAsync " << std::this_thread::get_id() << std::endl;
    const auto message = Message( MC_INFO_NAMES, content, tags );

    //  avoid observer insertion during iteration
    //  ensure multiple postAsync calls run sequentially to keep chronological order
    std::lock_guard<std::mutex> guard( observerMutex_ );

    std::cout << "    " << content << std::endl;
    std::this_thread::sleep_for( 2s );

    for ( const auto& observerRef : observers_ )
    {
        if ( observerRef.expired() ) {
            observers_.erase( observerRef );
            filter_.erase( observerRef );
            continue;
        }

        const auto& filter = filter_[ observerRef ];

        if ( ! filter.passes( message.tags() ) ) {
            continue;
        }

        auto observer = observerRef.lock();
        std::future<void> f = std::async( std::launch::async, [ &observer, &message ]{ observer->notify( message ); } );
        // std::thread( [ &observer, &message ]{ observer->notify( message ); } ).detach();
    }

    std::cout << "    " << DateTime::now().timeInfo() << " -- exit postAsync " << std::this_thread::get_id() << std::endl;
}


}   //  ::mc
