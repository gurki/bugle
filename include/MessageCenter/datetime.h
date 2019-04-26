#pragma once

#include <nlohmann/json.hpp>

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

        bool valid() const { return timestamp_.time_since_epoch().count() > 0; }
        std::string info( const Resolution resolution = Seconds ) const;
        std::string timeInfo( const Resolution resolution = Seconds ) const;
        std::string dateInfo() const;
        uint16_t milliseconds() const;
        uint32_t microseconds() const;
        double elapsed() const;
        uint32_t elapsedMicros() const;

        static DateTime now();
        static DateTime parse( const std::string& str );

        friend void to_json( nlohmann::json& json, const DateTime& message );
        friend void from_json( const nlohmann::json& json, DateTime& message );

    private:

        std::chrono::system_clock::time_point timestamp_ = {};
};


}   //  ::mc
