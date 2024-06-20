#include "bugle/recipient/jsonlogger.h"
#include "bugle/utility/timestamp.h"
#include "bugle/core/letter.h"

#include <format>
#include <filesystem>

namespace bugle {

////////////////////////////////////////////////////////////////////////////////
JsonLogger::~JsonLogger() {
    fout_.close();
}


////////////////////////////////////////////////////////////////////////////////
bool JsonLogger::open( const std::string& filename )
{
    fout_.close();

    if ( filename.empty() )
    {
        std::filesystem::create_directory( "logs/" );
        const auto dt = Timestamp::now();
        const std::string name = std::format( "logs/{}.jsonl", dt.fileInfo() );
        fout_.open( name );
    }
    else {
        fout_.open( filename );
    }

    return fout_.is_open();
}


////////////////////////////////////////////////////////////////////////////////
void JsonLogger::receive( const Letter& letter ) {
    const nlohmann::json data = letter;
    fout_ << data.dump() << "\n";
}


}   //  ::bugle