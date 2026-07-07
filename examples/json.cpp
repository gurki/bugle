//  json logger demo: every letter serializes to one line of json
//  (jsonl / ndjson), ideal for ingestion and offline analysis

#include <bugle/bugle.h>

#include <fstream>
#include <print>


////////////////////////////////////////////////////////////////////////////////
int main()
{
    auto& po = bugle::PostOffice::instance();

    constexpr auto filepath = "logs/json-example.jsonl";

    auto json = std::make_shared<bugle::JsonLogger>();
    json->open( filepath );     //  open() without argument timestamps the filename

    po.addObserver( json );

    for ( int i = 0; i < 3; i++ ) {
        po.post( "sample", { "measurement" }, {
            { "index", i },
            { "temperature", 20.f + 0.5f * i }
        });
    }

    po.post( "shutting down", { "info" } );
    po.flush();

    //  recipients are held as weak_ptr: releasing ours unregisters the
    //  observer and closes the file
    json.reset();

    //  show what landed on disk
    std::println( "content of '{}':\n", filepath );

    std::ifstream fin( filepath );

    for ( std::string line; std::getline( fin, line ); ) {
        std::println( "{}", line );
    }
}
