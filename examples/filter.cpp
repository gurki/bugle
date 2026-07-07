//  filter demo: recipients subscribe to boolean tag/attribute expressions.
//  literals on one line are AND-ed, lines are OR-ed, '!' negates, and
//  attributes support value comparisons (>, >=, <, <=, =, !=).

#include <bugle/bugle.h>


////////////////////////////////////////////////////////////////////////////////
int main()
{
    auto& po = bugle::PostOffice::instance();

    //  this console only receives letters matching the expression:
    //  (info AND NOT debug AND NOT value > 100) OR any measurement
    const bugle::Filter filter = bugle::Filter::fromString( R"(
        tag:info !tag:debug !attribute:value>100
        tag:measurement
    )" );

    auto console = std::make_shared<bugle::ConsoleLogger>();
    po.addObserver( console, filter );

    po.post( "shows: plain info", { "info" } );
    po.post( "hidden: tagged debug", { "info", "debug" } );
    po.post( "shows: value in range", { "info" }, {{ "value", 42 }} );
    po.post( "hidden: value too large", { "info" }, {{ "value", 1000 }} );
    po.post( "shows: measurements always pass", { "measurement" }, {{ "value", 1000 }} );
    po.post( "hidden: no matching tag", { "trace" } );

    //  filters can also be composed programmatically with custom predicates
    bugle::ValueFilter everyTenth( "tick", 10,
        []( const nlohmann::json& value, const nlohmann::json& divisor ) {
            return ( value.get<int>() % divisor.get<int>() ) == 0;
        }
    );

    auto decimator = std::make_shared<bugle::ConsoleLogger>();
    po.addObserver( decimator, everyTenth );

    for ( int tick = 1; tick <= 30; tick++ ) {
        po.post( "tick", { "trace" }, {{ "tick", tick }} );
    }

    po.flush();
}
