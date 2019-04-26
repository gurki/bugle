#include "messagecenter/messagecenter.h"
#include "messagecenter/message.h"
#include "messagecenter/observer.h"


using namespace std::chrono_literals;


namespace mc {


MessageCenterPtr MessageCenter::instance_ = std::make_shared<MessageCenter>();


////////////////////////////////////////////////////////////////////////////////
MessageCenter::MessageCenter()  {
    workerThread_ = std::thread( &MessageCenter::processQueue, this );
}


////////////////////////////////////////////////////////////////////////////////
MessageCenter::~MessageCenter() {
    shouldExit_ = true;
    queueReady_.notify_one();
    workerThread_.join();
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

    auto tid = std::this_thread::get_id();

    {
        std::unique_lock lock( queueMutex_ );
        messages_.push_back( { content, file, func, line, tags, tid } );
    }

    queueReady_.notify_one();
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::processQueue()
{
    while ( ! shouldExit_ ) 
    {
        {
            std::unique_lock lock( queueMutex_ );
            queueReady_.wait( lock );
        }

        MessageData data;
        bool finished = messages_.empty();

        while ( ! finished )
        {
            {
                std::unique_lock lock( queueMutex_ );
                data = std::move( messages_.front() );
                
                messages_.pop_front();
                finished = messages_.empty();
            }

            const auto messagePtr = std::make_shared<Message>( 
                data.file, data.func, data.line, data.threadId, data.content, data.tags 
            );

            //  lock to avoid observer insertion from another thread during iteration.
            //  as a side effect, this ensures multiple postAsync calls run sequentially, keeping
            //  chronological order.
            std::shared_lock lock( observerMutex_ );

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

                if ( ! filter.passes( messagePtr->tags() ) ) {
                    continue;
                }

                auto observer = observerRef.lock();
                observer->notify( messagePtr ); 
            };
        }
    }
}


}   //  ::mc
