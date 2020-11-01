#include "bugle/sinks/consolelogger.h"
#include "bugle/format/colortable.h"

#include <fmt/format.h>
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
void ConsoleLogger::notify( const Message& message )
{
    std::unique_lock lock( mutex_ );

    if ( ! formatter_  ) {
        return;
    }

    fmt::print( "{} \n", formatter_->format( message ) );
}


}   //  ::bugle
