#pragma once

#include <QObject>
#include <QVariantMap>
#include <QDateTime>


class Message
{
    Q_GADGET

    Q_PROPERTY( QString time READ timeInfo CONSTANT )
    Q_PROPERTY( QString text READ text CONSTANT )
    Q_PROPERTY( QVariantMap tags READ tags CONSTANT )

    public:

        Message();

        Message(
            const char* file,
            const char* function,
            const int line
        );

        Message(
            const QString& text,
            const QVariantMap& tags
        );

        void set(
            const QString& text = QString(),
            const QVariantMap& tags = QVariantMap()
        );
        void updateTimestamp();

        const QDateTime& timestamp() const { return datetime_; }
        const QString& text() const { return text_; }
        const QVariantMap& tags() const { return tags_; }
        const QString& file() const { return file_; }
        const QString& function() const { return function_; }
        int line() const { return line_; }
        int level() const { return level_; }
        Qt::HANDLE threadId() const { return threadId_; }

        QString info() const;
        QString timeInfo( const Qt::DateFormat format = Qt::ISODateWithMs ) const;
        bool hasTags() const { return ! tags_.empty(); }
        bool isIndexed() const { return line_ >= 0; }

    private:

        void addSystemTags();

        QString file_;
        QString function_;
        int line_ = -1;
        int level_ = -1;

        QDateTime datetime_;
        QString text_;
        QVariantMap tags_;
        Qt::HANDLE threadId_;
};
