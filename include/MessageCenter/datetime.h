#pragma once

#include <chrono>
#include <string>


namespace mc {


class DateTime
{
    public:

        enum Resolution {
            Seconds,
            Milliseconds,
            Microseconds
        };

        DateTime();

        std::string info() const;
        std::string timeInfo( const Resolution resolution ) const;
        std::string dateInfo() const;
        uint16_t milliseconds() const;
        uint32_t microseconds() const;

        static DateTime now();

    private:

        std::chrono::system_clock::time_point timestamp_;
};


}   //  mc::
