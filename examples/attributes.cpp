#include <bugle/bugle.h>
#include <future>


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    auto& po = bugle::PostOffice::instance();
    auto cl = std::make_shared<bugle::ConsoleLogger>();
    auto jl = std::make_shared<bugle::JsonLogger>();
    jl->open();

    // bugle::Route route = R"(
    //     message:main
    //     tag:info !tag:debug !attribute:value>100
    //     attribute:position
    // )";

    auto addressA = std::make_shared<bugle::Address>();
    auto tagA = std::make_shared<bugle::TagFilter>( "info" );
    auto tagB = std::make_shared<bugle::TagFilter>( "debug" );
    auto valA = std::make_shared<bugle::ValueFilter>( "value", 100, std::greater<>{} );
    addressA->lines = { { tagA }, { tagB, true }, { valA, true } };

    auto addressB = std::make_shared<bugle::Address>();
    auto msgA = std::make_shared<bugle::MessageFilter>( "main" );
    addressB->lines = { { msgA } };

    auto route = std::make_shared<bugle::Route>();
    auto attA = std::make_shared<bugle::AttributeFilter>( "position" );
    route->addresses = { addressA, addressB, attA };

    // bugle::Route route = R"(
    //     file:main.cpp
    //     tags:info,debug,!lambda,!scope
    //     attributes:envelope.duration>10 level>=1
    //     attributes:envelope
    //     timestamp>2024-04-25T00:00:00 timestamp<2024-04-25T23:59:59
    // )";
    po.addObserver( cl, route );
    po.addObserver( jl );

    bugle::Envelope scope( po, "main" );

    po.post( "nachos", { "info", "debug" }, {
        { "value", 5 },
        { "position", { 0.5f, 0.2f, 0.1f } }
    });

    po.post( "incoming", { "measurement" }, {
        { "temperature", 10 }
    });

    auto fn = [&]( int val, const std::string& name ) -> float {
        bugle::Envelope scope( po, name );
        std::this_thread::sleep_for( std::chrono::milliseconds( val ) );
        po.post( "go deeper", { "info" }, {{ "value", val }} );
        return -1.f;
    };

    fn( 100, "sync" );

    const auto future = std::async( fn, 200, "async" );
    future.wait();

    scope.close();
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
}


/*

GCC

21:56:54.250 [8ee1]  main opened ...  #envelope  [int main(int, char**)@attributes.cpp:24.39]
21:56:54.250 [8ee1]  ··hallo  #debug #info  [int main(int, char**)@attributes.cpp:26.12]
21:56:54.250 [8ee1]  ··sync opened ...  #envelope  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:32.41]
21:56:54.355 [8ee1]  ····yo, we need to go deeper  #info  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:34.16]
21:56:54.355 [8ee1]  ··sync closed (105 ms)  #envelope  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:32.41]
21:56:54.355 [562c]  async opened ...  #envelope  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:32.41]
21:56:54.560 [562c]  ··yo, we need to go deeper  #info  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:34.16]
21:56:54.560 [562c]  async closed (205 ms)  #envelope  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:32.41]
21:56:54.560 [8ee1]  main closed (310 ms)  #envelope  [int main(int, char**)@attributes.cpp:24.39]

*/