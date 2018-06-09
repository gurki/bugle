#pragma once

#include <json.hpp>

#include <QString>
#include <QColor>

namespace mc {

using namespace nlohmann;

//  c.f.
//    https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
//    https://jonasjacek.github.io/colors/

class ColorTable
{
    public:

        ColorTable();

        static uint8_t ansiIndex( const QColor& color );
        static QString hex( const uint8_t index );
        static QString ansiEscapeCode( const QColor& color );
        static QString ansiEscapeCode( const uint8_t index );

    private:

        static json table_;
        static ColorTable instance_;
};


QString defaultColor();
QString lightColor();
QPair<uint8_t, uint8_t> defaultTagColors();
QPair<uint8_t, uint8_t> defaultTextColors();

}
