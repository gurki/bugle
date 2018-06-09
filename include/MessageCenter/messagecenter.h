#pragma once

#define MC ( MessageCenter::instance() )

#ifndef MC_DISABLE_POST
    #define MC_POST( text, ... ) ( MC.post( text, MC_INFO, __VA_ARGS__ ) )
    #define MC_POST_RICH( text, ... ) ( MC.postm( text, __VA_ARGS__, MC_INFO ) )
    #define MC_POST_SINGLE( text, tag ) ( MC.posts( text, tag, MC_INFO ) )
#else
    #define MC_POST( text )
    #define MC_POST_RICH( text, ... )
    #define MC_POST_SINGLE( text, tag )
#endif

#define MC_P MC_POST
#define MC_PR MC_POST_RICH
#define MC_PS MC_POST_SINGLE

#include "messageinfo.h"
#include "messagescope.h"
#include "colortable.h"
#include "booleanfilter.h"

#include <QVariantMap>
#include <QMutex>


class Message;
class MessageObserver;


class MessageCenter : public QObject
{
    Q_OBJECT

    public:

        MessageCenter( QObject* parent = nullptr );
        ~MessageCenter();

        void enable() { enabled_ = true; }
        void disable() { enabled_ = false; }
        void setEnter( const bool enabled );
        void setLeave( const bool enabled );
        void setTrace( const bool enabled );
        void setTrace( const bool enter, const bool leave );

        static int level( const Qt::HANDLE threadId );
        static void increaseLevel( const Qt::HANDLE threadId );
        static void decreaseLevel( const Qt::HANDLE threadId );
        static MessageCenter& instance() { return instance_; }

    public slots:

        void addObserver(
            MessageObserver* observer,
            QString filter = ""
        );

        void post(
            const QString& text,
            MC_INFO_DEFINE_DEFAULT,
            const QStringList& tags = {},
            const QVariantMap& richTags = {}
        ) const;

        void posts(
            const QString& text,
            const QString& tag,
            MC_INFO_DEFINE_DEFAULT
        ) const;

        void postl(
            const QString& text,
            const QVariantList& tags,
            MC_INFO_DEFINE_DEFAULT
        ) const;

        void postm(
            const QString& text,
            const QVariantMap& tags,
            MC_INFO_DEFINE_DEFAULT
        ) const;

        void postq(
            const QString& text,
            MC_INFO_DEFINE_DEFAULT
        ) const;

        void postq(
            const QString& text,
            const QString& tag,
            MC_INFO_DEFINE_DEFAULT
        ) const;

        void postq(
            const QString& text,
            const QVariantList& tags,
            MC_INFO_DEFINE_DEFAULT
        ) const;

        void post( const Message& text ) const;

    signals:

        void posted( const Message& message ) const;

    private:

        bool enter_ = false;
        bool leave_ = false;
        bool enabled_ = true;
        QMap<MessageObserver*, BooleanFilter*> observers_;

        static QMap<Qt::HANDLE, int> levels_;
        static QMutex levelMutex_;
        static MessageCenter instance_;
};
