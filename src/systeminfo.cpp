#include "systeminfo.h"

#include <QSysInfo>
#include <QDateTime>
#include <QLocale>
#include <QDir>
#include <QThread>
#include <QCoreApplication>


////////////////////////////////////////////////////////////////////////////////
const QMap<QString, QString>& SystemInfo::buildInfo()
{
    static const QTime buildTime = QTime::fromString( __TIME__, "hh:mm:ss" );
    static const QDate buildDate = QLocale( QLocale::English ).toDate(
        QString( __DATE__ ).simplified(), "MMM d yyyy"
    );

    static const QMap<QString, QString> info = {
        { "date", buildDate.toString( Qt::ISODate ) },
        { "time", buildTime.toString( Qt::ISODate ) },
        { "os", QSysInfo::prettyProductName() },
        { "arch", QSysInfo::currentCpuArchitecture() },
        { "host", QSysInfo::machineHostName() },
        { "qt", ( QString( "v" ) + QT_VERSION_STR ) },
        { "app", QCoreApplication::applicationVersion() },
        { "comp", compilerInfo() },
        { "lang", languageInfo() }
    };

    return info;
}


////////////////////////////////////////////////////////////////////////////////
const QMap<QString, QString>& SystemInfo::sessionInfo()
{
    static const QMap<QString, QString> info = {
        { "date", QDate::currentDate().toString( Qt::ISODate ) },
        { "time", QTime::currentTime().toString( Qt::ISODate ) },
        { "bin", QDir::current().relativeFilePath( QCoreApplication::applicationFilePath() )},
        { "path", QDir::currentPath() },
        { "tid", QString::number( (quint64)QThread::currentThreadId() )},
        { "pid", QString::number( QCoreApplication::applicationPid() ) }
    };

    return info;
}


////////////////////////////////////////////////////////////////////////////////
QString SystemInfo::applicationName() {
    return QCoreApplication::applicationName();;
}


////////////////////////////////////////////////////////////////////////////////
//  [1] http://nadeausoftware.com/articles/2012/10/c_c_tip_how_detect_compiler_name_and_version_using_compiler_predefined_macros
QString SystemInfo::compilerInfo()
{
    #ifdef _MSC_FULL_VER

        QString info = QString::number( _MSC_FULL_VER );
        info.insert( 2, '.' );
        info.insert( 5, '.' );

        #ifdef _MSC_BUILD
            info += QString::asprintf( ".%02i", _MSC_BUILD );
            info.prepend( "Microsoft Visual C++ ");
        #endif

        return info;

    #else
        return "n/a";
    #endif

    return info;
}


////////////////////////////////////////////////////////////////////////////////
//  [1] https://stackoverflow.com/questions/2324658/how-to-determine-the-version-of-the-c-standard-used-by-the-compiler
QString SystemInfo::languageInfo()
{
    #ifdef _MSVC_LANG

        switch ( _MSVC_LANG )
        {
            case 201103L: return "c++11";
            case 201402L: return "c++14";
            case 201703L: return "c++17";
            default: return "n/a";
        }

    #else
        return "n/a";
    #endif
}


////////////////////////////////////////////////////////////////////////////////
const QVector<QString>& SystemInfo::orderedBuildKeys()
{
    static const QVector<QString> keys = {
        "app", "qt", "date", "time", "host", "os", "arch", "comp", "lang"
    };

    return keys;
}


////////////////////////////////////////////////////////////////////////////////
const QVector<QString>& SystemInfo::orderedSessionKeys()
{
    static const QVector<QString> keys = {
        "date", "time", "path", "bin", "pid", "tid"
    };

    return keys;
}
