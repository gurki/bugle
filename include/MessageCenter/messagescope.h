#pragma once

#ifndef MC_DISABLE_SCOPE
    #define MC_ENTER MessageScope __message_scope( MC_INFO )
#else
    #define MC_ENTER
#endif


#include <QMutex>
#include <QThread>

#include "messageinfo.h"
#include "message.h"


class MessageScope
{
    public:

        MessageScope( MC_INFO_DEFINE );
        ~MessageScope();

        QString timeInfo() const;
        int level() const { return level_; }

    private:

        MessageScope();

        Message message_;
        QTime time_;
        int level_ = -1;
};
