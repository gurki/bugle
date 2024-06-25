#include "bugle/recipient/consolelogger.h"
#include "bugle/format/colortable.h"
#include "bugle/format/duration.h"
#include "bugle/format/doge.h"
#include "bugle/utility/buildinfo.h"
#include "bugle/utility/sessioninfo.h"

#include <print>

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

    if ( letter.tags.contains( "system" ) ) 
    {
        if ( letter.tags.contains( "build" ) ) {
            logBuild( letter );
            return;
        } 
        
        if ( letter.tags.contains( "session" ) ) {
            logSession( letter );
            return;
        }
    }

    if ( letter.tags.contains( "envelope" ) ) {
        logEnvelope( letter );
        return;
    }

    std::println( "{}", formatter_->format( letter ) );
    std::fflush( nullptr );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::logEnvelope( const Letter& letter ) 
{    
    Letter envelope = letter;

    if ( letter.attributes.at( "open" ) ) {
        envelope.message = std::format( "{} opened ...", letter.message );
    } else {
        const std::string duration = durationInfo( letter.attributes.at( "duration" ) );
        envelope.message = std::format( "\033[3m{} ({})\033[0m", randomDoge(), duration );
    }

    std::println( "{}", formatter_->format( envelope ) );
    std::fflush( nullptr );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::logBuild( const Letter& letter ) 
{
    BuildInfo info = nlohmann::json( letter.attributes );

    const auto tx = 254;
    const auto tx2 = 250;
    const auto tx3 = 246;

    const auto h1 = [ this ]( const std::string& title ) {
        std::println( "\n{}", 
            formatter_->colorize( title, tx )
        );
    };

    const auto h2 = [ this ]( const std::string& title ) {
        std::println( "\n{}", 
            formatter_->colorize( title, tx )
        );
    };

    const auto kv = [ this ]( const std::string& key, const auto& val ) {
        std::println( "{:<12} {}", 
            formatter_->colorize( key, tx3 ),
            formatter_->colorize( std::format( "{}", val ), tx2 ) 
        );
    };

    h1( "🚧 BUILD" );

    //  environment
    h2( "🌳 Environment" );
    kv( "timestamp:", info.timestamp );
    kv( "bugle:", info.bugle );
    kv( "host:", info.host );
    // kv( "directory:", info.directory );

    //  compilation
    h2( "🏭 Compilation" );
    kv( "type:", info.type );
    kv( "cmake:", info.cmakeVersion );
    kv( "generator:", info.cmakeGenerator );
    kv( "compiler:", info.compilerName );
    kv( "version:", info.compilerVersion );

    // //  system
    // h2( "💻 System" );
    // kv( "name:", info.systemName );
    // kv( "version:", info.systemVersion );
    // kv( "architecture:", info.systemArchitecture );

    // //  hardware
    // h2( "💾 Hardware" );
    // kv( "cpu:", info.cpuName );
    // kv( "cores:", std::format( "{} / {}", info.cpuCoresPhysical, info.cpuCoresLogical ) );
    // kv( "ram:", std::format( "{:.2f} GiB / {:.2f} GiB", info.ramAvailableMb / 1024.f, info.ramTotalMb / 1024.f ) );
    // kv( "vram:", std::format( "{:.2f} GiB / {:.2f} GiB", info.vramAvailableMb / 1024.f, info.vramTotalMb / 1024.f )  );

    std::println( "" );
    std::fflush( nullptr );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::logSession( const Letter& letter ) 
{
    SessionInfo info = nlohmann::json( letter.attributes );

    const auto tx = 254;
    const auto tx2 = 250;
    const auto tx3 = 246;

    const auto h1 = [ this ]( const std::string& title ) {
        std::println( "\n{}", 
            formatter_->colorize( title, tx )
        );
    };

    const auto h2 = [ this ]( const std::string& title ) {
        std::println( "\n{}", 
            formatter_->colorize( title, tx )
        );
    };

    const auto kv = [ this ]( const std::string& key, const auto& val ) {
        std::println( "{:<12} {}", 
            formatter_->colorize( key, tx3 ),
            formatter_->colorize( std::format( "{}", val ), tx2 ) 
        );
    };

    h1( "💡 SESSION" );

    //  
    h2( "🍎 Application" );
    kv( "timestamp:", info.timestamp );
    kv( "app:", info.appName );
    kv( "version:", info.appVersion );
    kv( "commit:", info.appCommit );

    //  system
    h2( "💻 System" );
    kv( "name:", info.systemName );
    kv( "version:", info.systemVersion );
    kv( "architecture:", info.systemArchitecture );

    //  hardware
    h2( "💾 Hardware" );
    kv( "cpu:", std::format( "{}", info.cpuModel ) );
    kv( "cores:", std::format( "{}", info.cpuCores ) );
    kv( "ram:", std::format( "{:.2f} GiB / {:.2f} GiB", info.ramAvailableMb / 1024.f, info.ramTotalMb / 1024.f ) );

    std::println( "" );
    std::fflush( nullptr );
}


}   //  ::bugle
