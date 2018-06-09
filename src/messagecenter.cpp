#include "messagecenter.h"
#include "message.h"
#include "messageobserver.h"
#include "messageutility.h"

#include <QThread>
#include <QDebug>

#include <cstdarg>
#include <algorithm>


////////////////////////////////////////////////////////////////////////////////
MessageCenter MessageCenter::instance_;
QMap<Qt::HANDLE, int> MessageCenter::levels_;
QMutex MessageCenter::levelMutex_;


////////////////////////////////////////////////////////////////////////////////
MessageCenter::MessageCenter( QObject* parent ) :
    QObject( parent )
{}


////////////////////////////////////////////////////////////////////////////////
MessageCenter::~MessageCenter()
{
#ifndef MC_DISABLE_POST

    QMapIterator<MessageObserver*, BooleanFilter*> iter( observers_ );

    while ( iter.hasNext() ) {
        iter.next().value()->deleteLater();
    }

#endif
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::setTrace( const bool enabled ) {
    setTrace( enabled, enabled );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::setTrace( const bool enter, const bool leave ) {
    setEnter( enter );
    setLeave( leave );
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::setEnter( const bool enabled )
{
    if ( enter_ == enabled ) {
        return;
    }

    enter_ = enabled;
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::setLeave( const bool enabled )
{
    if ( leave_ == enabled ) {
        return;
    }

    leave_ = enabled;
}


////////////////////////////////////////////////////////////////////////////////
int MessageCenter::level( const Qt::HANDLE threadId )
{
    levelMutex_.lock();
    const int level = levels_[ threadId ];
    levelMutex_.unlock();

    return level;
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::increaseLevel( const Qt::HANDLE threadId ) {
    levelMutex_.lock();
    levels_[ threadId]++;
    levelMutex_.unlock();
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::decreaseLevel( const Qt::HANDLE threadId ) {
    levelMutex_.lock();
    levels_[ threadId ]--;
    levelMutex_.unlock();
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::addObserver(
    MessageObserver* observer,
    QString filter )
{
#ifndef MC_DISABLE_POST

    if ( ! observers_.contains( observer ) )
    {
        auto filter = new BooleanFilter();
        filter->moveToThread( this->thread() );

        observers_[ observer ] = filter;
    }

    observers_[ observer ]->unite( filter );

#endif
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::posts(
    const QString& text,
    const QString& tag,
    MC_INFO_DEFINE ) const
{
#ifndef MC_DISABLE_POST
    postm( text, {{ tag, QVariant() }}, MC_INFO_NAMES );
#endif
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post(
    const QString& text,
    MC_INFO_DEFINE,
    const QStringList& tags,
    const QVariantMap& richTags ) const
{
#ifndef MC_DISABLE_POST

    QVariantMap map;

    for ( const QString& tag : tags ) {
        map[ tag ] = QVariant();
    }

    map.unite( richTags );
    postm( text, map, MC_INFO_NAMES );

#endif
}



////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postl(
    const QString& text,
    const QVariantList& tags,
    MC_INFO_DEFINE ) const
{
#ifndef MC_DISABLE_POST

    QVariantMap richTags;

    for ( const auto& tag : tags )
    {
        if ( tag.type() == QVariant::String ) {
            richTags[ tag.toString() ] = QVariant();
        }
        else if ( tag.type() == QVariant::Map )
        {
            const QVariantMap map = tag.toMap();
            QMapIterator<QString, QVariant> iter( map );

            while ( iter.hasNext() )
            {
                const auto item = iter.next();
                const auto& key = item.key();
                const QVariant& value = item.value();

                richTags[ key ] = mc::parseVariant( value );
            }
        }
    }

    postm( text, richTags, MC_INFO_NAMES );

#endif
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postm(
    const QString& text,
    const QVariantMap& tags,
    MC_INFO_DEFINE ) const
{
#ifndef MC_DISABLE_POST
    Message msg( MC_INFO_NAMES );
    msg.set( text, tags );
    post( msg );
#endif
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postq(
    const QString& text,
    MC_INFO_DEFINE ) const
{
#ifndef MC_DISABLE_POST
    const QString tag = "__qml";
    postm( text, {{ tag, QVariant() }}, MC_INFO_NAMES );
#endif
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postq(
    const QString& text,
    const QString& tag,
    MC_INFO_DEFINE ) const
{
#ifndef MC_DISABLE_POST
    QStringList qtags = { tag };
    qtags.append( "__qml" );
    post( text, MC_INFO_NAMES, qtags );
#endif
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::postq(
        const QString& text,
        const QVariantList& tags,
        MC_INFO_DEFINE ) const
{
#ifndef MC_DISABLE_POST
    QVariantList qtags = tags;
    qtags.append( "__qml" );
    postl( text, qtags, MC_INFO_NAMES );
#endif
}


////////////////////////////////////////////////////////////////////////////////
void MessageCenter::post( const Message& message ) const
{
#ifndef MC_DISABLE_POST

    if ( ! enabled_ ) {
        return;
    }

    #ifndef MC_DISABLE_SCOPE
        if ( message.tags().contains( "__scope" ) )
        {
            if ( ! enter_ && message.tags()[ "__scope" ] == "enter" ) {
                return;
            }

            if ( ! leave_ && message.tags()[ "__scope" ] == "leave" ) {
                return;
            }
        };
    #endif

    QMapIterator<MessageObserver*, BooleanFilter*> iter( observers_ );

    while ( iter.hasNext() )
    {
        iter.next();

        const auto observer = iter.key();
        const auto filter = iter.value();

        if ( filter->passes( message.tags() ) ) {
            observer->notify( message );
        }
    }

    //  notify direct connections

    emit posted( message );

#endif
}
