#include "bugle/core/envelope.h"
#include "bugle/core/postoffice.h"

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Envelope::Envelope(
    const std::reference_wrapper<PostOffice>& _office,
    const std::string& _title,
    const tags_t& _tags,
    const std::source_location& _location ) :
    office( _office ),
    title( _title ),
    location( _location ),
    thread( std::this_thread::get_id() ),
    tags( _tags ),
    open( true ),
    openedAt( Timestamp::now() )
{
    tags.insert( "envelope" );

#ifdef BUGLE_ENABLE
    office.get().post(
        title,
        tags,
        { { "open", true } },
        location
    );
    office.get().push( thread );
#endif
}


////////////////////////////////////////////////////////////////////////////////
Envelope::~Envelope() {
    close();
}


////////////////////////////////////////////////////////////////////////////////
uint64_t Envelope::durationUs() const
{
    const auto end = open ? std::chrono::steady_clock::now() : stopped_;
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end - started_ );
    return duration.count();
}


////////////////////////////////////////////////////////////////////////////////
void Envelope::close()
{
    if ( ! open ) {
        return;
    }

    open = false;
    stopped_ = std::chrono::steady_clock::now();
    closedAt = Timestamp::now();
#ifdef BUGLE_ENABLE
    const auto duration = durationUs();

    attributes_t attributes = {
        { "duration", duration },
        { "open", false }
    };

    office.get().pop( thread );
    office.get().post(
        title,
        tags,
        attributes,
        location
    );
#endif
}


}   //  ::bugle
