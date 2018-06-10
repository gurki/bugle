#pragma once

#include <nlohmann/json.hpp>


namespace mc {


using namespace nlohmann;

//  c.f.
//    https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
//    https://jonasjacek.github.io/colors/


class ColorTable
{
    public:

        ColorTable();

        static uint8_t ansiIndex( const std::string& hex );
        static std::string hex( const uint8_t index );
        static std::string ansiEscapeCode( const std::string& hex );
        static std::string ansiEscapeCode( const uint8_t index );
        static void printTestTable( const uint8_t numSteps = 6 );

    private:

        static nlohmann::json table_;
        static ColorTable instance_;
};


}   //  mc::
