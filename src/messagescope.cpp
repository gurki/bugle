#include "messagecenter/messagescope.h"
#include "messagecenter/messagecenter.h"
//#include "colortable.h"
//#include "message.h"
#include <iostream>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
MessageScope::MessageScope( MC_INFO_DEFINE ) :
    MessageScope( {}, MC_INFO_NAMES )
{}


////////////////////////////////////////////////////////////////////////////////
MessageScope::MessageScope( const nlohmann::json& tags, MC_INFO_DEFINE ) {
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

    if ( message_.payload_.size() == 1 ) {
        message_.payload_ = message_.payload_.front();
    }

    if ( message_.tags_.size() == 1 ) {
        message_.tags_ = message_.tags_.front();
    }

    MC.post( message_ );
    dirty_ = false;
}


////////////////////////////////////////////////////////////////////////////////]
void MessageScope::append( const nlohmann::json& object ) {
    message_.payload_.push_back( object );
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
