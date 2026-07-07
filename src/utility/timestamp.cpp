#include "bugle/utility/timestamp.h"

#include <sstream>
#include <iomanip>  //  std::get_time
#include <ctime>    //  mktime
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

    std::tm t {};
    std::istringstream istr( str );
    istr >> std::get_time( &t, "%Y-%m-%dT%H:%M:%S" );

    //  interpret as utc, matching the utc-based output of info()
#ifdef _WIN32
    std::time_t time = _mkgmtime( &t );
#else
    std::time_t time = timegm( &t );
#endif

    Timestamp dt = { std::chrono::system_clock::from_time_t( time ) };

    if ( match.length( 2 ) > 0 )
    {
        //  scale the fraction by its digit count, e.g. ".5" -> 500 ms,
        //  ".050" -> 50 ms, ".000123" -> 123 us
        const std::string digits = match[ 2 ].str().substr( 0, 6 );
        int64_t f = std::stoll( digits );

        for ( size_t i = digits.size(); i < 6; i++ ) {
            f *= 10;
        }

        dt += std::chrono::microseconds( f );
    }

    return dt;
}


//////////////////////////////////////////////////////////////////////////////////
std::string Timestamp::fileInfo() const {
    const auto tp = std::chrono::time_point_cast<std::chrono::seconds>( *this );
    const auto zt = std::chrono::zoned_time( std::chrono::current_zone(), tp );
    return std::format( "{:%F}_{:%H%M%S}", zt, zt );
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
