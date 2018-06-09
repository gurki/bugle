#include "jsonlogger.h"

#include <json.hpp>

#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QMapIterator>

using namespace nlohmann;


////////////////////////////////////////////////////////////////////////////////
JsonLogger::JsonLogger( QObject* parent ) :
    QObject( parent )
{}


////////////////////////////////////////////////////////////////////////////////
bool JsonLogger::open( const QString& filename ) {
    file_.setFileName( filename );
    return file_.open( QFile::WriteOnly );
}


////////////////////////////////////////////////////////////////////////////////
void JsonLogger::createDefaultFile()
{
    const QString dt = QDateTime::currentDateTime().toString( "yyyy-MM-dd_HHmmss" );
    const QString name = "logs/" + dt + "_cc.json";

    QDir dir( "logs/" );
    dir.mkdir( "." );

    open( name );
}


////////////////////////////////////////////////////////////////////////////////
void JsonLogger::notify( const Message& message )
{
    if ( ! file_.isOpen() ) {
        return;
    }

    json jobj;

    if ( ! message.text().isEmpty() ) {
        jobj[ "text" ] = message.text().toStdString();
    }

    QMapIterator<QString, QVariant> iter( message.tags() );

    while( iter.hasNext() )
    {
        iter.next();

        const QString& key = iter.key();
        const bool systemKey = key.startsWith( "__" );
        json& node = systemKey ? jobj[ "system" ] : jobj;

        if ( iter.value().isValid() ) {
            node[ "rich" ][ key.toStdString() ] = iter.value().toString().toStdString();
        }
        else {
            node[ "tags" ].push_back( key.toStdString() );
        }
    }

    QTextStream str( &file_ );
    str << jobj.dump( 4 ).c_str();
    str << ",\n";
}
