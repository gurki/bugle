#include "bugle/core/messagecenter.h"
#include "bugle/core/message.h"
#include "bugle/core/observer.h"

#include <fmt/format.h>

using namespace std::chrono_literals;

namespace bugle {


MessageCenterUPtr MessageCenter::instance_ = nullptr;


////////////////////////////////////////////////////////////////////////////////
MessageCenter& MessageCenter::instance()
{
    if ( ! instance_ ) {
        instance_ = std::make_unique<MessageCenter>();
    }

    return *instance_;
}


////////////////////////////////////////////////////////////////////////////////
MessageCenter::MessageCenter()
{
#ifdef MC_DISABLE_POST
    return;
#endif

    workerThread_ = std::thread( &MessageCenter::processQueue, this );
}


////////////////////////////////////////////////////////////////////////////////
MessageCenter::~MessageCenter()
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! workerThread_.joinable() ) {
        return;
    }

    shouldExit_ = true;
    queueReady_.notify_one();
    workerThread_.join();
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::flush()
{
#ifdef MC_DISABLE_POST
    return;
#endif

    queueReady_.notify_one();
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::pushScope( Scope& scope )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    auto& stack = scopes_[ scope.threadId() ];
    scope.setLevel( int( stack.size() ));
    scopes_[ scope.threadId() ].push( scope );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::popScope( const Scope& scope )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    scopes_[ scope.threadId() ].pop();
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::addObserver(
    const ObserverRef& observer,
    const std::string& filter )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    std::unique_lock lock( observerMutex_ );

    observers_.insert( observer );

    if ( filter_.find( observer ) == filter_.end() ) {
        filter_[ observer ] = BooleanFilter( filter );
    } else {
        filter_[ observer ].unite( filter );
    }
}

////////////////////////////////////////////////////////////////////////////////
void MessageCenter::removeObserver( const ObserverRef& observer )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    std::unique_lock lock( observerMutex_ );

    observers_.erase( observer );
    filter_.erase( observer );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::processQueue()
{
    while ( ! shouldExit_ )
    {
        {
            std::unique_lock lock( queueMutex_ );

            queueReady_.wait( lock, [=](){
                return ! messages_.empty() || shouldExit_ ;
            });
        }

        Message message {};

        while ( ! messages_.empty() )
        {
            {
                std::unique_lock lock( queueMutex_ );
                message = std::move( messages_.front() );
                messages_.pop_front();
            }

            //  lock to avoid observer insertion from another thread during iteration.
            //  as a side effect, this ensures multiple postAsync calls run sequentially, keeping
            //  chronological order.
            std::shared_lock lock( observerMutex_ );

            //  remove expired observers
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

            for ( auto& observerRef : observers_ )
            {
                const auto& filter = filters.at( observerRef );

                if ( ! filter.passes( message.tags() ) ) {
                    continue;
                }

                auto observer = observerRef.lock();
                observer->notify( message );
            };
        }
    }
}


}   //  ::bugle
