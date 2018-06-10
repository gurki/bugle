#include "messagecenter/colortable.h"
#include <iostream>
#include <fstream>
#include <strstream>


namespace mc {


nlohmann::json ColorTable::table_ = {};
ColorTable ColorTable::instance_ = {};


////////////////////////////////////////////////////////////////////////////////
ColorTable::ColorTable()
{
    //  FIXME: relative path
    std::ifstream fin( "/Users/tobiasgurdan/Documents/projects/2018/messagecenter/data/colors.json" );

    if ( ! fin.is_open() ) {
        return;
    }

    table_ = nlohmann::json::parse( fin );
}


////////////////////////////////////////////////////////////////////////////////
std::string ColorTable::hex( const uint8_t index )
{
    if ( table_.is_null() ) {
        return "#000000";
    }

    return table_[ index ][ "hexString" ];
}


////////////////////////////////////////////////////////////////////////////////
uint8_t ColorTable::ansiIndex( const std::string& hex )
{
    if ( hex.empty() ) {
        return 0;
    }

    const std::string hexstr = ( hex.at( 0 ) == '#' ) ? hex.substr( 1 ) : hex;

    if ( hexstr.size() > 8 ) {
        return 0;
    }

    auto conv = []( const int v ) -> int {
        return std::roundf( 5.f * v / 255.f );
    };

    std::stringstream ss;
    ss << std::hex << hexstr;

    uint32_t val;
    ss >> val;

    const int r = conv( ( val >> 16 ) & 0xff );
    const int g = conv( ( val >> 8 ) & 0xff );
    const int b = conv( val & 0xff  );
    const uint8_t index = 16 + 36 * r + 6 * g + b;

    return index;
}


////////////////////////////////////////////////////////////////////////////////
std::string ColorTable::ansiEscapeCode( const std::string& hex ) {
    return ansiEscapeCode( ansiIndex( hex ) );
}


////////////////////////////////////////////////////////////////////////////////
std::string ColorTable::ansiEscapeCode( const uint8_t index ) {
    return std::string( "\033[38;5;" ) + std::to_string( index ) + "m";
}


//////////////////////////////////////////////////////////////////////////////////
//std::string defaultColor() {
//    return ColorTable::ansiEscapeCode( 251 );
//}


//////////////////////////////////////////////////////////////////////////////////
//std::string lightColor() {
//    return ColorTable::ansiEscapeCode( 246 );
//}


//////////////////////////////////////////////////////////////////////////////////
//std::p<uint8_t, uint8_t> defaultTagColors() {
//    return { 243, 248 };    //  grayscale
//}


//////////////////////////////////////////////////////////////////////////////////
//QPair<uint8_t, uint8_t> defaultTextColors() {
//    return { 251, 246 };
//}


//////////////////////////////////////////////////////////////////////////////////
void ColorTable::printTestTable( const uint8_t numSteps )
{
    if ( numSteps == 0 ) {
        return;
    }

    std::cout << std::endl;

    for ( size_t i = 0; i < 8; i++ ) {
        std::cout << mc::ColorTable::ansiEscapeCode( i ) << u8"●";
    }

    std::cout << " ";

//    for ( size_t i = 8; i < 16; i++ ) {
//        std::cout << mc::ColorTable::ansiEscapeCode( i ) << u8"●";
//    }

//    std::cout << std::endl;

    std::cout << mc::ColorTable::ansiEscapeCode( "#000000" ) << u8"●";
    std::cout << mc::ColorTable::ansiEscapeCode( "#ff0000" ) << u8"●";
    std::cout << mc::ColorTable::ansiEscapeCode( "#00ff00" ) << u8"●";
    std::cout << mc::ColorTable::ansiEscapeCode( "#ffff00" ) << u8"●";
    std::cout << mc::ColorTable::ansiEscapeCode( "#0000ff" ) << u8"●";
    std::cout << mc::ColorTable::ansiEscapeCode( "#ff00ff" ) << u8"●";
    std::cout << mc::ColorTable::ansiEscapeCode( "#00ffff" ) << u8"●";
    std::cout << mc::ColorTable::ansiEscapeCode( "#ffffff" ) << u8"●";
    std::cout << std::endl;
    std::cout << std::endl;

    const float step = ( numSteps == 1 ) ? 255.f : ( 255.f / ( numSteps - 1 ) );

    for ( float r = 0.f; r <= 255.f; r += step )
    {
        const uint8_t r8 = (uint8_t)std::round( r );

        for ( float g = 0.f; g <= 255.f; g += step )
        {
            const uint8_t g8 = (uint8_t)std::round( g );

            for ( float b = 0.f; b <= 255.f; b += step )
            {
                const uint8_t b8 = (uint8_t)std::round( b );
                const uint32_t val = ( r8 << 16 ) + ( g8 << 8 ) + b8;

                std::stringstream ss;
                ss << "#" << std::hex << val;
                std::cout << mc::ColorTable::ansiEscapeCode( ss.str() ) << u8"●";
            }

            std::cout << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;

    for ( size_t i = 232; i < 256; i++ ) {
        std::cout << mc::ColorTable::ansiEscapeCode( i ) << u8"●";
    }

    std::cout << std::endl << std::endl;
}


}   //  mc