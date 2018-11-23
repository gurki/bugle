#pragma once

#include "messagecenter/defines.h"  //  MC_INFO...
#include "messagecenter/utility.h"  //  WeakPtrHash, WeakPtrEqual
#include "messagecenter/booleanfilter.h"

#include <nlohmann/json.hpp>

#include <memory>
#include <mutex>
#include <unordered_set>    


namespace mc {


class Message;
class BooleanFilter;

using ObserverRef = std::weak_ptr<class Observer>;
using MessageCenterPtr = std::shared_ptr<class MessageCenter>;


class MessageCenter
{
    public:

        MessageCenter();
        ~MessageCenter() {}

        void enable() { enabled_ = true; }
        void disable() { enabled_ = false; }
        
        void addObserver(
            const ObserverRef& observer,
            const std::string& filter = {}
        );

        void post(
            const nlohmann::json& content,
            MC_INFO_DECLARE_DEFAULT,
            const nlohmann::json& tags = {}
        );

        void postMessage( const Message& message );

        static MessageCenter& instance() { return *instance_; }

    private:

        bool enabled_ = true;

        std::unordered_set<
            ObserverRef,
            WeakPtrHash<Observer>,
            WeakPtrEqual<Observer> 
        > observers_;

        std::unordered_map<
            ObserverRef, 
            BooleanFilter, 
            WeakPtrHash<Observer>,
            WeakPtrEqual<Observer> 
        > filter_;

        std::mutex observerMutex_;

        static MessageCenterPtr instance_;
};


}   //  ::mc
