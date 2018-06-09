#include "colortable.h"

#include <QDebug>
#include <QFile>
#include <QResource>

#include <iostream>


namespace mc {

////////////////////////////////////////////////////////////////////////////////
json ColorTable::table_ = json();
ColorTable ColorTable::instance_ = ColorTable();


////////////////////////////////////////////////////////////////////////////////
ColorTable::ColorTable()
{
    Q_INIT_RESOURCE( data );

    QFile fin( ":data/colors.json" );

    if ( ! fin.open( QFile::ReadOnly | QFile::Text ) ) {
        qWarning( "couldn't find color sheet" );
        return;
    }

    const QByteArray data = fin.readAll();

    try {
        table_ = json::parse( data );
    } catch ( std::exception e ) {
        std::cout << e.what() << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////
QString ColorTable::hex( const uint8_t index )
{
    if ( table_.is_null() ) {
        return "#000";
    }

    const auto hexString = table_[ index ][ "hexString" ];
    return QString::fromStdString( hexString );
}


////////////////////////////////////////////////////////////////////////////////
uint8_t ColorTable::ansiIndex( const QColor& color )
{
    auto conv = []( const int v ) -> int {
        return std::floorf( v / 266.f * 5.f );
    };

    const int r = conv( color.red() );
    const int g = conv( color.green() );
    const int b = conv( color.blue() );
    const uint8_t index = 16 + 36 * r + 6 * g + b;

    return index;
}


////////////////////////////////////////////////////////////////////////////////
QString ColorTable::ansiEscapeCode( const QColor& color ) {
    return ansiEscapeCode( ansiIndex( color ) );
}


////////////////////////////////////////////////////////////////////////////////
QString ColorTable::ansiEscapeCode( const uint8_t index ) {
    return QString::asprintf( "\033[38;5;%im", index );
}


////////////////////////////////////////////////////////////////////////////////
QString defaultColor() {
    return ColorTable::ansiEscapeCode( 251 );
}


////////////////////////////////////////////////////////////////////////////////
QString lightColor() {
    return ColorTable::ansiEscapeCode( 246 );
}


////////////////////////////////////////////////////////////////////////////////
QPair<uint8_t, uint8_t> defaultTagColors() {
    return { 243, 248 };    //  grayscale
}


////////////////////////////////////////////////////////////////////////////////
QPair<uint8_t, uint8_t> defaultTextColors() {
    return { 251, 246 };
}

}   //  mc
