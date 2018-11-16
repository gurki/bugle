#pragma once

#include "messagecenter/messageinfo.h"
#include "messagecenter/utility.h"
//#include "messagescope.h"
//#include "colortable.h"
//#include "booleanfilter.h"

#include <nlohmann/json.hpp>

#include <memory>
#include <mutex>
#include <unordered_set>


#define MC ( mc::MessageCenter::instance() )

#define PP_THIRD_ARG(a,b,c,...) c
#define VA_OPT_SUPPORTED_I(...) PP_THIRD_ARG(__VA_OPT__(,),true,false,)
#define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)

#ifdef MC_DISABLE_POST
    #define MC_POST( text )
    #define MC_POST_TAGGED( text, ... )
#else
    #ifdef VA_OPT_SUPPORTED
        #define MC_POST( text, ... ) ( MC.post( text, MC_INFO __VA_OPT__(, __VA_ARGS__) ))
    #else
        #define MC_POST( text ) ( MC.post( text, {}, MC_INFO ) )
        #define MC_POST_TAGGED( text, ... ) ( MC.post( text, MC_INFO, __VA_ARGS__ ) )
    #endif
#endif

#ifdef VA_OPT_SUPPORTED
    #define MCP MC_POST
    #define MCT MC_POST
#else
    #define MCP MC_POST
    #define MCT MC_POST_TAGGED
#endif


namespace mc {


class Message;
class MessageObserver;

using MessageObserverRef = std::weak_ptr<MessageObserver>;
using MessageCenterPtr =  std::shared_ptr<class MessageCenter>;


class MessageCenter
{
    public:

        MessageCenter() {}
        ~MessageCenter() {}

        void enable() { enabled_ = true; }
        void disable() { enabled_ = false; }
        
        void addObserver(
            const MessageObserverRef& observer,
            const std::string& filter
        );

        void post(
            const nlohmann::json& object,
            MC_INFO_DEFINE_DEFAULT,
            const nlohmann::json& tags = {}
        );

        void postMessage( const Message& text );

        static MessageCenter& instance() { return *instance_; }

    private:

        bool enabled_ = true;
        std::unordered_set<
            MessageObserverRef,
            WeakPtrHash<MessageObserver>,
            WeakPtrEqual<MessageObserver> >
            observers_;

        std::mutex observerMutex_;

        static MessageCenterPtr instance_;
};


}   //  mc::
