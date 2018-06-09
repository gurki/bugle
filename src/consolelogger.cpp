#include "messagecenter/consolelogger.h"
//#include "messagescope.h"
//#include "colortable.h"
//#include "messagecenter.h"
//#include "systeminfo.h"
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
ConsoleLogger::ConsoleLogger()
{
//    formatter_ = new MessageFormatter( this );
    printSystemInfo();
}


//////////////////////////////////////////////////////////////////////////////////
//void ConsoleLogger::setFormatter( MessageFormatter* formatter )
//{
//    if ( formatter == formatter_ ) {
//        return;
//    }

//    formatter_ = formatter;
//}


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::notify( const Message& message )
{
//    formatter_->pushType( MessageFormatter::AnsiEncoding );

//    QString msg = formatter_->format( message );
//    qDebug().noquote() << msg;

//    formatter_->popType();

    std::cout << message.text_ << message.tags_ << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::printSystemInfo() const
{
//    formatter_->pushType( MessageFormatter::AnsiEncoding );

//    //  title

//    const QString title = QCoreApplication::applicationName().toUpper();

//    qDebug().noquote() << " ";
//    qDebug().noquote() << formatter_->colorized( title, 253 );
//    qDebug().noquote() << " ";

//    //  build

//    qDebug().noquote() << formatter_->colorized( "BUILD", 251 );

//    const auto& buildInfo = SystemInfo::buildInfo();

//    for ( const auto& key : SystemInfo::orderedBuildKeys() ) {
//        qDebug().noquote() <<
//            formatter_->colorized( key, 242 ) <<
//            formatter_->colorized( buildInfo[ key ], 240 );
//    }

//    qDebug().noquote() << " ";

//    //  session

//    qDebug().noquote() << formatter_->colorized( "SESSION", 251 );

//    const auto& sessionInfo = SystemInfo::sessionInfo();

//    for ( const auto& key : SystemInfo::orderedSessionKeys() ) {
//        qDebug().noquote() <<
//            formatter_->colorized( key, 242 ) <<
//            formatter_->colorized( sessionInfo[ key ], 240 );
//    }


//    qDebug().noquote() << " ";

//    formatter_->popType();
}
