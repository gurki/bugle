#include "bugle/core/envelope.h"
#include "bugle/core/postoffice.h"
#include "bugle/format/duration.h"

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
#ifdef BUGLE_ENABLE
    close();
#endif
}


////////////////////////////////////////////////////////////////////////////////
uint64_t Envelope::durationUs() const
{
    if ( open ) {
        return openedAt.elapsedUs();
    }

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>( closedAt - openedAt );
    return duration.count();
}


////////////////////////////////////////////////////////////////////////////////
void Envelope::close()
{
#ifdef BUGLE_ENABLE
    if ( ! open ) {
        return;
    }

    open = false;
    closedAt = Timestamp::now();

    const auto duration = durationUs();
    const std::string dur = durationInfo( duration );

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
