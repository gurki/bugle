#include "bugle/recipients/jsonlogger.h"
#include "bugle/utility/timestamp.h"
#include "bugle/core/letter.h"

#include <format>
#include <filesystem>
#include <print>

namespace bugle {

////////////////////////////////////////////////////////////////////////////////
JsonLogger::~JsonLogger() {
    fout_.close();
}


////////////////////////////////////////////////////////////////////////////////
bool JsonLogger::open( const std::string& filename, const Format format )
{
#ifdef BUGLE_ENABLE
    fout_.close();
    format_ = format;

    std::string filepath = filename;

    if ( filename.empty() ) {
        const auto dt = Timestamp::now();
        const auto extension = ( format == Format::Cbor ) ? "cborseq" : "jsonl";
        filepath = std::format( "logs/{}.{}", dt.fileInfo(), extension );
    }

    const auto directory = std::filesystem::path( filepath ).parent_path();

    if ( ! std::filesystem::exists( directory ) )
    {
        const bool succ = std::filesystem::create_directories( directory );

        if ( ! succ ) {
            return false;
        }
    }

    if ( format == Format::Cbor ) {
        fout_.open( filepath, std::ios_base::binary );
    } else {
        fout_.open( filepath );
    }
#endif
    return fout_.is_open();
}


////////////////////////////////////////////////////////////////////////////////
void JsonLogger::receive( const Letter& letter )
{
    if ( ! fout_.is_open() ) {
        return;
    }

    const nlohmann::json data = letter;

    if ( format_ == Format::Cbor ) {
        const auto bytes = nlohmann::json::to_cbor( data );
        fout_.write( (const char*)bytes.data(), bytes.size() );
        return;
    }

    fout_ << data.dump() << "\n";
}


}   //  ::bugle