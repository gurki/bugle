#pragma once

#include <nlohmann/json.hpp>

//  c.f.
//    https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
//    https://jonasjacek.github.io/colors/


namespace mc {


//  256 ansi colors
//  indexed, with names and different representations
//  requires 'colors.json' to be in the specified MC_RESOURCE_DIR directory
class ColorTable
{
    public:

        static std::string hex( const uint8_t id );
        static std::string name( const uint8_t id );
        static uint8_t findHex( const std::string& hex );
        static uint8_t findName( const std::string& name );

        static void printTestTable( const uint8_t numSteps = 6 );

    private:

        ColorTable();
        ~ColorTable() {}

        static void load( const std::string& path );

        static nlohmann::json table_;
        static ColorTable instance_;    //  trigger population of table_
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

        static uint8_t fromRGBA( const uint32_t rgba );

    private:

        uint8_t id_ = 0;
};


}   //  mc::
