#include "bugle/format/formatter.h"
#include "bugle/utility/utility.h"
#include "bugle/format/colortable.h"
#include "bugle/format/theme.h"

#include <sstream>
#include <format>
#include <algorithm>
#include <cctype>

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
std::string Formatter::format( const Letter& letter ) const
{
    std::stringstream ss;

    //  time

    ss << colorize(
        letter.timestamp.timeInfo<std::chrono::milliseconds>(),
        theme_->primary().variant
    );

    //  thread

    ss << skip( 1 );
    ss << colorize(
        std::format( "[{}]", letter.threadInfo() ),
        theme_->secondary().variant
    );

    //  letter without quotes

    ss << skip( 2 );
    ss << indent( letter );

    if ( ! letter.message.empty() ) {
        ss << colorize( letter.message, theme_->primary().color );
        ss << skip( 2 );
    }

    //  tags

    const std::string tinfo = tagInfo( letter.tags );

    if ( ! tinfo.empty() ) {
        ss << tinfo << skip( 2 );
    }

    //  attributes

    const std::string ainfo = attributeInfo( letter.attributes );

    if ( ! ainfo.empty() ) {
        ss << ainfo << skip( 2 );
    }

    //  meta

    const auto text = std::format( "{}:{}", letter.fileInfo(), letter.line() );
    const auto location = std::format( "[{} {}]", letter.functionInfo(), text );

    ss << colorize( location, theme_->secondary().variant );
    return ss.str();
}


////////////////////////////////////////////////////////////////////////////////
//  banner styling
static constexpr uint8_t kBannerGlyph = 244;
static constexpr uint8_t kBannerTitle = 254;
static constexpr uint8_t kBannerValue = 250;
static constexpr uint8_t kBannerKey = 246;


////////////////////////////////////////////////////////////////////////////////
static std::string bannerValueInfo( const nlohmann::json& value )
{
    if ( value.is_number_float() ) {
        return std::format( "{:.2f}", value.get<float>() );
    }

    if ( value.is_string() ) {
        return value.get<std::string>();
    }

    return value.dump();
}


