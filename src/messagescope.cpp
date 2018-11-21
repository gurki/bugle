#include "messagecenter/messagescope.h"
#include "messagecenter/messagecenter.h"
//#include "colortable.h"
//#include "message.h"
#include <iostream>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
MessageScope::MessageScope( MC_INFO_DECLARE ) :
    MessageScope( {}, MC_INFO_NAMES )
{}


////////////////////////////////////////////////////////////////////////////////
MessageScope::MessageScope( const nlohmann::json& tags, MC_INFO_DECLARE ) {
    message_ = Message( MC_INFO_NAMES, nlohmann::json::array(), tags );
}


////////////////////////////////////////////////////////////////////////////////
MessageScope::~MessageScope() {
    emit();
}


////////////////////////////////////////////////////////////////////////////////]
void MessageScope::emit()
{
    if ( ! dirty_ ) {
        return;
    }

    MC.postMessage( message_ );
    dirty_ = false;
}


////////////////////////////////////////////////////////////////////////////////]
void MessageScope::append( const nlohmann::json& object ) {
    // TODO
    // message_.content_.push_back( object );
    dirty_ = true;
}


////////////////////////////////////////////////////////////////////////////////]
MessageScope& MessageScope::operator << ( const nlohmann::json& object ) {
    append( object );
    return *this;
}


////////////////////////////////////////////////////////////////////////////////]
MessageScope& MessageScope::operator << ( const MessageScope::Flag flag )
{
    if ( flag == MessageScope::EmitMessage ) {
        emit();
    }

    return *this;
}


}   //  mc::
