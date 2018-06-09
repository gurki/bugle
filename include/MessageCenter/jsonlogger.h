#pragma once

#include "messageobserver.h"
#include <QFile>


class JsonLogger : public QObject, public MessageObserver
{
    Q_OBJECT

    public:

        explicit JsonLogger( QObject* parent = nullptr );

        bool open( const QString& filename );
        void createDefaultFile();

    public slots:

        virtual void notify( const Message& message );

    private:

        QFile file_;
};
