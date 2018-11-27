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
MessageCenter::~MessageCenter() {
    terminate();
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::terminate()
{
    std::lock_guard<std::mutex> tguard( threadMutex_ );

    for ( auto& item : activeThreads_ ) {
        item.second.~thread();
    }

    activeThreads_.clear();
}


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

    //  asynchronous construction of non-moveable Message in case of time-consuming tag 
    //  parsing. using thread::detach instead of async, to allow non-blocking continuation 
    //  from caller thread. arguments are copied to ensure thread-safety.
    //
    //  TODO(tgurdan): 
    //    provide rvalue reference implementation to avoid copies if not necessary
    std::lock_guard<std::mutex> guard( threadMutex_ );

    auto tid = std::this_thread::get_id();
    auto t = std::thread( &MessageCenter::postAsync, this, content, MC_INFO_NAMES, tags, tid );
    t.detach();

    activeThreads_[ tid ] = std::move( t );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postAsync(
    const nlohmann::json& content,
    MC_INFO_DECLARE,
    const nlohmann::json& tags,
    const std::thread::id& threadId )
{
    //  construct message non-blocking, allowing partially parallel postAsync calls
    const auto message = Message( MC_INFO_NAMES, content, tags );

    //  disallow thread termination with active observer lock
    std::lock_guard<std::mutex> tguard( threadMutex_ );

    //  lock to avoid observer insertion from another thread during iteration.
    //  as a side effect, this ensures multiple postAsync calls run sequentially, keeping
    //  chronological order.
    std::lock_guard<std::mutex> oguard( observerMutex_ );

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
    }

    //  thread about to finish, remove such that it won't be manually terminated by parent
    activeThreads_.erase( threadId );
}


}   //  ::mc
