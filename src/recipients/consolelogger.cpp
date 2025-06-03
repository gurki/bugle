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

    if ( letter.tags.contains( "envelope" ) ) {
        logEnvelope( letter );
        return;
    }

    std::println( "{}", formatter_->format( letter ) );

    if ( letter.attributes.contains( "_title" ) ) {
        logAttributes( letter.attributes );
    }

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
void ConsoleLogger::logAttributes( const attributes_t& attributes )
{
    if ( attributes.contains( "_title" ) )
    {
        std::string heading = attributes.at( "_title" );
        std::ranges::transform( 
            heading, heading.begin(), 
            []( unsigned char c ) { 
                return (char)( std::toupper( c ) ); 
            }
        );

        if ( attributes.contains( "_icon" ) ) {
            h1( std::format( "{} {}", attributes.at( "_icon" ).get<std::string>(), heading ) );
        } else {
            h1( std::format( "{}", heading ) );
        }
    }
    
    int sectionId = 0;

    for ( const auto& section : attributes ) 
    {
        sectionId++;
        
        if ( ! section.second.is_object() ) {
            continue;
        }
        
        std::string title = section.first;
        title[ 0 ] = std::toupper( title[ 0 ] );

        const auto& items = section.second;
        
        if ( items.contains( "_icon" ) ) {
            h2( std::format( "{} {}", items.value( "_icon", "" ), title ) );
        } else {
            h2( std::format( "{}", title ) );
        }

        int itemId = 0;

        for ( const auto& item : items.items() ) 
        {
            itemId++;

            if ( item.key() == "_title" || item.key() == "_icon" ) {
                continue;
            }

            std::string value;

            if ( item.value().is_number_float() ) {
                value = std::format( "{:.2f}", item.value().get<float>() );
            } else if ( item.value().is_string() ) {
                value = item.value();
            } else {
                value = item.value().dump();
            }

            kv( 
                item.key(), 
                value,
                itemId == items.size(), 
                itemId == items.size() && sectionId == attributes.size() 
            );
        }
    }

    std::fflush( nullptr );
}


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::h1( const std::string& title ) {
    std::println( "  {} {}",
        formatter_->colorize( "┌", 244 ),
        formatter_->colorize( title, tx1_ )
    );
};


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::h2( const std::string& title ) {
    std::println( "  {} {} {}",
        formatter_->colorize( "│", 244 ),
        formatter_->colorize( "┌", 244 ),
        formatter_->colorize( title, tx1_ )
    );
};


////////////////////////////////////////////////////////////////////////////////
void ConsoleLogger::kv( 
    const std::string& key, 
    const auto& val,
    const bool closeInner, 
    const bool closeOuter ) 
{
    std::println( "  {} {} {:<20}: {}",
        formatter_->colorize( closeOuter ? "└" : "│", 244 ),
        formatter_->colorize( closeInner ? "└" : "├", 244 ),
        formatter_->colorize( key, tx3_ ),
        formatter_->colorize( std::format( "{}", val ), tx2_ )
    );
};


}   //  ::bugle
