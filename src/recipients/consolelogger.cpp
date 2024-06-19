#include "bugle/recipient/consolelogger.h"
#include "bugle/format/colortable.h"
#include "bugle/format/duration.h"

#include <print>
#include <iostream>
#include <thread>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
ConsoleLogger::ConsoleLogger() {
    formatter_ = std::make_shared<AsciiFormatter>();
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::setFormatter( const FormatterPtr& formatter ) {
    std::unique_lock lock( mutex_ );
    formatter_ = formatter;
}


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::receive( const Letter& letter )
{
    std::unique_lock lock( mutex_ );

    if ( ! formatter_  ) {
        return;
    }

    if ( ! letter.tags.contains( "envelope" ) ) {
        std::println( "{}", formatter_->format( letter ) );
        return;
    }

    Letter envelope = letter;

    if ( letter.attributes.at( "open" ) ) {
        envelope.message = std::format( "{} opened ...", letter.message );
    } else {
        const std::string duration = durationInfo( letter.attributes.at( "duration" ) );
        envelope.message = std::format( "{} closed ({})", letter.message, duration );
    }

    std::println( "{}", formatter_->format( envelope ) );
}


}   //  ::bugle
