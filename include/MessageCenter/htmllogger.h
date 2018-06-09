#pragma once

#include "messageobserver.h"
#include "messageformatter.h"

#include <QFile>
#include <QTextStream>


class HtmlLogger : public QObject, public MessageObserver
{
    Q_OBJECT

    public:

        HtmlLogger( QObject* parent = nullptr );
        ~HtmlLogger();

        void createDefaultFile( const bool open = false );
        void createFile( const QString& name, const bool open = false );

        void setFormatter( MessageFormatter* formatter );

    public slots:

        virtual void notify( const Message& message );

    private:

        void writeHeader();
        void writeSystemInfo();
        void writeSessionInfo();
        void writeTitle();
        void writeDiv( const QString& key, const QString& value );

        QFile fout_;
        QTextStream stream_;
        MessageFormatter* formatter_ = nullptr;
};
