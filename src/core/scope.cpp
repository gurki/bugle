#include "bugle/core/scope.h"
#include "bugle/core/postoffice.h"

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Scope::Scope(
    const std::reference_wrapper<PostOffice>& _office,
    const std::string& _title,
    const std::source_location& _location ) :
    Letter( _title, { "scope" }, {}, _location ),
    office( _office )
{
#ifdef BUGLE_ENABLE
    level = office.get().level( this->thread );

    tags.insert( "open" );
    office.get().post( message, tags, attributes, location );
    tags.erase( "open" );

    office.get().pushScope( this->thread );
#endif
}


////////////////////////////////////////////////////////////////////////////////
Scope::~Scope() {
#ifdef BUGLE_ENABLE
    close();
#endif
}


////////////////////////////////////////////////////////////////////////////////
uint64_t Scope::durationUs() const
{
    if ( open_ ) {
        return timestamp.elapsedUs();
    }

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end_ - timestamp );
    return duration.count();
}


////////////////////////////////////////////////////////////////////////////////
void Scope::close()
{
#ifdef BUGLE_ENABLE
    if ( ! open_ ) {
        return;
    }

    open_ = false;
    end_ = Timestamp::now();
    office.get().popScope( this->thread );

    tags.insert( "close" );
    office.get().post( message, tags, attributes, location );
    tags.erase( "close" );
#endif
}


}   //  ::bugle
