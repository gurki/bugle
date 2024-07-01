#pragma once

#include "bugle/core/filter.h"
#include "bugle/core/letter.h"
#include "bugle/core/envelope.h"
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

using RecipientRef = std::weak_ptr<class Recipient>;
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
        void push( const std::thread::id& );
        void pop( const std::thread::id& );

        void addObserver(
            const RecipientRef& observer,
            const FilterPtr& filter = {}
        );

        void removeObserver( const RecipientRef& observer );

        void post( Letter&& );
        void post(
            const std::string& message = {},
            const tags_t& tags = {},
            const attributes_t& attributes = {},
            const std::source_location& location = std::source_location::current()
        );

        template <typename... Args>
        void memo( std::format_string<Args...> fmt, Args&&... args );

        template <typename... Args>
        void card(
            const tags_t& tags,
            std::format_string<Args...> fmt, Args&&... args
        );

        static PostOffice& instance();

    private:

        void processQueue();

        std::atomic_bool enabled_ = true;

        std::unordered_set<
            RecipientRef,
            WeakPtrHash<Recipient>,
            WeakPtrEqual<Recipient>
        > observers_;

        std::unordered_map<
            RecipientRef,
            FilterPtr,
            WeakPtrHash<Recipient>,
            WeakPtrEqual<Recipient>
        > filter_;

        std::unordered_map<std::thread::id, int> levels_;

        std::thread workerThread_;
        std::shared_mutex observerMutex_;
        std::mutex queueMutex_;
        std::deque<Letter> letters_;
        std::condition_variable queueReady_;
        std::atomic_bool shouldExit_ = false;
        std::atomic_bool onRoute_ = false;

        static PostOfficeUPtr instance_;
};


template <typename... Args>
void PostOffice::memo( std::format_string<Args...> fmt, Args&&... args ) {
    post( std::format( fmt, std::forward<Args>( args )... ) );
}

template <typename... Args>
void PostOffice::card(
    const tags_t& tags,
    std::format_string<Args...> fmt, Args&&... args )
{
    post(
        std::format( fmt, std::forward<Args>( args )... ),
        tags
    );
}


}   //  ::bugle
