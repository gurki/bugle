#include "messagecenter/messagecenter.h"
#include "messagecenter/message.h"
#include "messagecenter/observer.h"

#include <future>   //  std::async
#include <execution>    //  std::execution::par
#include <algorithm>    //  std::for_each

using namespace std::chrono_literals;


namespace mc {


MessageCenterPtr MessageCenter::instance_ = std::make_shared<MessageCenter>();


////////////////////////////////////////////////////////////////////////////////
MessageCenter::MessageCenter() {
    worker_ = std::thread( &MessageCenter::processQueue, this );
}


////////////////////////////////////////////////////////////////////////////////
MessageCenter::~MessageCenter() {
    terminate();
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::terminate() 
{
    std::cout << "terminate" << std::endl;
    alive_ = false;
    std::lock_guard<std::mutex> guard( workerMutex_ );
    std::lock_guard<std::mutex> fguard( futureMutex_ );
    std::cout << "locked" << std::endl;
    worker_.~thread();

    while ( ! futures_.empty() ) {
        futures_.front().~future();
        futures_.pop();
    }
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

    //  TODO(tgurdan): 
    //    provide rvalue reference implementation to avoid copies if not necessary
    auto tid = std::this_thread::get_id();

    auto f = std::async( 
        std::launch::deferred,
        &MessageCenter::postAsync, 
        this, content, MC_INFO_NAMES, tags, tid 
    );
    
    std::lock_guard<std::mutex> guard( futureMutex_ );
    futures_.push( std::move( f ) );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::processQueue()
{
    std::shared_future<void> f;

    while ( true ) 
    {
        std::lock_guard<std::mutex> guard( workerMutex_ );

        if ( ! alive_ ) {
            std::cout << "not alive" << std::endl;
            return;
        }

        if ( futures_.empty() ) {
            std::this_thread::sleep_for( 10ms );
            continue;
        }

        {
            std::cout << "process ..." << std::endl;
            std::lock_guard<std::mutex> guard( futureMutex_ );
            f = futures_.front().share();
            futures_.pop();
        }

        f.wait();
        std::cout << "done." << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postAsync(
    const nlohmann::json& content,
    MC_INFO_DECLARE,
    const nlohmann::json& tags,
    const std::thread::id& threadId )
{
    std::lock_guard<std::mutex> guard( observerMutex_ );
    //  asynchronous construction of non-moveable Message. included in postAsync in case 
    //  of time-consuming tag parsing. allows partially parallel postAsync calls.
    const auto messagePtr = std::make_shared<Message>( MC_INFO_NAMES, content, tags );

    //  lock to avoid observer insertion from another thread during iteration.
    //  as a side effect, this ensures multiple postAsync calls run sequentially, keeping
    //  chronological order.

    for ( auto it = observers_.begin(); it != observers_.end(); )
    {
        if ( ! it->expired() ) {
            ++it;
            continue;
        }

        filter_.erase( *it );
        it = observers_.erase( it );
    }

    const auto& filters = filter_;
    
    std::for_each( 
        std::execution::par, 
        observers_.begin(), observers_.end(), 
        [ &messagePtr, &filters ]( const auto& observerRef ) 
        {
            const auto& filter = filters.at( observerRef );

            if ( ! filter.passes( messagePtr->tags() ) ) {
                return;
            }

            auto observer = observerRef.lock();
            std::this_thread::sleep_for( 500ms );
            observer->notify( messagePtr ); 
        }
    );
}


}   //  ::mc
