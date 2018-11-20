#include "messagecenter/colortable.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>  //  std::hex
#include <string_view>


namespace mc {


using namespace std::string_literals;


nlohmann::json ColorTable::table_ = {};
ColorTable ColorTable::instance_ = {};


////////////////////////////////////////////////////////////////////////////////
ColorTable::ColorTable() {
    std::ifstream fin( MC_RESOURCE_DIR + "colors.json"s );
    assert( fin.is_open() );
    table_ = nlohmann::json::parse( fin );
}


////////////////////////////////////////////////////////////////////////////////
std::string ColorTable::hexString( const uint8_t index )
{
    if ( table_.is_null() ) {
        return "#000000";
    }

    return table_[ index ][ "hexString" ];
}


////////////////////////////////////////////////////////////////////////////////
uint8_t ColorTable::findName( const std::string& name )
{
    if ( table_.is_null() ) {
        return 15;  //  white
    }

    auto it = std::find_if( table_.begin(), table_.end(), [ &name ]( const json& item ) {
        return item[ "name" ] == name;
    });

    if ( it == table_.end() ) {
        return 15;  //  white
    }
    
    return it->at( "colorId" );
}


////////////////////////////////////////////////////////////////////////////////
std::string ColorTable::colorName( const uint8_t id ) {
    return table_[ id ][ "name" ];
}


////////////////////////////////////////////////////////////////////////////////
uint8_t ColorTable::colorId( const std::string& hex )
{
    if ( hex.empty() ) {
        return 15;  //  white
    }

    std::string_view hexstr( &hex[ 0 ], hex.size() );

    if ( hexstr[ 0 ] == '#' ) {
        hexstr.remove_prefix( 1 );
    }

    if ( hexstr.size() > 8 ) {
        return 15;  //  white
    }

    std::stringstream ss;
    ss << std::hex << hexstr;

    uint32_t val;
    ss >> val;

    auto conv = []( const int v ) -> int {
        return int( std::roundf( 5.f * v / 255.f ) );
    };

    const int r = conv( ( val >> 16 ) & 0xff );
    const int g = conv( ( val >> 8 ) & 0xff );
    const int b = conv( val & 0xff  );
    const uint8_t index = 16 + 36 * r + 6 * g + b;

    return index;
}


////////////////////////////////////////////////////////////////////////////////
std::string ColorTable::ansiEscapeCode( const std::string& hex ) {
    return ansiEscapeCode( colorId( hex ) );
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
    static const std::string sym = "\xe2\x97\x8f";

    if ( numSteps == 0 ) {
        return;
    }

    std::cout << std::endl;

    for ( uint8_t i = 0; i < 8; i++ ) {
        std::cout << mc::ColorTable::ansiEscapeCode( i ) << sym;
    }

    std::cout << " ";

//    for ( size_t i = 8; i < 16; i++ ) {
//        std::cout << mc::ColorTable::ansiEscapeCode( i ) << sym;
//    }

//    std::cout << std::endl;

    std::cout << mc::ColorTable::ansiEscapeCode( "#000000" ) << sym;
    std::cout << mc::ColorTable::ansiEscapeCode( "#ff0000" ) << sym;
    std::cout << mc::ColorTable::ansiEscapeCode( "#00ff00" ) << sym;
    std::cout << mc::ColorTable::ansiEscapeCode( "#ffff00" ) << sym;
    std::cout << mc::ColorTable::ansiEscapeCode( "#0000ff" ) << sym;
    std::cout << mc::ColorTable::ansiEscapeCode( "#ff00ff" ) << sym;
    std::cout << mc::ColorTable::ansiEscapeCode( "#00ffff" ) << sym;
    std::cout << mc::ColorTable::ansiEscapeCode( "#ffffff" ) << sym;
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
                std::cout << mc::ColorTable::ansiEscapeCode( ss.str() ) << sym;
            }

            std::cout << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;

    //  NOTE(tgurdan): uint8_t causes endless loop here
    for ( uint16_t i = 232; i < 255; i++ ) {
        std::cout << mc::ColorTable::ansiEscapeCode( uint8_t( i ) ) << sym;
    }

    std::cout << std::endl << std::endl;
}


}   //  mc
