#include "messagecenter/formatter.h"
#include "messagecenter/utility.h"
#include "messagecenter/colortable.h"

#include <sstream>


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

    ss << skip( 2 );

    const std::string tinfo = tagInfo( message.tags() );

    if ( ! tinfo.empty() ) {
        ss << tinfo << skip( 2 );
    }

    if ( message.isIndexed() ) {
        ss << colorize( message.info(), 238 );
    }

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
std::string Formatter::skip( const uint8_t count ) const  {
    return repeat( space(), count );
}


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

        if ( firstItem ) {
            firstItem = false;
        } else {
            stream << skip( 1 );
        }

        stream << colorize( "#", text1 );
        stream << colorize( key, tag1 );

        if ( value.empty() ) {
            continue;
        }

        stream << colorize( ":", text1 );
        stream << colorize( value.dump(), tag2 );
    }

    return stream.str();
}


////////////////////////////////////////////////////////////////////////////////
std::string AsciiFormatter::beginColor( const uint8_t index ) const {
    return "\x1b[38;5;" + std::to_string( index ) + "m";
}


////////////////////////////////////////////////////////////////////////////////
std::string AsciiFormatter::endColor() const {
    return "\x1b[0m";
}


////////////////////////////////////////////////////////////////////////////////
std::string HtmlFormatter::beginColor( const uint8_t index ) const {
    const std::string hex = ColorTable::hexString( index );
    return "<font color='" + hex + "'>";
}


////////////////////////////////////////////////////////////////////////////////
std::string HtmlFormatter::endColor() const {
    return "</font>";
}


}   //  mc::
