#include "bugle/recipient/consolelogger.h"
#include "bugle/format/colortable.h"
#include "bugle/format/duration.h"
#include "bugle/format/doge.h"

#include <print>
#include <iostream>
#include <thread>
#include <random>

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
ConsoleLogger::ConsoleLogger() {
    formatter_ = std::make_shared<AsciiFormatter>();
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

    if ( ! letter.tags.contains( "envelope" ) ) {
        std::println( "{}", formatter_->format( letter ) );
        return;
    }

    Letter envelope = letter;

    if ( letter.attributes.at( "open" ) ) {
        envelope.message = std::format( "{} opened ...", letter.message );
    } else {
        const std::string duration = durationInfo( letter.attributes.at( "duration" ) );
        auto gen = std::mt19937{std::random_device{}()};
        std::vector<std::string> confs( 2 );
        std::ranges::sample( firstWords, confs.begin(), 1, gen );
        std::ranges::sample( secondWords, confs.begin() + 1, 1, gen );
        envelope.message = std::format( "\033[3m{} {} ({})\033[0m", confs[0], confs[1], duration );
    }

    std::println( "{}", formatter_->format( envelope ) );
}


}   //  ::bugle
