#include <bugle/bugle.h>

#include <future>
#include <execution>
#include <print>
#include <ranges>


////////////////////////////////////////////////////////////////////////////////
std::vector<float> child( const int val = 5 ) {
    bugle::Envelope scope( PO );
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    return {};
}


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    auto& po = bugle::PostOffice::instance();

    auto cl = std::make_shared<bugle::ConsoleLogger>();
    auto jl = std::make_shared<bugle::JsonLogger>();
    auto pr = std::make_shared<bugle::Profiler>();

    jl->open();
    pr->open();

    bugle::Filter routeA = bugle::Filter::fromString( R"(
        message:main
        tag:info !tag:debug !attribute:value>100
        attribute:position
    )" );

    bugle::ValueFilter valA( "value", 1000,
        []( const nlohmann::json& valA, const nlohmann::json& valB ) -> bool {
            return ( valA.get<int>() % valB.get<int>() ) == 0;
        }
    );

    bugle::Filter tagA = bugle::Filter::fromString( "tag:benchmark" );
    bugle::Route routeB({ tagA, valA });

    bugle::Filter tagB = bugle::Filter::fromString( "tag:envelope" );
    bugle::Address addressA({ { tagB, true } });

    po.addObserver( cl );
    po.addObserver( jl );
    po.addObserver( pr, tagB );

    const nlohmann::json buildInfo = bugle::BuildInfo::current();
    const nlohmann::json sessionInfo = bugle::SessionInfo::current();
    po.post( {}, { "session", "system" }, sessionInfo );
    po.post( {}, { "build", "system" }, buildInfo );

    bugle::Envelope scope( po );

    bugle::Envelope outer( po, "outer" );
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        bugle::Envelope inner( po, "inner" );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    outer.close();

    po.post( "hallo", { "info", "debug" }, {
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

    // std::vector<int> vals( 1<<18 );
    // std::iota( vals.begin(), vals.end(), 0 );
    // std::for_each( std::execution::par_unseq, vals.begin(), vals.end(), [ &po ]( const int val ) {
    //     po.post( "", { "benchmark" }, { { "value", val } } );
    // });

    child();

    scope.close();
    po.flush();
}


/*

GCC

21:56:54.250 [8ee1]  main opened ...  #envelope  [int main(int, char**)@attributes.cpp:24.39]
21:56:54.250 [8ee1]  ··hallo  #debug #info  [int main(int, char**)@attributes.cpp:26.12]
21:56:54.250 [8ee1]  ··sync opened ...  #envelope  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:32.41]
21:56:54.355 [8ee1]  ····we need to go deeper  #info  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:34.16]
21:56:54.355 [8ee1]  ··sync closed (105 ms)  #envelope  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:32.41]
21:56:54.355 [562c]  async opened ...  #envelope  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:32.41]
21:56:54.560 [562c]  ··we need to go deeper  #info  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:34.16]
21:56:54.560 [562c]  async closed (205 ms)  #envelope  [main(int, char**)::<lambda(int, const std::string&)>@attributes.cpp:32.41]
21:56:54.560 [8ee1]  main closed (310 ms)  #envelope  [int main(int, char**)@attributes.cpp:24.39]

MSVC

21:19:24.207 [de22]  main opened ...  #envelope  [int __cdecl main(int,char *[]) attributes.cpp:65]
21:19:24.207 [de22]  ··hallo  #info #debug  [int __cdecl main(int,char *[]) attributes.cpp:67]
21:19:24.207 [de22]  ··sync opened ...  #envelope  [float __cdecl main::<lambda_2>::operator ()(int,const class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > &) const attributes.cpp:77]
21:19:24.315 [de22]  ····we need to go deeper  #info  [float __cdecl main::<lambda_2>::operator ()(int,const class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > &) const attributes.cpp:79]
21:19:24.315 [de22]  ··sync closed (108 ms)  #envelope  [float __cdecl main::<lambda_2>::operator ()(int,const class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > &) const attributes.cpp:77]
21:19:24.316 [2c21]  async opened ...  #envelope  [float __cdecl main::<lambda_2>::operator ()(int,const class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >
&) const attributes.cpp:77]
21:19:24.518 [2c21]  ··we need to go deeper  #info  [float __cdecl main::<lambda_2>::operator ()(int,const class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > &) const
attributes.cpp:79]
21:19:24.518 [2c21]  async closed (202 ms)  #envelope  [float __cdecl main::<lambda_2>::operator ()(int,const class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > &) const attributes.cpp:77]
21:19:24.518 [de22]  main closed (311 ms)  #envelope  [int __cdecl main(int,char *[]) attributes.cpp:65]

*/