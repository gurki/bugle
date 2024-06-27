#include "bugle/format/colortable.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>  //  std::hex
#include <regex>
#include <print>

namespace bugle {


using namespace std::string_literals;


nlohmann::json ColorTable::table_ = {};
ColorTable ColorTable::instance_ = {};


////////////////////////////////////////////////////////////////////////////////
ColorTable::ColorTable() {
    load( "res/colors.json" );
}


////////////////////////////////////////////////////////////////////////////////
void ColorTable::load( const std::string& path )
{
    std::ifstream fin( path );

    if ( ! fin.is_open() ) {
        std::println( "couldn't load {}", path );
        return;
    }

    table_ = nlohmann::json::parse( fin );
}


////////////////////////////////////////////////////////////////////////////////
std::string ColorTable::hex( const uint8_t id ) {
    return table_.at( id ).at( "hexString" );
}


////////////////////////////////////////////////////////////////////////////////
std::string ColorTable::name( const uint8_t id )  {
    return table_.at( id ).at( "name" );
}


////////////////////////////////////////////////////////////////////////////////
uint8_t ColorTable::findHex( const std::string& hex )
{
    if ( table_.is_null() ) {
        return 0;
    }

    auto it = std::find_if(
        table_.begin(), table_.end(),
        [ &hex ]( const nlohmann::json& item ) {
            return item[ "hexString" ] == hex;
        }
    );

    if ( it == table_.end() ) {
        return 0;
    }

    return it->at( "colorId" );
}


////////////////////////////////////////////////////////////////////////////////
uint8_t ColorTable::findName( const std::string& name )
{
    if ( table_.is_null() ) {
        return 0;
    }

    auto it = std::find_if(
        table_.begin(), table_.end(),
        [ &name ]( const nlohmann::json& item ) {
            return item[ "name" ] == name;
        }
    );

    if ( it == table_.end() ) {
        return 0;
    }

    return it->at( "colorId" );
}


//////////////////////////////////////////////////////////////////////////////////
void ColorTable::printTestTable( const uint8_t numSteps )
{
    static const std::string sym = "\xe2\x97\x8f";

    if ( numSteps == 0 ) {
        return;
    }

    std::cout << std::endl;

    auto ansi = []( const Color& col ) -> std::string {
       return "\x1b[38;5;" + std::to_string( col.id() ) + "m";
    };

    for ( uint8_t i = 0; i < 8; i++ ) {
        std::cout << ansi( i ) << sym;
    }

    std::cout << " ";
    std::cout << ansi( "#000000"s ) << sym;
    std::cout << ansi( "#ff0000"s ) << sym;
    std::cout << ansi( "#00ff00"s ) << sym;
    std::cout << ansi( "#ffff00"s ) << sym;
    std::cout << ansi( "#0000ff"s ) << sym;
    std::cout << ansi( "#ff00ff"s ) << sym;
    std::cout << ansi( "#00ffff"s ) << sym;
    std::cout << ansi( "#ffffff"s ) << sym;
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
                std::cout << ansi( ss.str() ) << sym;
            }

            std::cout << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;

    //  NOTE(tgurdan): uint8_t causes endless loop here
    for ( uint16_t i = 232; i <= 255; i++ ) {
        std::cout << ansi( uint8_t( i ) ) << sym;
    }

    std::cout << std::endl << std::endl;
}


//////////////////////////////////////////////////////////////////////////////////
Color::Color( const uint8_t index ) :
    id_( index )
{}


//////////////////////////////////////////////////////////////////////////////////
Color::Color( const std::string& hexOrName )
{
    //  try hex
    std::regex re( "^#((?:[0-9a-fA-F]{3}){1,2})$" );
    std::smatch match;
    std::regex_match( hexOrName, match, re );

    //  revert to name
    if ( match.empty() ) {
        id_ = ColorTable::findName( hexOrName );
        return;
    }

    //  parse hex

    std::stringstream ss;
    const std::string& hexstr = match[ 1 ];

    ss << std::hex;

    if ( hexstr.size() == 3 )  {
        //  unpack 3-digit hex
        for ( const auto& c : hexstr ) {
            ss << c << c;
        }
    } else {
        ss << hexstr;
    }

    uint32_t rgba;
    ss >> rgba;

    id_ = fromRGBA( rgba );
}


//////////////////////////////////////////////////////////////////////////////////
std::string Color::hex() const {
    return ColorTable::hex( id_ );
}


//////////////////////////////////////////////////////////////////////////////////
std::string Color::name() const {
    return ColorTable::name( id_ );
}


//////////////////////////////////////////////////////////////////////////////////
uint8_t Color::fromRGBA( const uint32_t rgba )
{
    auto conv = []( const int v ) -> int {
        return int( std::roundf( 5.f * v / 255.f ) );
    };

    const int r = conv( ( rgba >> 16 ) & 0xff );
    const int g = conv( ( rgba >> 8 ) & 0xff );
    const int b = conv( rgba & 0xff  );

    return ( 16 + 36 * r + 6 * g + b );
}


}   //  mc
