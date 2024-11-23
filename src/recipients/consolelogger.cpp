#include "bugle/recipients/consolelogger.h"
#include "bugle/format/colortable.h"
#include "bugle/format/duration.h"
#include "bugle/format/doge.h"
#include "bugle/utility/buildinfo.h"
#include "bugle/utility/sessioninfo.h"
#include "bugle/utility/gpuinfo.h"

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

        if ( letter.tags.contains( "gpu" ) ) {
            logGpu( letter );
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

    if ( letter.attributes.at( "open" ) )
    {
        const std::string name = (
            letter.message.empty() ?
            letter.functionInfo() :
            std::format( "{}::{}", letter.functionInfo(), letter.message )
        );

        envelope.message = std::format( "{} …", name );
    }
    else {
        const std::string duration = durationInfo( letter.attributes.at( "duration" ) );
        envelope.message = std::format( "\033[3m({}) {}\033[0m", duration, randomDoge() );
    }

    std::println( "{}", formatter_->format( envelope ) );
    std::fflush( nullptr );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::logBuild( const Letter& letter )
{
    BuildInfo info = nlohmann::json( letter.attributes );

    h1( "🚧 BUILD" );

    //  environment
    h2( "🌳 Environment" );
    kv( "timestamp:", info.timestamp );
    kv( "bugle:", info.bugle );
    kv( "host:", info.host, true );
    kv( "directory:", info.directory );

    //  compilation
    h2( "🏭 Compilation" );
    kv( "type:", info.type );
    kv( "cmake:", info.cmakeVersion );
    kv( "generator:", info.cmakeGenerator );
    kv( "compiler:", info.compilerName );
    kv( "version:", info.compilerVersion, true, true );

    // //  system
    // h2( "💻 System" );
    // kv( "name:", info.systemName );
    // kv( "version:", info.systemVersion );
    // kv( "architecture:", info.systemArchitecture, true );

    // //  hardware
    // h2( "💾 Hardware" );
    // kv( "cpu:", info.cpuName );
    // kv( "cores:", std::format( "{} / {}", info.cpuCoresPhysical, info.cpuCoresLogical ) );
    // kv( "ram:", std::format( "{:.2f} GiB / {:.2f} GiB", info.ramAvailableMb / 1024.f, info.ramTotalMb / 1024.f ) );
    // kv( "vram:", std::format( "{:.2f} GiB / {:.2f} GiB", info.vramAvailableMb / 1024.f, info.vramTotalMb / 1024.f ), true, true );

    std::println( "" );
    std::fflush( nullptr );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::logSession( const Letter& letter )
{
    SessionInfo info = nlohmann::json( letter.attributes );

    h1( "💡 SESSION" );

    //
    h2( "🍎 Application" );
    kv( "timestamp:", info.timestamp );
    kv( "app:", info.appName );
    kv( "version:", info.appVersion );
    kv( "commit:", info.appCommit, true );

    //  system
    h2( "💻 System" );
    kv( "name:", info.systemName );
    kv( "version:", info.systemVersion );
    kv( "architecture:", info.systemArchitecture, true );

    //  hardware
    h2( "💾 Hardware" );
    kv( "cpu:", std::format( "{}", info.cpuModel ) );
    kv( "cores:", std::format( "{}", info.cpuCores ) );
    kv( "ram:", std::format( "{:.2f} GiB / {:.2f} GiB", info.ramAvailableMb / 1024.f, info.ramTotalMb / 1024.f ), true, true );

    std::println( "" );
    std::fflush( nullptr );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::logGpu( const Letter& letter )
{
    bugle::GpuInfo info = nlohmann::json( letter.attributes );

    h1( "🎨 GPU" );

    //  renderer
    h2( "👩‍🎨 Renderer" );
    kv( "renderer:", info.renderer );
    kv( "version:", info.version );
    kv( "ram:", std::format( "{:.2f} GiB / {:.2f} GiB", info.ramAvailableMb / 1024.f, info.ramTotalMb / 1024.f ), true );

    //  capabilities
    h2( "🦾 Capabilities" );
    kv( "maxPatchVertices:", info.maxPatchVertices );
    kv( "maxTextureImageUnits:", info.maxTextureImageUnits );
    kv( "maxTextureSize:", info.maxTextureSize );
    kv( "maxArrayTextureLayers:", info.maxArrayTextureLayers );
    kv( "max3dTextureSize:", info.max3dTextureSize, true, true );

    std::println( "" );
    std::fflush( nullptr );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::h1( const std::string& title ) {
    std::println( "┌ {}",
        formatter_->colorize( title, tx )
    );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::h2( const std::string& title ) {
    std::println( "│ ┌ {}",
        formatter_->colorize( title, tx )
    );
}


//////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::kv( const std::string& key, const auto& val, const bool closeInner, const bool closeOuter ) 
{
    std::println( "{} {} {:<20} {}",
        closeOuter ? "└" : "│",
        closeInner ? "└" : "├",
        formatter_->colorize( key, tx3 ),
        formatter_->colorize( std::format( "{}", val ), tx2 )
    );
}


}   //  ::bugle
