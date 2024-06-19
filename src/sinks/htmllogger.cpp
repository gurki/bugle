// #include "bugle/htmllogger.h"
// #include "bugle/messagecenter.h"
// #include "bugle/systeminfo.h"

// #include <QDir>
// #include <QDebug>
// #include <QThread>
// #include <QVersionNumber>


// ////////////////////////////////////////////////////////////////////////////////
// HtmlLogger::HtmlLogger( QObject* parent ) :
//     QObject( parent )
// {
//     formatter_ = new Formatter( this );
// }


// ////////////////////////////////////////////////////////////////////////////////
// HtmlLogger::~HtmlLogger()
// {
//     if ( fout_.isWritable() ) {
//         stream_ << "\n";
//         stream_ << "</html> \n";
//         stream_ << "</body> \n";
//     }
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::setFormatter( Formatter* formatter )
// {
//     if ( formatter == formatter_ ) {
//         return;
//     }

//     formatter_ = formatter;
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::createDefaultFile( const bool open )
// {
//     const QString dt = QDateTime::currentDateTime().toString( "yyyy-MM-dd_HHmmss" );
//     const QString name = "logs/" + dt + "_cc.html";

//     QDir dir( "logs/" );
//     dir.mkdir( "." );

//     createFile( name, open );
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::createFile( const QString& name, const bool open )
// {
//     if ( fout_.fileName() == name ) {
//         return;
//     }

//     fout_.close();
//     fout_.setFileName( name );

//     if ( ! fout_.open( QFile::WriteOnly | QFile::Truncate ) )
//     {
//         BUGLE_INSTANCE.postm( "couldn't open file", {
//             { "warning", {} },
//             { "detail", name }
//         });

//         fout_.setFileName( "" );
//         return;
//     }

//     stream_.setDevice( &fout_ );

//     //  copy stylesheet

//     const QString styleSource = ":/data/style.css";
//     const QString styleTarget = "logs/style.css";

//     QFile finStyle( styleSource );

//     if ( ! finStyle.open( QFile::ReadOnly ) )
//     {
//         BUGLE_INSTANCE.postm( "couldn't find stylesheet", {
//             { "warning", {} },
//             { "detail", styleSource }
//         });
//     }
//     else {
//         QFile fout( styleTarget );
//         fout.remove();
//         finStyle.copy( styleTarget );
//     }

//     //  start log

//     writeTitle();
//     writeHeader();
//     writeSystemInfo();
//     writeSessionInfo();
//     stream_ << "<br> \n";

//     //  open log file in chrome

//     if ( open ) {
//         QDir dir( fout_.fileName() );
//         const std::string call = "start chrome " + dir.absolutePath().toStdString();
//         system( call.c_str() );
//     }
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::writeHeader()
// {
//     //  font from [1]
//     //  [1] https://www.cssfontstack.com/Verdana

//     stream_ << "" \
//         "<!DOCTYPE html> \n" \
//         "<meta http-equiv='refresh' content='5'> \n" \
//         "<head> \n" \
//         "<link rel='stylesheet' type='text/css' href='style.css'> \n" \
//         "</head> \n" \
//         "<body> \n";

//     stream_.flush();
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::writeSystemInfo()
// {
//     stream_ << "<h3>";
//     stream_ << formatter_->colorized( "BUILD", 251 );
//     stream_ << "</h3>";

//     const auto& info = SystemInfo::buildInfo();

//     for ( const auto& key : SystemInfo::orderedBuildKeys() ) {
//         writeDiv( key, info[ key ] );
//     }
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::writeSessionInfo()
// {
//     stream_ << "<h3>";
//     stream_ << formatter_->colorized( "SESSION", 251 );
//     stream_ << "</h3>";

//     const auto& info = SystemInfo::sessionInfo();

//     for ( const auto& key : SystemInfo::orderedSessionKeys() ) {
//         writeDiv( key, info[ key ] );
//     }
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::writeTitle() {

//     stream_ << "<h1>";
//     stream_ << formatter_->colorized( SystemInfo::applicationName().toUpper(), 253 );
//     stream_ << "</h1> \n";
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::writeDiv( const QString& key, const QString& value )
// {
//     stream_ << "<div class='systeminfo'><b>";
//     stream_ << formatter_->colorized( key, 242 );
//     stream_ << "</b>";
//     stream_ << formatter_->spaces( 2 );
//     stream_ << formatter_->colorized( value, 240 );
//     stream_ << "</div> \n";
// }


// ////////////////////////////////////////////////////////////////////////////////
// void HtmlLogger::notify( const Letter& message )
// {
//     if ( ! fout_.isWritable() ) {
//         return;
//     }

//     formatter_->pushType( Formatter::Html );

//     const QString text = formatter_->format( message );
//     stream_ << "<div>" << text.toUtf8() << "</div> \n";
//     stream_.flush();

//     formatter_->popType();
// }
