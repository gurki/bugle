//  json logger demo: every letter serializes to one line of json
//  (jsonl / ndjson), ideal for ingestion and offline analysis.
//  a binary cbor variant is available for compact logs.

#include <bugle/bugle.h>

#include <filesystem>
#include <fstream>
#include <print>


////////////////////////////////////////////////////////////////////////////////
int main()
{
    auto& po = bugle::PostOffice::instance();

    //  named threads are serialized as 'threadName'
    bugle::PostOffice::setThreadName( "main" );

    constexpr auto jsonlPath = "logs/json-example.jsonl";
    constexpr auto cborPath = "logs/json-example.cborseq";

    auto json = std::make_shared<bugle::JsonLogger>();
    json->open( jsonlPath );    //  open() without arguments timestamps the filename

    auto cbor = std::make_shared<bugle::JsonLogger>();
    cbor->open( cborPath, bugle::JsonLogger::Format::Cbor );

    po.addObserver( json );
    po.addObserver( cbor );

    for ( int i = 0; i < 3; i++ ) {
        po.post( "sample", { "measurement" }, {
            { "index", i },
            { "temperature", 20.f + 0.5f * i }
        });
    }

    po.post( "shutting down", { "info" } );
    po.flush();

    //  recipients are held as weak_ptr: releasing ours unregisters the
    //  observers and closes the files
    json.reset();
    cbor.reset();

    //  show what landed on disk
    std::println( "content of '{}':\n", jsonlPath );

    std::ifstream fin( jsonlPath );

    for ( std::string line; std::getline( fin, line ); ) {
        std::println( "{}", line );
    }

    std::println( "\njsonl: {} bytes, cbor: {} bytes",
        std::filesystem::file_size( jsonlPath ),
        std::filesystem::file_size( cborPath )
    );
}
