#include "bugle/sinks/consolelogger.h"
#include "bugle/format/colortable.h"

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
void ConsoleLogger::notify( const Letter& message )
{
    std::unique_lock lock( mutex_ );

    if ( ! formatter_  ) {
        return;
    }

    std::println( "{}", formatter_->format( message ) );
}


}   //  ::bugle
