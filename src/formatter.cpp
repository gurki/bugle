#include "messagecenter/formatter.h"
#include "messagecenter/utility.h"
#include "messagecenter/colortable.h"
#include "messagecenter/theme.h"

#include <sstream>


namespace mc {


static const uint8_t col_text = 254;
static const uint8_t col_light_text = 246;
static const uint8_t col_secondary = 242;
static const uint8_t col_light_secondary = 238;


////////////////////////////////////////////////////////////////////////////////
Formatter::Formatter() {
    theme_ = std::make_shared<Theme>();
}


////////////////////////////////////////////////////////////////////////////////
void Formatter::setIndent( const uint8_t indent ) {
    indent_ = indent;
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::format( const Message& message ) const
{
    //  time

    std::stringstream ss;
    ss << colorize( 
        message.timestamp().timeInfo( DateTime::Microseconds ), 
        theme_->secondary().first 
    );

    //  message without quotes

    if ( ! message.content().empty() ) {
        ss << skip( 2 );
        const std::string msg = message.content().dump();
        ss << colorize( msg.substr( 1, msg.size() - 2 ), theme_->primary().first );
    }

    ss << skip( 2 );

    //  tags
    const std::string tinfo = tagInfo( message.tags() );

    if ( ! tinfo.empty() ) {
        ss << tinfo << skip( 2 );
    }

    //  meta

    if ( message.isIndexed() ) {
        ss << colorize( message.info(), theme_->secondary().second );
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

        const auto& colors = theme_->indices( key );
        stream << colorize( "#", theme_->secondary().second );
        stream << colorize( key, colors.first );

        if ( value.empty() ) {
            continue;
        }

        stream << colorize( ":", theme_->secondary().second );
        stream << colorize( value.dump(), colors.second );
    }

    return stream.str();
}


////////////////////////////////////////////////////////////////////////////////
std::string AsciiFormatter::beginColor( const uint8_t index ) const {
    return ColorTable::ansiEscapeCode( index );
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
