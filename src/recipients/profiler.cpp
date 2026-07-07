#include "bugle/recipients/profiler.h"
#include "bugle/core/letter.h"

#include <print>
#include <filesystem>
#include <ranges>

using namespace std::string_view_literals;

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Profiler::~Profiler() {
    fout_.close();
}


////////////////////////////////////////////////////////////////////////////////
void Profiler::receive( const Letter& letter )
{
    //  only well-formed envelope letters carry profiling information
    if ( ! letter.tags.contains( "envelope" ) ) {
        return;
    }

    const auto openIt = letter.attributes.find( "open" );

    if ( openIt == letter.attributes.end() || ! openIt->second.is_boolean() ) {
        return;
    }

    const std::thread::id tid = letter.thread;

    if ( ! stacks_.contains( tid ) ) {
        stacks_[ tid ] = std::vector<std::string>();
        offloads_[ tid ] = std::vector<int>();
    }

    auto& stack = stacks_[ tid ];
    auto& offload = offloads_[ tid ];

    const std::string name = (
        letter.message.empty() ?
        std::format( "{}", letter.functionInfo() ) :
        std::format( "{}::{}", letter.functionInfo(), letter.message )
    );

    if ( openIt->second.get<bool>() ) {
        stack.push_back( name );
        offload.push_back( 0 );
        return;
    }

    //  a close without a matching open, e.g. when attached mid-envelope
    if ( stack.empty() || offload.empty() ) {
        return;
    }

    const auto durIt = letter.attributes.find( "duration" );
    const int dur = (
        durIt != letter.attributes.end() && durIt->second.is_number() ?
        durIt->second.get<int>() : 0
    );

    const auto path = stack | std::views::join_with( ';' );
    const int off = offload.back();
    offload.pop_back();

    const std::string line = std::format( "{};{} {}",
        letter.threadInfo(),
        std::string( path.begin(), path.end() ),
        dur - off
    );

    append( line );

    stack.pop_back();

    if ( offload.empty() ) {
        return;
    }

    offload.back() += dur;

}


////////////////////////////////////////////////////////////////////////////////
bool Profiler::open()
{
    constexpr std::string_view folder = "profiles";

    if ( ! std::filesystem::exists( folder ) ) {
        std::filesystem::create_directory( folder );
    }

    const auto time = std::chrono::system_clock::now();
    const auto outname = std::format(
        "{}/{:%Y-%m-%d_%H%M%S_collapsed.txt}",
        folder,
        std::chrono::time_point_cast<std::chrono::seconds>( time )
    );

    fout_.open( outname, std::ios::app );
    return fout_.is_open();
}


////////////////////////////////////////////////////////////////////////////////
void Profiler::append( const std::string& line )
{
    if ( ! fout_.is_open() ) {
        return;
    }

    fout_ << line << "\n";
}


}   //  ::bugle
