#pragma once

#include "message.h"

#include <QTextStream>
#include <QMap>
#include <QColor>
#include <QPair>
#include <QStack>


class MessageFormatter : public QObject
{
    Q_OBJECT

    public:

        enum FormatType {
            Default,
            Plaintext,
            AnsiEncoding,
            Html
        };
        Q_ENUM( FormatType )

        MessageFormatter( QObject* parent = nullptr );

        QString format( const Message& message ) const;
        void clearTagColors() { colors_.clear(); }
        void pushType( const FormatType type );
        void popType();

        void setDateFormat( const Qt::DateFormat format );
        void setIndent( const uint8_t spaces );
        void setTagColor( const QString& tag, const uint8_t key );
        void setTagColor( const QString& tag, const uint8_t key, const uint8_t value );

        FormatType type() const;
        bool hasTagColor( const QString& tag );
        const auto& tagColors() const { return colors_; }
        QPair<QString, QString> tagColorCodes( const QString& tag );

        QString spaces( const uint8_t count ) const;
        QString colorized(
            const QString& text,
            const uint8_t index
        ) const;

    private:

        QString spacer() const;
        QString space() const;
        QString beginColor( const uint8_t index ) const;
        QString endColor() const;

        QString tagInfo( const QVariantMap& tags ) const;
        QString levelIndent( const Message& message ) const;

        bool systemTags_ = false;
        uint8_t indent_ = 0;
        Qt::DateFormat format_ = Qt::ISODate;
        QStack<FormatType> types_;
        QMap<QString, QPair<uint8_t, uint8_t> > colors_;
};
