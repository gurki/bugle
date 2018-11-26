#include "messagecenter/datetime.h"

#include <iostream>
#include <sstream>
#include <iomanip>  //  std::put_time
#include <ctime>    //  localtime
#include <regex>


namespace mc {


using std::chrono::system_clock;


//////////////////////////////////////////////////////////////////////////////////
DateTime::DateTime()
{}


//////////////////////////////////////////////////////////////////////////////////
std::string DateTime::info( const Resolution resolution ) const {
    return dateInfo() + "T" + timeInfo( resolution );
}


//////////////////////////////////////////////////////////////////////////////////
std::string DateTime::timeInfo( const Resolution resolution ) const
{
    const auto time = system_clock::to_time_t( timestamp_ );

    std::stringstream ss;
    ss << std::put_time( std::localtime( &time ), "%T" );

    if ( resolution == Microseconds ) {
        ss << "." << microseconds();
    } else if ( resolution == Milliseconds ) {
        ss << "." << milliseconds();
    }

    return ss.str();
}


//////////////////////////////////////////////////////////////////////////////////
std::string DateTime::dateInfo() const
{
    const auto time = system_clock::to_time_t( timestamp_ );

    std::stringstream ss;
    ss << std::put_time( std::localtime( &time ), "%F" );

    return ss.str();
}


//////////////////////////////////////////////////////////////////////////////////
uint16_t DateTime::milliseconds() const {
    using Ms = std::chrono::milliseconds;
    auto timeMs = std::chrono::time_point_cast<Ms>( timestamp_ );
    return ( timeMs.time_since_epoch().count() % 1000 );
}


//////////////////////////////////////////////////////////////////////////////////
uint32_t DateTime::microseconds() const {
    using Us = std::chrono::microseconds;
    auto timeUs = std::chrono::time_point_cast<Us>( timestamp_ );
    return ( timeUs.time_since_epoch().count() % 1000000 );
}


//////////////////////////////////////////////////////////////////////////////////
DateTime DateTime::now() {
    DateTime dt = {};
    dt.timestamp_ = system_clock::now();
    return dt;
}


//////////////////////////////////////////////////////////////////////////////////
DateTime DateTime::parse( const std::string& str ) 
{
    std::regex re( "(\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2})(?:.(\\d+))?" );
    std::smatch match;
    std::regex_match( str, match, re );

    if ( match.empty() ) {
        return {};
    }

    std::tm t;
    std::istringstream istr( match[1] );
    istr >> std::get_time( &t, "%FT%T" );
    std::time_t time = std::mktime( &t );

    DateTime dt;
    dt.timestamp_ = std::chrono::system_clock::from_time_t( time );

    if ( match.length( 2 ) > 0 ) 
    {
        const int f = std::stoi( match[ 2 ] );

        if ( f < 999 ) {
            dt.timestamp_ += std::chrono::milliseconds( f );
        } else {
            dt.timestamp_ += std::chrono::microseconds( f );
        }
    }

    return dt;
}


//////////////////////////////////////////////////////////////////////////////////
void to_json( nlohmann::json& json, const DateTime& datetime ) {
    json = datetime.info( DateTime::Microseconds );
}


//////////////////////////////////////////////////////////////////////////////////
void from_json( const nlohmann::json& json, DateTime& datetime ) {
    datetime = DateTime::parse( json.get<std::string>() );
}


}   //  mc::
