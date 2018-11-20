#pragma once

#include "messagecenter/messageinfo.h"
#include "messagecenter/utility.h"
#include "messagecenter/message.h"
//#include "messagescope.h"
//#include "colortable.h"
#include "messagecenter/booleanfilter.h"

#include <nlohmann/json.hpp>

#include <memory>
#include <mutex>
#include <unordered_set>


#define MC ( mc::MessageCenter::instance() )
#define mci MC

#define PP_THIRD_ARG(a,b,c,...) c
#define VA_OPT_SUPPORTED_I(...) PP_THIRD_ARG(__VA_OPT__(,),true,false,)
#define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)

#ifdef MC_DISABLE_POST
    #define MC_POST( text )
#else    
    #if VA_OPT_SUPPORTED
        #define MC_POST( text, ... ) ( MC.post( text, MC_INFO __VA_OPT__(, __VA_ARGS__) ))
    #else
        #define MC_POST( text, ... ) ( MC.post( text, MC_INFO, __VA_ARGS__ ) )
    #endif
#endif

#define MCP MC_POST
#define mcp MC_POST
#define mc_post MC_POST 


namespace mc {


class MessageObserver;

using MessageObserverRef = std::weak_ptr<MessageObserver>;
using MessageCenterPtr =  std::shared_ptr<class MessageCenter>;


class MessageCenter
{
    public:

        MessageCenter();
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

        void postMessage( const Message& message );

        static MessageCenter& instance() { return *instance_; }

    private:

        bool enabled_ = true;
        std::unordered_set<
            MessageObserverRef,
            WeakPtrHash<MessageObserver>,
            WeakPtrEqual<MessageObserver> > observers_;
        std::unordered_map<
            MessageObserverRef, 
            BooleanFilter, 
            WeakPtrHash<MessageObserver>,
            WeakPtrEqual<MessageObserver> > filter_;
        std::mutex observerMutex_;

        static MessageCenterPtr instance_;
};


}   //  ::mc
