#pragma once

#include "messagecenter/defines.h"
#include "messagecenter/utility.h"
#include "messagecenter/message.h"
#include "messagecenter/booleanfilter.h"

#include <nlohmann/json.hpp>

#include <memory>
#include <mutex>
#include <unordered_set>


namespace mc {


using MessageObserverRef = std::weak_ptr<class MessageObserver>;
using MessageCenterPtr = std::shared_ptr<class MessageCenter>;


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
            MC_INFO_DECLARE_DEFAULT,
            const nlohmann::json& tags = {}
        );

        void postMessage( const Message& message );

        static MessageCenter& instance() { return *instance_; }

    private:

        bool enabled_ = true;

        std::unordered_set<
            MessageObserverRef,
            WeakPtrHash<MessageObserver>,
            WeakPtrEqual<MessageObserver> 
        > observers_;

        std::unordered_map<
            MessageObserverRef, 
            BooleanFilter, 
            WeakPtrHash<MessageObserver>,
            WeakPtrEqual<MessageObserver> 
        > filter_;

        std::mutex observerMutex_;

        static MessageCenterPtr instance_;
};


}   //  ::mc
