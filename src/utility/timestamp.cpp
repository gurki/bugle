#include "bugle/utility/timestamp.h"

#include <iostream>
#include <sstream>
#include <iomanip>  //  std::put_time
#include <ctime>    //  localtime
#include <regex>


namespace bugle {


using std::chrono::system_clock;


//////////////////////////////////////////////////////////////////////////////////
std::string Timestamp::dateInfo() const {
    return std::format( "{0:%F}", (std::chrono::system_clock::time_point)*this );
}


//////////////////////////////////////////////////////////////////////////////////
uint16_t Timestamp::milliseconds() const {
    using Ms = std::chrono::milliseconds;
    auto timeMs = std::chrono::time_point_cast<Ms>( *this );
    return ( timeMs.time_since_epoch().count() % 1000 );
}


//////////////////////////////////////////////////////////////////////////////////
uint32_t Timestamp::microseconds() const {
    using Us = std::chrono::microseconds;
    auto timeUs = std::chrono::time_point_cast<Us>( *this );
    return ( timeUs.time_since_epoch().count() % 1000000 );
}


//////////////////////////////////////////////////////////////////////////////////
uint64_t Timestamp::elapsedUs() const {
    using Us = std::chrono::microseconds;
    auto durationUs = std::chrono::duration_cast<Us>( system_clock::now() - *this );
    return durationUs.count();
}


//////////////////////////////////////////////////////////////////////////////////
double Timestamp::elapsed() const {
    return elapsedUs() / 1000000.0;
}


//////////////////////////////////////////////////////////////////////////////////
Timestamp Timestamp::now() {
    return { system_clock::now() };
}


//////////////////////////////////////////////////////////////////////////////////
Timestamp Timestamp::parse( const std::string& str )
{
    std::regex re( "(\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2})(?:.(\\d+))?" );
    std::smatch match;
    std::regex_match( str, match, re );

    if ( match.empty() ) {
        return {};
    }

    std::tm t;
    std::istringstream istr( str );
    istr >> std::get_time( &t, "%Y-%m-%dT%H:%M:%S" );
    std::time_t time = std::mktime( &t );

    Timestamp dt = { std::chrono::system_clock::from_time_t( time ) };

    if ( match.length( 2 ) > 0 )
    {
        const int f = std::stoi( match[ 2 ] );

        if ( f < 999 ) {
            dt += std::chrono::milliseconds( f );
        } else {
            dt += std::chrono::microseconds( f );
        }
    }

    return dt;
}


//////////////////////////////////////////////////////////////////////////////////
void to_json( nlohmann::json& json, const Timestamp& datetime ) {
    json = datetime.info<std::chrono::microseconds>();
}


//////////////////////////////////////////////////////////////////////////////////
void from_json( const nlohmann::json& json, Timestamp& datetime ) {
    datetime = Timestamp::parse( json.get<std::string>() );
}


}   //  ::bugle
