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

        static std::string hexString( const uint8_t id );
        static std::string colorName( const uint8_t id );
        static std::string ansiEscapeCode( const uint8_t id );

        static uint8_t colorId( const std::string& hex );
        static std::string ansiEscapeCode( const std::string& hex );

        static uint8_t findName( const std::string& name );
        
        static void printTestTable( const uint8_t numSteps = 6 );

    private:

        static nlohmann::json table_;
        static ColorTable instance_;
};


//  6x6x6 rgb color
class Color 
{
    public:

        Color( const uint8_t index = 0 );
        Color( const std::string& hexOrName );

        uint8_t id() const { return id_; }
        std::string hex() const;
        std::string name() const;

    private:

        uint8_t id_ = 0;
};


}   //  mc::
