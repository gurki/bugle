#include "bugle/core/postoffice.h"
#include "bugle/core/letter.h"
#include "bugle/core/recipient.h"

namespace bugle {


std::unordered_map<std::thread::id, std::string> PostOffice::threadNames_ = {};
std::shared_mutex PostOffice::threadNameMutex_;


////////////////////////////////////////////////////////////////////////////////
PostOffice& PostOffice::instance()
{
    static PostOffice office;
    return office;
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

    {
        std::scoped_lock lock( queueMutex_ );
        shouldExit_ = true;
    }

    queueReady_.notify_one();
    workerThread_.join();
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::flush()
{
#ifdef BUGLE_ENABLE
    std::unique_lock lock( queueMutex_ );

    queueDrained_.wait( lock, [ this ](){
        return letters_.empty() && ! dispatching_;
    });
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
        letters_.emplace_back( std::move( letter ) );
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
    std::scoped_lock lock( levelMutex_ );

    const auto it = levels_.find( thread );

    if ( it == levels_.end() ) {
        return 0;
    }

    return it->second;
#else
    return 0;
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::push( const std::thread::id& thread )
{
#ifdef BUGLE_ENABLE
    std::scoped_lock lock( levelMutex_ );
    levels_[ thread ]++;
#endif
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::pop( const std::thread::id& thread )
{
#ifdef BUGLE_ENABLE
    std::scoped_lock lock( levelMutex_ );

    const auto it = levels_.find( thread );

    if ( it == levels_.end() ) {
        return;
    }

    if ( --it->second <= 0 ) {
        levels_.erase( it );
    }
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
void PostOffice::registerThread( const std::source_location& location )
{
    if ( threadName().has_value() ) {
        return;
    }

    //  reuse the letter's function name cleanup
    setThreadName( Letter( {}, {}, {}, location ).functionInfo() );
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::setThreadName( const std::string& name ) {
    setThreadName( name, std::this_thread::get_id() );
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::setThreadName( const std::string& name, const std::thread::id id )
{
    std::unique_lock lock( threadNameMutex_ );
    threadNames_[ id ] = name;
}


////////////////////////////////////////////////////////////////////////////////
std::optional<std::string> PostOffice::threadName() {
    return threadName( std::this_thread::get_id() );
}


////////////////////////////////////////////////////////////////////////////////
std::optional<std::string> PostOffice::threadName( const std::thread::id id )
{
    std::shared_lock lock( threadNameMutex_ );

    const auto it = threadNames_.find( id );

    if ( it == threadNames_.end() ) {
        return {};
    }

    return it->second;
}


////////////////////////////////////////////////////////////////////////////////
void PostOffice::processQueue()
{
    while ( true )
    {
        Letter letter {};

        {
            std::unique_lock lock( queueMutex_ );

            queueReady_.wait( lock, [ this ](){
                return ! letters_.empty() || shouldExit_;
            });

            //  exit only once the queue is fully drained, so no letters are
            //  lost on shutdown
            if ( letters_.empty() ) {
                queueDrained_.notify_all();
                return;
            }

            letter = std::move( letters_.front() );
            letters_.pop_front();
            dispatching_ = true;
        }

        //  remove expired observers under exclusive lock before dispatching
        {
            std::scoped_lock lock( observerMutex_ );

            for ( auto it = observers_.begin(); it != observers_.end(); )
            {
                if ( ! it->expired() ) {
                    ++it;
                    continue;
                }

                filter_.erase( *it );
                it = observers_.erase( it );
            }
        }

        {
            //  lock to avoid observer insertion from another thread during iteration.
            //  as a side effect, this ensures letters are dispatched sequentially,
            //  keeping chronological order.
            std::shared_lock lock( observerMutex_ );

            for ( const auto& observerRef : observers_ )
            {
                if ( filter_.contains( observerRef ) )
                {
                    const auto& filter = filter_.at( observerRef );

                    if ( ! filter.matches( letter ) ) {
                        continue;
                    }
                }

                if ( auto observer = observerRef.lock() ) {
                    observer->receive( letter );
                }
            }
        }

        {
            std::scoped_lock lock( queueMutex_ );
            dispatching_ = false;

            if ( letters_.empty() ) {
                queueDrained_.notify_all();
            }
        }
    }
}


}   //  ::bugle
