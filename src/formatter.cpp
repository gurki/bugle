#include "messagecenter/formatter.h"
#include "messagecenter/utility.h"
#include "messagecenter/colortable.h"
#include "messagecenter/theme.h"

#include <sstream>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
Formatter::Formatter( const std::string& space, const std::string& newline ) :
    space_( space ),
    newline_( newline )
{
    theme_ = std::make_shared<Theme>();
}


////////////////////////////////////////////////////////////////////////////////
void Formatter::setIndent( const uint8_t indent ) {
    indent_ = indent;
}


////////////////////////////////////////////////////////////////////////////////
void Formatter::setTheme( const ThemePtr& theme ) {
    theme_ = theme;
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::format( const Message& message ) const
{
    //  time

    std::stringstream ss;
    ss << colorize( 
        message.timestamp().timeInfo( DateTime::Microseconds ), 
        theme_->secondary().color 
    );

    //  message without quotes

    if ( ! message.content().empty() ) {
        ss << skip( 2 );
        const std::string msg = message.content().dump();
        ss << colorize( msg.substr( 1, msg.size() - 2 ), theme_->primary().color );
    }

    ss << skip( 2 );

    //  tags
    const std::string tinfo = tagInfo( message.tags() );

    if ( ! tinfo.empty() ) {
        ss << tinfo << skip( 2 );
    }

    //  meta

    if ( message.isIndexed() ) {
        ss << colorize( message.info(), theme_->secondary().variant );
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

        const auto& pair = theme_->get( key );
        stream << colorize( "#", theme_->secondary().variant );
        stream << colorize( key, pair.color );

        if ( value.empty() ) {
            continue;
        }

        stream << colorize( ":", theme_->secondary().variant );
        stream << colorize( value.dump(), pair.variant );
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
    const std::string hex = ColorTable::hex( index );
    return "<font color='" + hex + "'>";
}


////////////////////////////////////////////////////////////////////////////////
std::string HtmlFormatter::endColor() const {
    return "</font>";
}


}   //  mc::
