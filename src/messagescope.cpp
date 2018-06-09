#include "messagescope.h"
#include "messagecenter.h"
#include "colortable.h"
#include "message.h"


////////////////////////////////////////////////////////////////////////////////
MessageScope::MessageScope( MC_INFO_DEFINE ) :
    time_( QTime::currentTime() )
{
    message_ = Message( MC_INFO_NAMES );
    message_.set( {}, { { "__scope", "enter" } } );
    MC.post( message_ );

    MessageCenter::increaseLevel( message_.threadId() );
}


////////////////////////////////////////////////////////////////////////////////
MessageScope::~MessageScope()
{
    MessageCenter::decreaseLevel( message_.threadId() );

    message_.updateTimestamp();
    message_.set( {}, {
        { "__scope", "leave" },
        { "__elapsed", time_.elapsed() }
    });

    MC.post( message_ );
}


////////////////////////////////////////////////////////////////////////////////
QString MessageScope::timeInfo() const {
    return QString::asprintf( "%.03fs", time_.elapsed() / 1000.f );
}
