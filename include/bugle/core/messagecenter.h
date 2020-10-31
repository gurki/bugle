#pragma once

#include "bugle/core/booleanfilter.h"
#include "bugle/core/defines.h"  //  MC_INFO, ...
#include "bugle/core/scope.h"
#include "bugle/utility/utility.h"  //  WeakPtrHash, WeakPtrEqual

#include <nlohmann/json.hpp>

#include <atomic>   //  enabled_, ...
#include <condition_variable>
#include <deque>
#include <memory>   //  MessageCenterPtr, ...
#include <mutex>    //  observerMutex_, ...
#include <shared_mutex>
#include <stack>
#include <thread>
#include <unordered_map>
#include <unordered_set>    //  observers_, ...


namespace bugle {


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


}   //  ::bugle
