#include "messageformatter.h"
#include "colortable.h"
#include "messagecenter.h"

#include <QDateTime>
#include <QDebug>


////////////////////////////////////////////////////////////////////////////////
MessageFormatter::MessageFormatter( QObject* parent ) :
    QObject( parent ),
    colors_({
        { "info", { 39, 45 } },
        { "success", { 40, 46 } },
        { "warning", { 220, 226 } },
        { "error", { 196, 202 } },
        { "debug", { 159, 195 } }
    })
{
    types_.push( Html );
}


////////////////////////////////////////////////////////////////////////////////
void MessageFormatter::setDateFormat( const Qt::DateFormat format ) {
    format_ = format;
}


////////////////////////////////////////////////////////////////////////////////
void MessageFormatter::setIndent( const uint8_t spaces ) {
    indent_ = spaces;
}


////////////////////////////////////////////////////////////////////////////////
void MessageFormatter::pushType( const FormatType type ) {
    types_.push( type );
}


////////////////////////////////////////////////////////////////////////////////
void MessageFormatter::popType() {
    if ( types_.size() > 1 ) {
        types_.pop();
    }
}


////////////////////////////////////////////////////////////////////////////////
MessageFormatter::FormatType MessageFormatter::type() const {
    return types_.top();
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::format( const Message& message ) const
{
    QString msg;
    QTextStream stream( &msg );

    stream << colorized( message.timeInfo( Qt::ISODate ), 246 );

#ifndef MC_DISABLE_SCOPE
    const bool isScope = message.tags().contains( "__scope" );
#else
    const bool isScope = false;
#endif

    if ( ! message.text().isEmpty() || isScope )
    {
        stream << spaces( 2 );

        if ( indent_ ) {
            stream << levelIndent( message );
        }

        if ( isScope ) {
            stream << colorized( message.function(), 243 );
        } else {
            stream << colorized( message.text(), 251 );
        }
    }

    stream << spaces( 2 );

    if ( message.hasTags() )
    {
        const QString tags = tagInfo( message.tags() );

        if ( ! tags.isEmpty() ) {
            stream << tags;
            stream << spaces( 2 );
        }
    }

    if ( message.tags().contains( "debug" ) ) {
        stream << colorized( message.info(), 240 );
    }

    return msg;
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::spacer() const {
    return ( type() == Html ? "&middot;" : QString::fromUtf8( "\xc2\xb7" ) );
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::space() const {
    return ( type() == Html ? "&nbsp;" : " " );
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::spaces( const uint8_t count ) const  {
    return space().repeated( count );
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::levelIndent( const Message& message ) const
{
    if ( indent_ == 0 ) {
        return QString();
    }

    const QString indent = spacer() + " ";

    QString info;
    QTextStream stream( &info );

    const QString colind = colorized( indent , 240 );
    stream << colind.repeated( message.level() * indent_ );

#ifndef MC_DISABLE_SCOPE
    if ( message.tags().contains( "__scope" ) )
    {
        const QString& value = message.tags()[ "__scope" ].toString();

        if ( value == "leave" ) {
            stream << colorized( "/", 243 );
        }
    }
#endif

    return info;
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::tagInfo( const QVariantMap& tags ) const
{
    static const uint8_t textColor = 246;

    QString info;
    QTextStream stream( &info );

    auto iter = tags.constBegin();
    bool firstItem = true;

    while ( iter != tags.constEnd() )
    {
        const auto& tag = iter.key();

        if ( ! systemTags_ && tag.startsWith( "__" ) ) {
            iter++;
            continue;
        }

        if ( firstItem ) {
            stream << colorized( "[", textColor );
            firstItem = false;
        } else {
            stream << colorized( ",", textColor );
        }

        auto codes = mc::defaultTagColors();

        if ( colors_.contains( tag ) ) {
            codes = colors_[ tag ];
        }

        stream << colorized( iter.key(), codes.first );

        if ( iter.value().isValid() ) {
            stream << colorized( ":", textColor );
            stream << colorized( iter.value().toString(), codes.second );
        }

        iter++;
    }

    if ( ! firstItem ) {
        stream << colorized( "]", textColor );
    }

    return info;
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::colorized(
    const QString& text,
    const uint8_t index ) const
{
    if ( type() == Plaintext ) {
        return text;
    }

    return beginColor( index ) + text + endColor();
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::beginColor( const uint8_t index ) const
{
    switch ( type() )
    {
        case AnsiEncoding:
            return "\033[38;5;" + QString::number( index ) + "m";

        case Html:
            const QString hex = mc::ColorTable::hex( index );
            return "<font color='" + hex + "'>";
    }

    return QString();
}


////////////////////////////////////////////////////////////////////////////////
QString MessageFormatter::endColor() const
{
    switch ( type() ) {
        case AnsiEncoding: return "\033[0m";
        case Html: return "</font>";
    }

    return QString();
}


////////////////////////////////////////////////////////////////////////////////
void MessageFormatter::setTagColor( const QString& tag, const uint8_t key ) {
    colors_[ tag ] = { key, key };
}


////////////////////////////////////////////////////////////////////////////////
void MessageFormatter::setTagColor(
    const QString& tag,
    const uint8_t key,
    const uint8_t value )
{
    colors_[ tag ] = { key, value };
}


////////////////////////////////////////////////////////////////////////////////
bool MessageFormatter::hasTagColor( const QString& tag ) {
    return colors_.contains( tag );
}


////////////////////////////////////////////////////////////////////////////////
QPair<QString, QString> MessageFormatter::tagColorCodes( const QString& tag )
{
    auto codes = mc::defaultTagColors();

    if ( colors_.contains( tag ) ) {
        codes = colors_[ tag ];
    }

    return {
        mc::ColorTable::ansiEscapeCode( codes.first ),
        mc::ColorTable::ansiEscapeCode( codes.second )
    };
}

