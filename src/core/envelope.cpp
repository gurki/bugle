#include "bugle/core/envelope.h"
#include "bugle/core/postoffice.h"

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Envelope::Envelope(
    const std::reference_wrapper<PostOffice>& _office,
    const std::string& _title,
    const std::source_location& _location ) :
    Letter( _title, { "envelope" }, {}, _location ),
    office( _office ),
    open( true )
{
#ifdef BUGLE_ENABLE
    level = office.get().level( thread );

    attributes[ "envelope.open" ] = true;
    office.get().post( message, tags, attributes, location );
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
        return timestamp.elapsedUs();
    }

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>( closedAt - timestamp );
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
    attributes[ "envelope.duration" ] = durationUs();
    attributes[ "envelope.open" ] = false;
    office.get().pop( thread );
    office.get().post( message, tags, attributes, location );
#endif
}


}   //  ::bugle
