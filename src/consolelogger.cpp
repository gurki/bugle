#include "messagecenter/consolelogger.h"
#include "messagecenter/colortable.h"

#include <iostream>
#include <thread>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
ConsoleLogger::ConsoleLogger() {
    formatter_ = std::make_shared<Formatter>();
    printSystemInfo();
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::setFormatter( const FormatterPtr& formatter ) {
    formatter_ = formatter;
}


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::notify( const Message& message )
{
    std::cout << "      " << DateTime::now().timeInfo() << " -- enter notify " << std::this_thread::get_id() << std::endl;

    if ( ! formatter_ ) {
        return;
    }

    std::cout << formatter_->format( message ) << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::printSystemInfo() const
{
//    formatter_->pushType( Formatter::AnsiEncoding );

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


}   //  mc::
