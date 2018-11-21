#pragma once

#include "messagecenter/defines.h"
#include "messagecenter/message.h"


//#define MCS ( mc::MessageScope( {}, MC_INFO_VALUES ) )
//#define MCS ( mc::MessageScope( MC_INFO_VALUES ) )
#define MCS(...) ( mc::MessageScope( { __VA_ARGS__ }, MC_INFO_VALUES ) )
#define MCE ( mc::MessageScope::EmitMessage )


namespace mc {


class MessageScope
{
    public:

        enum Flag {
            EmitMessage
        };

        MessageScope( MC_INFO_DECLARE_DEFAULT );
        MessageScope( const nlohmann::json& tags = {}, MC_INFO_DECLARE_DEFAULT );
        ~MessageScope();

        void append( const nlohmann::json& object );
        void emit();

        MessageScope& operator << ( const nlohmann::json& object );
        MessageScope& operator << ( const MessageScope::Flag flag );

    private:

        MessageScope() {}
        Message message_;

        bool dirty_ = true;
};




}   //  mc::
