#include "messagecenter/messageformatter.h"
#include "messagecenter/utility.h"
#include "messagecenter/colortable.h"

#include <sstream>
#include <string_view>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
MessageFormatter::MessageFormatter()
{}


////////////////////////////////////////////////////////////////////////////////
void MessageFormatter::setIndent( const uint8_t indent ) {
    indent_ = indent;
}


////////////////////////////////////////////////////////////////////////////////
std::string MessageFormatter::format( const Message& message ) const
{
    std::stringstream ss;
    ss << colorize( message.datetime_.timeInfo( DateTime::Microseconds ), "Grey58" );

    if ( ! message.payload_.empty() )
    {
        ss << skip( 2 );

        const std::string msg = message.payload_.dump();
        std::string_view view( &msg[0], msg.size() );
//        view.remove_prefix( 1 );
//        view.remove_suffix( 1 );

        std::stringstream ssv;
        ssv << view;

        ss << colorize( msg, "Gray78" );
    }

    ss << skip( 2 );

    for ( const auto& tag : message.tags_ )
    {
        if ( ! tag.empty() ) {
            ss << tag;
            ss << skip( 2 );
        }
    }

    return ss.str();
}


////////////////////////////////////////////////////////////////////////////////
std::string MessageFormatter::colorize(
    const std::string& text,
    const uint8_t index ) const
{
    return beginColor( index ) + text + endColor();
}


////////////////////////////////////////////////////////////////////////////////
std::string MessageFormatter::colorize(
        const std::string& text,
        const std::string& colorName ) const
{
    const uint8_t index = ColorTable::findName( colorName );
    return beginColor( index ) + text + endColor();
}


////////////////////////////////////////////////////////////////////////////////
std::string MessageFormatter::beginColor( const uint8_t index ) const {
    return "\x1b[38;5;" + std::to_string( index ) + "m";
//    const QString hex = mc::ColorTable::hex( index );
//    return "<font color='" + hex + "'>";
}


////////////////////////////////////////////////////////////////////////////////
std::string MessageFormatter::endColor() const {
    return "\x1b[0m";
//    case Html: return "</font>";
}


////////////////////////////////////////////////////////////////////////////////
std::string MessageFormatter::skip( const uint8_t count ) const  {
    return repeat( space(), count );
}


//////////////////////////////////////////////////////////////////////////////////
//QString MessageFormatter::levelIndent( const Message& message ) const
//{
//    if ( indent_ == 0 ) {
//        return QString();
//    }

//    const QString indent = spacer() + " ";

//    QString info;
//    QTextStream stream( &info );

//    const QString colind = colorized( indent , 240 );
//    stream << colind.repeated( message.level() * indent_ );

//#ifndef MC_DISABLE_SCOPE
//    if ( message.tags().contains( "__scope" ) )
//    {
//        const QString& value = message.tags()[ "__scope" ].toString();

//        if ( value == "leave" ) {
//            stream << colorized( "/", 243 );
//        }
//    }
//#endif

//    return info;
//}


//////////////////////////////////////////////////////////////////////////////////
//QString MessageFormatter::tagInfo( const QVariantMap& tags ) const
//{
//    static const uint8_t textColor = 246;

//    QString info;
//    QTextStream stream( &info );

//    auto iter = tags.constBegin();
//    bool firstItem = true;

//    while ( iter != tags.constEnd() )
//    {
//        const auto& tag = iter.key();

//        if ( ! systemTags_ && tag.startsWith( "__" ) ) {
//            iter++;
//            continue;
//        }

//        if ( firstItem ) {
//            stream << colorized( "[", textColor );
//            firstItem = false;
//        } else {
//            stream << colorized( ",", textColor );
//        }

//        auto codes = mc::defaultTagColors();

//        if ( colors_.contains( tag ) ) {
//            codes = colors_[ tag ];
//        }

//        stream << colorized( iter.key(), codes.first );

//        if ( iter.value().isValid() ) {
//            stream << colorized( ":", textColor );
//            stream << colorized( iter.value().toString(), codes.second );
//        }

//        iter++;
//    }

//    if ( ! firstItem ) {
//        stream << colorized( "]", textColor );
//    }

//    return info;
//}



//////////////////////////////////////////////////////////////////////////////////
//QPair<QString, QString> MessageFormatter::tagColorCodes( const QString& tag )
//{
//    auto codes = mc::defaultTagColors();

//    if ( colors_.contains( tag ) ) {
//        codes = colors_[ tag ];
//    }

//    return {
//        mc::ColorTable::ansiEscapeCode( codes.first ),
//        mc::ColorTable::ansiEscapeCode( codes.second )
//    };
//}


}   //  mc::
