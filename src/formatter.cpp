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
    ss << colorize( message.datetime_.timeInfo( DateTime::Microseconds ), 246 );

    if ( ! message.payload_.empty() ) {
        ss << skip( 2 );
        const std::string msg = message.payload_.dump();
        ss << colorize( msg.substr( 1, msg.size() - 2 ), 251 );
    }

    ss << skip( 2 );

    // for ( const auto& tag : message.tags_ )
    // {
        // ss << tag.first;

        // if ( ! tag.second.empty() ) {
        //     ss << ":" << tag.second;
        // }

        // ss << skip( 1 );
    // }

    ss << tagInfo( message.tags_ );

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
std::string Formatter::tagInfo( const jmap_t& tags ) const
{
    static const uint8_t text1 = 246;
    static const uint8_t tag1 = 248;
    static const uint8_t tag2 = 243;
    static const size_t maxSize = 24;

    std::stringstream stream;

    bool firstItem = true;

    for ( const auto& tag : tags )
    {
        const auto& key = tag.first;
        const auto& value = tag.second;

        // if ( ! systemTags_ && key.startsWith( "__" ) ) {
        //     iter++;
        //     continue;
        // }

        if ( firstItem ) {
            // stream << colorize( "[", text1 );
            firstItem = false;
        } else {
            stream << colorize( " ", text1 );
        }

        // auto codes = mc::defaultTagColors();

        // if ( colors_.contains( key ) ) {
        //     codes = colors_[ key ];
        // }

        stream << colorize( "#", text1 );
        stream << colorize( key, tag1 );

        if ( ! value.empty() ) 
        {
            stream << colorize( ":", text1 );

            const std::string valueStr = value.dump();
            stream << colorize( valueStr.substr( 0, maxSize ), tag2 );
            
            if ( valueStr.size() > maxSize ) {
                stream << colorize( "\xE2\x80\xA6", tag2 );
            }
        }
    }

    // if ( ! firstItem ) {
    //     stream << colorize( "]", text1 );
    // }

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
