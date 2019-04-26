#pragma once

#include "messagecenter/defines.h"  //  MC_INFO, ...
#include "messagecenter/utility.h"  //  WeakPtrHash, WeakPtrEqual
#include "messagecenter/booleanfilter.h"
#include "messagecenter/scope.h"

#include <nlohmann/json.hpp>

#include <thread>
#include <memory>   //  MessageCenterPtr, ...
#include <mutex>    //  observerMutex_, ...
#include <shared_mutex>
#include <unordered_set>    //  observers_, ...
#include <atomic>   //  enabled_, ...
#include <deque>
#include <condition_variable>
#include <stack>
#include <unordered_map>


namespace mc {


class Message;
class BooleanFilter;

using ScopeWrp = std::reference_wrapper<Scope>;
using ObserverRef = std::weak_ptr<class Observer>;
using MessageCenterPtr = std::shared_ptr<class MessageCenter>;


class MessageCenter
{
    public:

        MessageCenter();
        ~MessageCenter();

        void enable() { enabled_ = true; }
        void disable() { enabled_ = false; }

        void pushScope( Scope& scope );
        void popScope( const Scope& scope );
        
        void addObserver(
            const ObserverRef& observer,
            const std::string& filter = {}
        );

        void removeObserver( const ObserverRef& observer );

        void post(
            const nlohmann::json& content,
            MC_INFO_DECLARE_DEFAULT,
            const nlohmann::json& tags = {}
        );

        void lineskip();

        static MessageCenter& instance() { return *instance_; }

    private:

        struct MessageData 
        {    
            nlohmann::json content = {};
            const char* file = nullptr;
            const char* func = nullptr;
            int line = -1;
            int level = -1;
            nlohmann::json tags = {};
            std::thread::id threadId = {};
        };

        [[ noreturn ]] void processQueue();

        std::atomic_bool enabled_ = true;

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

        std::unordered_map< 
            std::thread::id, 
            std::stack<ScopeWrp> 
        > scopes_;

        std::thread workerThread_;
        std::shared_mutex observerMutex_;
        std::mutex queueMutex_;
        std::deque<MessageData> messages_;
        std::condition_variable queueReady_;
        std::atomic_bool shouldExit_ = false;

        static MessageCenterPtr instance_;
};


}   //  ::mc
