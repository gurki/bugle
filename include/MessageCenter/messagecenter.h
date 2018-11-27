#pragma once

#include "messagecenter/defines.h"  //  MC_INFO, ...
#include "messagecenter/utility.h"  //  WeakPtrHash, WeakPtrEqual
#include "messagecenter/booleanfilter.h"

#include <nlohmann/json.hpp>

#include <memory>   //  MessageCenterPtr, ...
#include <mutex>    //  observerMutex_, ...
#include <unordered_set>    //  observers_, ...
#include <atomic>   //  enabled_, ...
#include <queue>
#include <condition_variable>
#include <future>


namespace mc {


class Message;
class BooleanFilter;

using ObserverRef = std::weak_ptr<class Observer>;
using MessageCenterPtr = std::shared_ptr<class MessageCenter>;


class MessageCenter
{
    public:

        MessageCenter();
        ~MessageCenter();

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

        void terminate();

        static MessageCenter& instance() { return *instance_; }

    private:

        void postAsync(
            const nlohmann::json& content,
            MC_INFO_DECLARE,
            const nlohmann::json& tags,
            const std::thread::id& threadId
        );

        void processQueue();
        
        std::atomic_bool enabled_ = true;
        std::atomic_bool alive_ = true;

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
        std::mutex futureMutex_, workerMutex_;
        std::queue< std::future<void> > futures_;
        std::thread worker_;
        std::condition_variable cv_;

        static MessageCenterPtr instance_;
};


}   //  ::mc
