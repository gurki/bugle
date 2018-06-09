#pragma once

#include "message.h"
#include "messageobserver.h"
#include "messageformatter.h"

#include <json.hpp>

#include <QAbstractListModel>


class MessageList : public QAbstractListModel, public MessageObserver
{
    Q_OBJECT
    Q_PROPERTY( int size READ rowCount NOTIFY sizeChanged )

    public:

        enum Roles {
            PrettySummaryRole = Qt::UserRole + 1,
            RichTagsRole,
            TimestampRole
        };
        Q_ENUM( Roles )

        MessageList(
            const int maxRingSize = -1,
            QObject* parent = nullptr
        );

        void setFormatter( MessageFormatter* formatter_ );

        virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
        virtual QVariant data( const QModelIndex& index, int role ) const;
        virtual QHash<int, QByteArray> roleNames() const;

        static void to_json( nlohmann::json& json, const MessageList& list ) {}
        static void from_json( const nlohmann::json& json, MessageList& list ) {}

    signals:

        void hasBeenNotified() const;
        void sizeChanged() const;
        void fullRingBufferChanged() const;

    public slots:

        virtual void notify( const Message& message );

    private:

        int ringIndex_ = 0;
        int ringSize_ = 0;
        const int maxRingSize_ = -1;

        QVector<Message> messages_;
        MessageFormatter* formatter_ = nullptr;
};
