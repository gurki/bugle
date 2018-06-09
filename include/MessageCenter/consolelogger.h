#pragma once

#include "messageobserver.h"
#include "messageformatter.h"


class ConsoleLogger : public QObject, public MessageObserver
{
    Q_OBJECT

    public:

        ConsoleLogger( QObject* parent = nullptr );

        void setFormatter( MessageFormatter* formatter );

    public slots:

        virtual void notify( const Message& message );

    private:

        void printSystemInfo() const;

        MessageFormatter* formatter_ = nullptr;
};
