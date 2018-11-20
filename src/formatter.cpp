#include "messagecenter/formatter.h"
#include "messagecenter/utility.h"
#include "messagecenter/colortable.h"

#include <sstream>
#include <string_view>
#include <unordered_map>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
Formatter::Formatter()
{}


////////////////////////////////////////////////////////////////////////////////
void Formatter::setIndent( const uint8_t indent ) {
    indent_ = indent;
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::format( const Message& message ) const
{
    std::stringstream ss;
    ss << colorize( message.timestamp().timeInfo( DateTime::Microseconds ), 242 );

    if ( ! message.content().empty() ) {
        ss << skip( 2 );
        const std::string msg = message.content().dump();
        ss << colorize( msg.substr( 1, msg.size() - 2 ), 254 );
    }

    ss << skip( 1 );

    const std::string tinfo = tagInfo( message.tags() );

    if ( ! tinfo.empty() ) {
        ss << tinfo << skip( 2 );
    }

    // if ( message.isIndexed() ) {
    //     ss << colorize( message.info(), 238 );
    // }

    return ss.str();
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::colorize(
    const std::string& text,
    const uint8_t index ) const
{
    return beginColor( index ) + text + endColor();
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::beginColor( const uint8_t index ) const {
    return "\x1b[38;5;" + std::to_string( index ) + "m";
    // const QString hex = mc::ColorTable::hex( index );
    // return "<font color='" + hex + "'>";
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::endColor() const {
    return "\x1b[0m";
    // case Html: return "</font>";
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::skip( const uint8_t count ) const  {
    return repeat( space(), count );
}


//////////////////////////////////////////////////////////////////////////////////
//QString Formatter::levelIndent( const Message& message ) const
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


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::tagInfo( const tags_t& tags ) const
{
    static const uint8_t text1 = 238;
    static const uint8_t tag1 = 246;
    static const uint8_t tag2 = 242;
    static const size_t maxSize = 24;

    std::stringstream stream;

    bool firstItem = true;

    for ( const auto& tag : tags )
    {
        const auto& key = tag.first;
        const auto& value = tag.second;

        if ( ! value.empty() ) {
            firstItem = false;
            continue;
        }

        stream << skip( 1 );
        stream << colorize( "#", text1 );
        stream << colorize( key, tag1 );
    }

    if ( firstItem ) {
        return stream.str();
    }

    for ( const auto& tag : tags )
    {
        const auto& key = tag.first;
        const auto& value = tag.second;

        if ( value.empty() ) {
            continue;
        }

        stream << "\n";
        stream << skip( 2 );
        stream << colorize( "#", text1 );
        stream << colorize( key, tag1 );
        stream << colorize( ":", text1 );
        stream << colorize( value.dump(), tag2 );
    }

    return stream.str();
}



//////////////////////////////////////////////////////////////////////////////////
//QPair<QString, QString> Formatter::tagColorCodes( const QString& tag )
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
