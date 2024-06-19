#include "bugle/core/postoffice.h"
#include "bugle/core/letter.h"
#include "bugle/core/observer.h"

namespace bugle {


PostOfficeUPtr PostOffice::instance_ = nullptr;


////////////////////////////////////////////////////////////////////////////////
PostOffice& PostOffice::instance()
{
    if ( ! instance_ ) {
        instance_ = std::make_unique<PostOffice>();
    }

    return *instance_;
}


////////////////////////////////////////////////////////////////////////////////
PostOffice::PostOffice()
{
#ifdef MC_DISABLE_POST
    return;
#endif

    workerThread_ = std::thread( &PostOffice::processQueue, this );
}


////////////////////////////////////////////////////////////////////////////////
PostOffice::~PostOffice()
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
void PostOffice::flush()
{
#ifdef MC_DISABLE_POST
    return;
#endif

    queueReady_.notify_one();
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::post( Letter&& letter )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! enabled_ ) {
        return;
    }

    {
        std::scoped_lock lock( queueMutex_ );
        letters_.emplace_back( letter );
    }

    queueReady_.notify_one();
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::post(
    const std::string& message,
    const tags_t& tags,
    const attributes_t& attributes,
    const std::source_location& location )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! enabled_ ) {
        return;
    }

    const int level = this->level( std::this_thread::get_id() );
    post( { message, tags, attributes, location, level } );
}


////////////////////////////////////////////////////////////////////////////////
int PostOffice::level( const std::thread::id& thread )
{
    if ( ! levels_.contains( thread ) ) {
        return 0;
    }

    return levels_.at( thread );
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::push( const std::thread::id& thread )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! levels_.contains( thread ) ) {
        levels_[ thread ] = 1;
        return;
    }

    levels_[ thread ]++;

}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::pop( const std::thread::id& thread )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! levels_.contains( thread ) ) {
        return;
    }

    levels_[ thread ]--;
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::addObserver(
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
void PostOffice::removeObserver( const ObserverRef& observer )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    std::unique_lock lock( observerMutex_ );

    observers_.erase( observer );
    filter_.erase( observer );
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::processQueue()
{
    while ( ! shouldExit_ )
    {
        {
            std::unique_lock lock( queueMutex_ );

            queueReady_.wait( lock, [ this ](){
                return ! letters_.empty() || shouldExit_ ;
            });
        }

        Letter message {};

        while ( ! letters_.empty() )
        {
            {
                std::unique_lock lock( queueMutex_ );
                message = std::move( letters_.front() );
                letters_.pop_front();
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

                if ( ! filter.passes( message.tags ) ) {
                    continue;
                }

                auto observer = observerRef.lock();
                observer->notify( message );
            };
        }
    }
}


}   //  ::bugle
