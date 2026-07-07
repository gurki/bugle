#include "bugle/recipients/consolelogger.h"
#include "bugle/format/duration.h"
#include "bugle/format/doge.h"

#include <print>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
    #include <io.h>
#else
    #include <unistd.h>
#endif

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
//  returns true if stdout is a terminal with working ansi escape support.
//  on windows, this also enables virtual terminal processing and utf-8 output.
static bool enableConsoleColors()
{
#ifdef _WIN32
    if ( ! _isatty( _fileno( stdout ) ) ) {
        return false;
    }

    SetConsoleOutputCP( CP_UTF8 );

    HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );

    if ( handle == INVALID_HANDLE_VALUE || handle == nullptr ) {
        return false;
    }

    DWORD mode = 0;

    if ( ! GetConsoleMode( handle, &mode ) ) {
        return false;
    }

    return SetConsoleMode( handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING );
#else
    return isatty( fileno( stdout ) );
#endif
}


////////////////////////////////////////////////////////////////////////////////
ConsoleLogger::ConsoleLogger()
{
    //  fall back to the color-free base formatter when piped or without
    //  ansi support, so redirected output stays free of escape codes
    if ( enableConsoleColors() ) {
        formatter_ = std::make_shared<AsciiFormatter>();
    } else {
        formatter_ = std::make_shared<Formatter>();
    }
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::setFormatter( const FormatterPtr& formatter ) {
    formatter_ = formatter;
}


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::receive( const Letter& letter )
{
    if ( ! formatter_  ) {
        return;
    }

    if ( letter.tags.contains( "envelope" ) ) {
        logEnvelope( letter );
        return;
    }

    if ( letter.attributes.contains( "_title" ) ) {
        logBanner( letter );
        return;
    }

    std::println( "{}", formatter_->format( letter ) );
    std::fflush( stdout );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::logBanner( const Letter& letter )
{
    //  the message line first for chronology and source location; the banner
    //  content renders below, so strip the attributes from the line itself
    Letter heading = letter;
    heading.attributes.clear();

    std::println( "{}", formatter_->format( heading ) );
    std::println( "{}", formatter_->banner( letter.attributes ) );
    std::fflush( stdout );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::logEnvelope( const Letter& letter )
{
    Letter envelope = letter;

    //  open/duration are internal bookkeeping; the duration is rendered
    //  into the message below, so don't repeat them as attributes
    envelope.attributes.erase( "open" );
    envelope.attributes.erase( "duration" );

    const auto openIt = letter.attributes.find( "open" );
    const bool open = openIt != letter.attributes.end() && openIt->second.is_boolean() && openIt->second.get<bool>();

    if ( open )
    {
        const std::string name = (
            letter.message.empty() ?
            letter.functionInfo() :
            std::format( "{}::{}", letter.functionInfo(), letter.message )
        );

        envelope.message = std::format( "{} …", name );
    }
    else {
        const auto durIt = letter.attributes.find( "duration" );
        const uint64_t durationUs = (
            durIt != letter.attributes.end() && durIt->second.is_number() ?
            durIt->second.get<uint64_t>() : 0
        );

        const std::string duration = durationInfo( durationUs );

        envelope.message = std::format( "{}({}) {}{}",
            formatter_->beginItalic(),
            duration,
            randomDoge(),
            formatter_->endItalic()
        );
    }

    std::println( "{}", formatter_->format( envelope ) );
    std::fflush( stdout );
}


}   //  ::bugle
