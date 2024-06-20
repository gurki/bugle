#include "bugle/recipient/jsonlogger.h"
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
bool JsonLogger::open( const std::string& filename )
{
#ifdef BUGLE_ENABLE
    fout_.close();

    std::string filepath = filename;

    if ( filename.empty() ) {
        const auto dt = Timestamp::now();
        filepath = std::format( "logs/{}.jsonl", dt.fileInfo() );
    }

    const auto directory = std::filesystem::relative( filepath ).parent_path();

    if ( ! std::filesystem::exists( directory ) )
    {
        const bool succ = std::filesystem::create_directories( directory );

        if ( ! succ ) {
            return false;
        }
    }

    fout_.open( filepath );
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
    fout_ << data.dump() << "\n";
}


}   //  ::bugle