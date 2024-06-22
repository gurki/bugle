#include "bugle/format/formatter.h"
#include "bugle/utility/utility.h"
#include "bugle/format/colortable.h"
#include "bugle/format/theme.h"

#include <sstream>
#include <format>
#include <regex>
#include <print>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Formatter::Formatter(
    const std::string& space,
    const std::string& spacer,
    const std::string& newline ) :
    space_( space ),
    spacer_( spacer ),
    newline_( newline )
{
    theme_ = std::make_shared<DefaultTheme>();
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
std::string Formatter::format( const Letter& message ) const
{
    std::stringstream ss;

    //  time

    ss << colorize(
        message.timestamp.timeInfo<std::chrono::milliseconds>(),
        theme_->primary().variant
    );

    //  thread

    ss << skip( 1 );
    ss << colorize(
        std::format( "[{}]", message.threadInfo() ),
        theme_->primary().variant
    );

    //  message without quotes

    if ( ! message.message.empty() ) {
        ss << skip( 2 );
        ss << indent( message.level );
        ss << colorize( message.message, theme_->primary().color );
    }

    ss << skip( 2 );

    //  tags

    const std::string tinfo = tagInfo( message.tags );

    if ( ! tinfo.empty() ) {
        ss << tinfo << skip( 2 );
    }

    // //  attributes

    // const std::string ainfo = attributeInfo( message.attributes );

    // if ( ! ainfo.empty() ) {
    //     ss << ainfo << skip( 2 );
    // }

    //  meta

    static const std::regex re( R"((.* )?(.*)(\(.*\))(::<lambda(.*)>)?)" );
    const std::string fn = message.function();
    std::smatch match;
    std::regex_match( fn, match, re );

    std::string name;
    if ( ! match.empty() ) 
    {
        name = match[ 2 ].str();

        if ( match[ 4 ].matched ) {
            name += "::lambda";
        }
    }

    const auto text = std::format( "{}:{}", message.fileInfo(), message.line() );
    // const auto link = std::format( "{}:{}", message.file(), message.line() );
    // const auto hyperlink = std::format( "\e]8;;{}\e\\{}\e]8;;\e\\", link, text );
    const auto location = std::format( "[{} {}]", message.functionInfo(), text );

    ss << colorize( location, theme_->secondary().variant );
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
    return bugle::repeat( space(), count );
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::indent( const int level ) const
{
    if ( level <= 0 || indent_ == 0 ) {
        return {};
    }

    return colorize(
        bugle::repeat( spacer(), indent_ * level ),
        theme_->secondary().variant
    );
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::pretty( const nlohmann::json& value ) const
{
    if ( value.is_string() )
    {
        const auto cols = theme_->get( value );

        return std::format( "{}{}",
            colorize( "#", theme_->secondary().variant ),
            colorize( value.get<std::string>(), cols.color )
        );
    }

    if ( value.is_primitive() ) {
        return {};
    }

    if ( value.size() > 1 ) {
        return colorize( "#…", theme_->secondary().variant );
    }

    if ( value.is_object() )
    {
        std::stringstream stream;

        const auto iter = value.items().begin();
        const auto& key = iter.key();
        const auto& value = iter.value();

        const auto pair = theme_->get( key );
        const auto cols = ( ! value.empty() && value.is_primitive() ) ? ColorPair( { pair.variant, pair.color } ) : pair;

        stream << colorize( "#", theme_->secondary().variant );
        stream << colorize( key, cols.color );

        if ( value.empty() ) {
            return stream.str();
        }

        stream << colorize( ":", theme_->secondary().variant );

        if ( value.is_primitive() ) {
            stream << colorize( value.dump(), cols.variant );
        } else {
            stream << colorize( "…", cols.variant );
        }

        return stream.str();
    }

    return {};
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::tagInfo( const tags_t& tags ) const
{
    static const size_t maxSize = 24;

    if ( tags.empty() ) {
        return {};
    }

    std::stringstream stream;
    bool firstItem = true;

    for ( const auto& tag : tags )
    {
        if ( firstItem ) {
            firstItem = false;
        } else {
            stream << skip( 1 );
        }

        stream << pretty( tag );
    }

    return stream.str();
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::attributeInfo( const attributes_t& attributes ) const
{
    static const size_t maxSize = 24;

    if ( attributes.empty() ) {
        return {};
    }

    std::stringstream stream;
    bool firstItem = true;

    for ( const auto& [ key, value ] : attributes )
    {
        if ( firstItem ) {
            firstItem = false;
        } else {
            stream << skip( 1 );
        }

        const auto pair = theme_->get( key );
        const auto cols = ( ! value.empty() && value.is_primitive() ) ? ColorPair( { pair.variant, pair.color } ) : pair;

        // stream << colorize( "#", theme_->secondary().variant );
        stream << colorize( key, cols.color );

        if ( value.empty() ) {
            continue;
        }

        stream << colorize( ":", theme_->secondary().variant );

        if ( value.is_primitive() ) {
            stream << colorize( value.dump(), cols.variant );
        } else {
            stream << colorize( "…", cols.variant );
        }
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


}   //  ::bugle
