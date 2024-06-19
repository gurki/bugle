#pragma once

#include "bugle/core/booleanfilter.h"
#include "bugle/core/letter.h"
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

// using ScopeWrp = std::reference_wrapper<Scope>;
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

        int level( const std::thread::id& );
        void pushScope( const std::thread::id& );
        void popScope( const std::thread::id& );

        void addObserver(
            const ObserverRef& observer,
            const std::string& filter = {}
        );

        void removeObserver( const ObserverRef& observer );

        void post( Letter&& );
        void post(
            const std::string& message = {},
            const tags_t& tags = {},
            const attributes_t& attributes = {},
            const std::source_location& location = std::source_location::current()
        );

        static PostOffice& instance();

    private:

        void processQueue();

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

        std::unordered_map<std::thread::id, int> levels_;

        std::thread workerThread_;
        std::shared_mutex observerMutex_;
        std::mutex queueMutex_;
        std::deque<Letter> letters_;
        std::condition_variable queueReady_;
        std::atomic_bool shouldExit_ = false;

        static PostOfficeUPtr instance_;
};


}   //  ::bugle
