#include "bugle/format/formatter.h"
#include "bugle/utility/utility.h"
#include "bugle/format/colortable.h"
#include "bugle/format/theme.h"

#include <sstream>
#include <format>


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
std::string Formatter::format( const Message& message ) const
{
    //  time

    std::stringstream ss;
    ss << colorize(
        message.timestamp().timeInfo( DateTime::Milliseconds ),
        theme_->primary().variant
    );

    //  message without quotes

    if ( ! message.content().empty() ) {
        ss << skip( 2 );
        ss << indent( message.level() );
        ss << colorize( message.content(), theme_->primary().color );
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
std::string Formatter::pretty( const nlohmann::json& tag ) const
{
    if ( tag.is_string() ) 
    {
        const auto cols = theme_->get( tag );

        return std::format( "{}{}",
            colorize( "#", theme_->secondary().variant ),
            colorize( tag.get<std::string>(), cols.color )
        );
    }

    if ( tag.is_primitive() ) {
        return {};
    }

    if ( tag.size() > 1 ) {
        return colorize( "#…", theme_->secondary().variant );
    }

    if ( tag.is_object() ) 
    {
        std::stringstream stream;

        const auto iter = tag.items().begin();
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

    if ( tags.is_primitive() ) {
        return pretty( tags );
    }
    
    std::stringstream stream;
    bool firstItem = true;
    
    if ( tags.is_array() ) 
    {        
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

    if ( tags.is_object() ) 
    {
        for ( const auto& item : tags.items() ) 
        {
            if ( firstItem ) {
                firstItem = false;
            } else {
                stream << skip( 1 );
            }

            const auto& key = item.key();
            const auto& value = item.value();

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
        }

        return stream.str();
    }
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