////////////////////////////////////////////////////////////////////////////////
//  visible keys of an object: "_order"-listed first, remaining alphabetically;
//  keys starting with '_' are metadata and skipped
static std::vector<std::string> bannerKeys( const nlohmann::json& object )
{
    std::vector<std::string> keys;

    for ( const auto& item : object.items() )
    {
        if ( item.key().starts_with( '_' ) ) {
            continue;
        }

        keys.push_back( item.key() );   //  json objects iterate alphabetically
    }

    if ( ! object.contains( "_order" ) || ! object.at( "_order" ).is_array() ) {
        return keys;
    }

    std::vector<std::string> ordered;

    for ( const auto& entry : object.at( "_order" ) )
    {
        if ( ! entry.is_string() ) {
            continue;
        }

        const auto it = std::ranges::find( keys, entry.get<std::string>() );

        if ( it == keys.end() ) {
            continue;
        }

        ordered.push_back( *it );
        keys.erase( it );
    }

    ordered.insert( ordered.end(), keys.begin(), keys.end() );
    return ordered;
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::banner( const attributes_t& attributes ) const
{
    const nlohmann::json object( attributes );
    std::vector<std::string> lines;

    //  heading

    if ( object.contains( "_title" ) && object.at( "_title" ).is_string() )
    {
        std::string heading = object.at( "_title" ).get<std::string>();
        std::ranges::transform( heading, heading.begin(), []( unsigned char c ) {
            return (char)std::toupper( c );
        });

        if ( object.contains( "_icon" ) && object.at( "_icon" ).is_string() ) {
            heading = std::format( "{} {}", object.at( "_icon" ).get<std::string>(), heading );
        }

        lines.push_back( std::format( "  {} {}",
            colorize( "┌", kBannerGlyph ),
            colorize( heading, kBannerTitle )
        ));
    }

    //  partition top-level keys into scalar rows and sections

    std::vector<std::string> scalars;
    std::vector<std::string> sections;

    for ( const auto& key : bannerKeys( object ) )
    {
        if ( ! object.at( key ).is_object() ) {
            scalars.push_back( key );
            continue;
        }

        if ( ! bannerKeys( object.at( key ) ).empty() ) {
            sections.push_back( key );  //  sections without visible keys are skipped
        }
    }

    const auto kv = [ this, &lines ](
        const std::string& key,
        const nlohmann::json& value,
        const bool closeInner,
        const bool closeOuter )
    {
        lines.push_back( std::format( "  {} {} {}: {}",
            colorize( closeOuter ? "└" : "│", kBannerGlyph ),
            colorize( closeInner ? "└" : "├", kBannerGlyph ),
            colorize( std::format( "{:<20}", key ), kBannerKey ),
            colorize( bannerValueInfo( value ), kBannerValue )
        ));
    };

    //  top-level scalar rows

    for ( size_t i = 0; i < scalars.size(); i++ )
    {
        const bool last = ( i + 1 == scalars.size() );
        kv( scalars[ i ], object.at( scalars[ i ] ), last, last && sections.empty() );
    }

    //  sections

    for ( size_t si = 0; si < sections.size(); si++ )
    {
        const auto& section = object.at( sections[ si ] );

        std::string title = sections[ si ];
        title[ 0 ] = (char)std::toupper( (unsigned char)title[ 0 ] );

        if ( section.contains( "_icon" ) && section.at( "_icon" ).is_string() ) {
            title = std::format( "{} {}", section.at( "_icon" ).get<std::string>(), title );
        }

        lines.push_back( std::format( "  {} {} {}",
            colorize( "│", kBannerGlyph ),
            colorize( "┌", kBannerGlyph ),
            colorize( title, kBannerTitle )
        ));

        const auto keys = bannerKeys( section );

        for ( size_t i = 0; i < keys.size(); i++ )
        {
            const bool closeInner = ( i + 1 == keys.size() );
            const bool closeOuter = closeInner && ( si + 1 == sections.size() );
            kv( keys[ i ], section.at( keys[ i ] ), closeInner, closeOuter );
        }
    }

    std::stringstream ss;

    for ( size_t i = 0; i < lines.size(); i++ )
    {
        if ( i > 0 ) {
            ss << newline();
        }

        ss << lines[ i ];
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
std::string Formatter::indent( const Letter& letter ) const
{
    const int level = letter.level;

    if ( level < 0 || indent_ == 0 ) {
        return {};
    }

    const bool isEnv = letter.tags.contains( "envelope" );
    const bool isOpen = (
        letter.attributes.contains( "open" ) ?
        letter.attributes.at( "open" ).get<bool>() :
        false
    );

    std::string sym = "├";

    if ( isEnv ) {
        sym = isOpen ? "┌" : "└";
    } else if ( lastThread != letter.thread ) {
        sym = "┬";
    }

    lastThread = letter.thread;

    return colorize(
        bugle::repeat( spacer(), indent_ * std::max( level, 0 ) ) + sym + " ",
        theme_->secondary().variant
    );
}


////////////////////////////////////////////////////////////////////////////////
//  tags render as colorized "#tag"
std::string Formatter::pretty( const nlohmann::json& value ) const
{
    if ( ! value.is_string() ) {
        return {};
    }

    const auto cols = theme_->get( value );

    return std::format( "{}{}",
        colorize( "#", theme_->secondary().color ),
        colorize( value.get<std::string>(), cols.color )
    );
}


////////////////////////////////////////////////////////////////////////////////
std::string Formatter::tagInfo( const tags_t& tags ) const
{
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
    if ( attributes.empty() ) {
        return {};
    }

    std::stringstream stream;
    bool firstItem = true;

    for ( const auto& [ key, value ] : attributes )
    {
        //  metadata keys, e.g. banner "_title"/"_icon"/"_order"
        if ( key.starts_with( '_' ) ) {
            continue;
        }

        if ( firstItem ) {
            firstItem = false;
        } else {
            stream << skip( 1 );
        }

        const auto pair = theme_->get( key );
        const auto cols = ( ! value.empty() && value.is_primitive() ) ? ColorPair( { pair.variant, pair.color } ) : pair;

        stream << colorize( key, cols.color );

        if ( value.empty() ) {
            continue;
        }

        stream << colorize( ":", theme_->secondary().variant );

        if ( value.is_primitive() ) {
            stream << colorize( bannerValueInfo( value ), cols.variant );
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
std::string AsciiFormatter::beginItalic() const {
    return "\x1b[3m";
}


////////////////////////////////////////////////////////////////////////////////
std::string AsciiFormatter::endItalic() const {
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
