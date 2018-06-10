#pragma once

#include "messagecenter/messageinfo.h"
#include "messagecenter/utility.h"
//#include "messagescope.h"
//#include "colortable.h"
//#include "booleanfilter.h"

#include <nlohmann/json.hpp>

#include <memory>
#include <mutex>
#include <unordered_set>


#define MC ( mc::MessageCenter::instance() )

#ifdef MC_DISABLE_POST
    #define MC_POST( text )
    #define MC_POST_RICH( text, ... )
#else
    #define MC_POST( text ) ( MC.post( text, MC_INFO ) )
    #define MC_POST_RICH( text, ... ) ( MC.post( text, __VA_ARGS__, MC_INFO ) )
#endif

#define MC_PROST MC_POST_RICH


namespace mc {


class Message;
class MessageObserver;
using MessageObserverRef = std::weak_ptr<MessageObserver>;
using MessageCenterPtr =  std::shared_ptr<class MessageCenter>;


class MessageCenter
{
    public:

        enum Flag {
            EndOfMessage
        };

        MessageCenter();
        ~MessageCenter();

        void enable() { enabled_ = true; }
        void disable() { enabled_ = false; }
//        void setEnter( const bool enabled );
//        void setLeave( const bool enabled );
//        void setTrace( const bool enabled );
//        void setTrace( const bool enter, const bool leave );

//        static int level( const Qt::HANDLE threadId );
//        static void increaseLevel( const Qt::HANDLE threadId );
//        static void decreaseLevel( const Qt::HANDLE threadId );
        static MessageCenter& instance() { return *instance_; }


        void addObserver(
            const MessageObserverRef& observer,
            const std::string& filter
        );

        void post(
            const nlohmann::json& object,
            MC_INFO_DEFINE_DEFAULT
        ) const;

        void post(
            const nlohmann::json& object,
            const nlohmann::json& tags,
            MC_INFO_DEFINE_DEFAULT
        ) const;

//        void posts(
//            const QString& text,
//            const QString& tag,
//            MC_INFO_DEFINE_DEFAULT
//        ) const;

//        void postl(
//            const QString& text,
//            const QVariantList& tags,
//            MC_INFO_DEFINE_DEFAULT
//        ) const;

//        void postm(
//            const QString& text,
//            const QVariantMap& tags,
//            MC_INFO_DEFINE_DEFAULT
//        ) const;

        void post( const Message& text ) const;

    private:

//        bool enter_ = false;
//        bool leave_ = false;
        bool enabled_ = true;
        mutable std::unordered_set<
            MessageObserverRef,
            WeakPtrHash<MessageObserver>,
            WeakPtrEqual<MessageObserver> >
            observers_;

        std::mutex observerMutex_;

//        static QMap<Qt::HANDLE, int> levels_;
//        static QMutex levelMutex_;
        static MessageCenterPtr instance_;
};


const MessageCenter& operator << (
    const MessageCenter& msgcntr,
    const nlohmann::json& data
);


const MessageCenter& operator << (
    const MessageCenter& msgcntr,
    const MessageCenter::Flag flag
);


}   //  mc::
