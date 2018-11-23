#pragma once

#include <nlohmann/json.hpp>


namespace mc {


//  c.f.
//    https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
//    https://jonasjacek.github.io/colors/


class ColorTable
{
    public:

        ColorTable();

        static uint8_t colorId( const std::string& hexString );
        static std::string hexString( const uint8_t index );
        static uint8_t findName( const std::string& name );
        static std::string colorName( const uint8_t id );
        static std::string ansiEscapeCode( const std::string& hexString );
        static std::string ansiEscapeCode( const uint8_t colorId );
        static void printTestTable( const uint8_t numSteps = 6 );

    private:

        static nlohmann::json table_;
        static ColorTable instance_;
};


}   //  mc::
