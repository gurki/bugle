#pragma once

#include "bugle/core/booleanfilter.h"
#include "bugle/core/message.h"
#include "bugle/core/defines.h"  //  MC_INFO, ...
#include "bugle/core/scope.h"
#include "bugle/utility/utility.h"  //  WeakPtrHash, WeakPtrEqual

#include <nlohmann/json.hpp>
#include <fmt/format.h>

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


class BooleanFilter;

using ScopeWrp = std::reference_wrapper<Scope>;
using ObserverRef = std::weak_ptr<class Observer>;
using MessageCenterPtr = std::shared_ptr<class MessageCenter>;
using MessageCenterUPtr = std::unique_ptr<class MessageCenter>;


class MessageCenter
{
    public:

        MessageCenter();
        ~MessageCenter();

        void enable() { enabled_ = true; }
        void disable() { enabled_ = false; }
        void flush();

        void pushScope( Scope& scope );
        void popScope( const Scope& scope );

        void addObserver(
            const ObserverRef& observer,
            const std::string& filter = {}
        );

        void removeObserver( const ObserverRef& observer );

        // void post(
        //     const nlohmann::json& content,
        //     BUGLE_INFO_DECLARE_DEFAULT,
        //     const nlohmann::json& tags = {}
        // );

        template<typename... Args>
        void post(
            BUGLE_INFO_DECLARE,
            const std::string& message,
            const nlohmann::json& tags = {},
            Args... args
        );

        static MessageCenter& instance();

    private:

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
        std::deque<Message> messages_;
        std::condition_variable queueReady_;
        std::atomic_bool shouldExit_ = false;

        static MessageCenterUPtr instance_;
};


////////////////////////////////////////////////////////////////////////////////
template<typename... Args>
void MessageCenter::post(
    BUGLE_INFO_DECLARE,
    const std::string& message,
    const nlohmann::json& tags,
    Args... args )
{
#ifdef MC_DISABLE_POST
    return;
#endif

    if ( ! enabled_ ) {
        return;
    }

    auto tid = std::this_thread::get_id();
    auto level = -1;

    if ( scopes_.find( tid ) != scopes_.end() ) {
        level = int( scopes_[ tid ].size() );
    }

    {
        std::unique_lock lock( queueMutex_ );
        const std::string content = fmt::format( message, args... );
        messages_.push_back( { file, func, line, level, tid, content, tags } );
    }

    queueReady_.notify_one();
}


}   //  ::bugle
