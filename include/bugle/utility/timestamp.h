#pragma once

#include <nlohmann/json.hpp>
#include <chrono>
#include <string>

namespace bugle {


struct Timestamp : public std::chrono::system_clock::time_point
{
    public:

        bool valid() const { return time_since_epoch().count() > 0; }

        template <typename Resolution>
        std::string info() const;

        template <typename Resolution>
        std::string timeInfo() const;

        std::string isoInfo() const;
        std::string fileInfo() const;
        std::string dateInfo() const;
        uint16_t milliseconds() const;
        uint32_t microseconds() const;
        double elapsed() const;
        uint64_t elapsedUs() const;

        static Timestamp now();
        static Timestamp parse( const std::string& str );
};


void to_json( nlohmann::json& json, const Timestamp& message );
void from_json( const nlohmann::json& json, Timestamp& message );


//////////////////////////////////////////////////////////////////////////////////
template <typename Resolution>
std::string Timestamp::info() const {
    return std::format( "{}T{}", dateInfo(), timeInfo<Resolution>() );
}


//////////////////////////////////////////////////////////////////////////////////
template <typename Resolution>
std::string Timestamp::timeInfo() const {
    const auto time = std::chrono::time_point_cast<Resolution>( *this );
    return std::format( "{0:%T}", time );
}


}   //  ::bugle
