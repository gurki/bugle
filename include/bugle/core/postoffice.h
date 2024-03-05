#pragma once

#include "bugle/core/booleanfilter.h"
#include "bugle/core/message.h"
#include "bugle/core/defines.h"  //  MC_INFO, ...
#include "bugle/core/scope.h"
#include "bugle/utility/utility.h"  //  WeakPtrHash, WeakPtrEqual

#include <nlohmann/json.hpp>

#include <atomic>   //  enabled_, ...
#include <condition_variable>
#include <deque>
#include <format>
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
using PostOfficePtr = std::shared_ptr<class PostOffice>;
using PostOfficeUPtr = std::unique_ptr<class PostOffice>;


class PostOffice
{
    public:

        PostOffice();
        ~PostOffice();

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
            std::format_string<Args...> message,
            const nlohmann::json& tags = {},
            Args... args
        );

        static PostOffice& instance();

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

        static PostOfficeUPtr instance_;
};


////////////////////////////////////////////////////////////////////////////////
template<typename... Args>
void PostOffice::post(
    BUGLE_INFO_DECLARE,
    std::format_string<Args...> message,
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
        const std::string content = std::format( message, std::forward<Args>(args)... );
        std::unique_lock lock( queueMutex_ );
        messages_.push_back( { file, func, line, level, tid, content, tags } );
    }

    queueReady_.notify_one();
}


}   //  ::bugle
