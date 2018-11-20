#include "messagecenter/datetime.h"

#include <iostream>
#include <sstream>
#include <iomanip>  //  std::put_time
#include <ctime>    //  localtime_s


namespace mc {


using std::chrono::system_clock;


//////////////////////////////////////////////////////////////////////////////////
DateTime::DateTime()
{}


//////////////////////////////////////////////////////////////////////////////////
std::string DateTime::info() const
{
    const auto time = system_clock::to_time_t( timestamp_ );
    struct tm buf;
    localtime_s( &buf, &time );

    std::stringstream ss;
    ss << std::put_time( &buf, "%F %T" );
    ss << "." << milliseconds();

    return ss.str();
}


//////////////////////////////////////////////////////////////////////////////////
std::string DateTime::timeInfo( const Resolution resolution ) const
{
    const auto time = system_clock::to_time_t( timestamp_ );
    struct tm buf;
    localtime_s( &buf, &time );

    std::stringstream ss;
    ss << std::put_time( &buf, "%T" );

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
    struct tm buf;
    localtime_s( &buf, &time );

    std::stringstream ss;
    ss << std::put_time( &buf, "%F" );

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
    DateTime dt = {};

    std::tm t = {};
    auto res = std::get_time( &t, "%F %T" );
    auto time = std::mktime( &t );

    dt.timestamp_ = std::chrono::system_clock::from_time_t( time );

    return dt;
}


}   //  mc::
