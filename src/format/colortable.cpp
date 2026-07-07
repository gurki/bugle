#include "bugle/format/colortable.h"
#include "bugle/format/colors.h"

#include <algorithm>
#include <cmath>
#include <ios>  //  std::hex
#include <regex>
#include <sstream>

namespace bugle {


nlohmann::json ColorTable::table_ = {};
ColorTable ColorTable::instance_ = {};


////////////////////////////////////////////////////////////////////////////////
ColorTable::ColorTable() {
    table_ = kColorNames;
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


}   //  ::bugle
