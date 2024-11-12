#include "bugle/core/postoffice.h"
#include "bugle/core/letter.h"
#include "bugle/core/recipient.h"

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
#ifdef BUGLE_ENABLE
    workerThread_ = std::thread( &PostOffice::processQueue, this );
#endif
}


////////////////////////////////////////////////////////////////////////////////
PostOffice::~PostOffice()
{
#ifdef BUGLE_ENABLE
    if ( ! workerThread_.joinable() ) {
        return;
    }

    shouldExit_ = true;
    queueReady_.notify_one();
    workerThread_.join();
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::flush()
{
#ifdef BUGLE_ENABLE
    //  FIXME: probably need to guard empty check against threading
    while ( ! letters_.empty() || onRoute_ ) {
        std::scoped_lock lock( queueMutex_ );
        queueReady_.notify_one();
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::post( Letter&& letter )
{
#ifdef BUGLE_ENABLE
    if ( ! enabled_ ) {
        return;
    }

    {
        std::scoped_lock lock( queueMutex_ );
        letters_.emplace_back( letter );
    }

    queueReady_.notify_one();
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::post(
    const std::string& message,
    const tags_t& tags,
    const attributes_t& attributes,
    const std::source_location& location )
{
#ifdef BUGLE_ENABLE
    if ( ! enabled_ ) {
        return;
    }

    const int level = this->level( std::this_thread::get_id() );
    post( { message, tags, attributes, location, level } );
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::memo( const std::string& message, const std::source_location& location )
{
    post( message, {}, {}, location );
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::card( const tags_t& tags, const std::source_location& location )
{
    post( {}, tags, {}, location );
}


////////////////////////////////////////////////////////////////////////////////
int PostOffice::level( const std::thread::id& thread )
{
#ifdef BUGLE_ENABLE
    std::scoped_lock lock( queueMutex_ );

    if ( ! levels_.contains( thread ) ) {
        return 0;
    }

    return levels_.at( thread );
#else
    return 0;
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::push( const std::thread::id& thread )
{
#ifdef BUGLE_ENABLE
    std::scoped_lock lock( queueMutex_ );

    if ( ! levels_.contains( thread ) ) {
        levels_[ thread ] = 1;
        return;
    }

    levels_[ thread ]++;
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::pop( const std::thread::id& thread )
{
#ifdef BUGLE_ENABLE
    std::scoped_lock lock( queueMutex_ );

    if ( ! levels_.contains( thread ) ) {
        return;
    }

    levels_[ thread ]--;
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::addObserver(
    const RecipientRef& observer,
    const Filter& filter )
{
#ifdef BUGLE_ENABLE
    std::scoped_lock lock( observerMutex_ );

    observers_.insert( observer );

    if ( ! filter.matches ) {
        return;
    }

    filter_[ observer ] = filter;
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::removeObserver( const RecipientRef& observer )
{
#ifdef BUGLE_ENABLE
    std::scoped_lock lock( observerMutex_ );

    observers_.erase( observer );
    filter_.erase( observer );
#endif
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

        Letter letter {};

        while ( ! letters_.empty() )
        {
            if ( shouldExit_ ) {
                return;
            }

            onRoute_ = true;

            {
                std::scoped_lock lock( queueMutex_ );
                letter = std::move( letters_.front() );
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

            for ( auto& observerRef : observers_ )
            {
                if ( shouldExit_ ) {
                    return;
                }

                if ( filter_.contains( observerRef ) )
                {
                    const auto& filter = filter_.at( observerRef );

                    if ( ! filter.matches( letter ) ) {
                        continue;
                    }
                }

                if ( observerRef.expired() ) {
                    continue;
                }

                auto observer = observerRef.lock();
                observer->receive( letter );
            };

            onRoute_ = false;
        }
    }
}


}   //  ::bugle
