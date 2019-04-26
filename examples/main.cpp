#include "scopetest.h"

#include <messagecenter/tags.h>
#include <messagecenter/messagecenter.h>
#include <messagecenter/consolelogger.h>
#include <messagecenter/scope.h>
#include <messagecenter/message.h>
#include <messagecenter/colortable.h>
#include <messagecenter/formatter.h>
#include <messagecenter/theme.h>
#include <messagecenter/filteritem.h>
#include <messagecenter/datetime.h>
//#include <consolelogger.h>
//#include <jsonlogger.h>
//#include <formatter.h>
//#include <htmllogger.h>

#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <utility>
#include <string>


void scopeTest();
//void typeTest();
//void negationTest();
//void colorTest();
//void timingTest();
//void timingTestRandom();


using namespace mc;
using namespace std::chrono_literals;
using namespace std::string_literals;


////////////////////////////////////////////////////////////////////////////////
void parallelMessages()
{
    mcp( "parallel message", "parallel" );

    std::this_thread::sleep_for( 200ms );
    mcp( "later one", { "parallel", "no2" } );
    
    std::this_thread::sleep_for( 200ms );
    mcp( "another one", { "parallel", "no3" } );
}


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    // //  test color

    // Color cols[] = {
    //     "#abc"s,
    //     "#aabbcc"s,
    //     "unknown"s,
    //     "#1a2b3c"s,
    //     17
    // };

    // for ( const auto& col : cols ) {
    //     std::cout << col.name() << std::endl;
    // }

    // std::cout << std::endl;


    // //  test datetime    

    // auto dt = DateTime::now();
    // std::cout << dt.info( DateTime::Microseconds ) << std::endl;

    // nlohmann::json jdt = dt;
    // DateTime dt2 = jdt;

    // std::cout << dt2.info( DateTime::Microseconds ) << std::endl;
    // std::cout << std::endl;


    //  test message post

    auto clog = std::make_shared<ConsoleLogger>();
    // mci.addObserver( clog, "debug voltage>3,a, !  awesometag" );
    mci.addObserver( clog );


    static const nlohmann::json obj = {
        { "pi", 3.141 },
        { "happy", true },
        { "name", "Niels" },
        { "nothing", nullptr },
        { "answer", {
            { "everything", 42 }
        }},
        { "list", { 1, 0, 2 } },
        { "object", {
            { "currency", "USD" },
            { "value", 42.99 }
        }}
    };

    //  different parameters
    mcp( "message only" );
    mcp( "single tag", "awesometag" );
    mcp( "multiple tags", { "imatag", "metoo" } );
    mcp( "array", { "a", 4 } );
    mcp( "object", {{ "a", 4 }} );
    mcp( "object in array", { "a", {{ "b", 4 }} });
    mcp( "array value", { "a", { "b", { "c", 4, true } }});
    mcp( "large object", obj );
    mcp( "emojis", { "⭐⭐⭐", "🚩", "🙈" } );

    //  filtering
    mcp( "debug message", "debug" );
    mcp( "radio message", "radio" );
    mcp( "trick message", "!debug" );   //  likely non-intended behaviour, can't filter for '!debug'
    mcp( "multiple tags", { "radio", { "voltage", 2.41 }, { "debug", 14 } });
    
    // std::cout << std::endl;
        
    //  test parallelism and mutability

    // auto frmt2 = std::make_shared<AsciiFormatter>();
    // auto them2 = std::make_shared<Theme>();
    // auto clog2 = std::make_shared<ConsoleLogger>();
    // frmt2->setTheme( them2 );
    // clog2->setFormatter( frmt2 );
    // mci.addObserver( clog2 );
    // mci.addObserver( clog );

    // nlohmann::json jmut = "mutable string";
    // mcp( jmut, "mutable" );
    // mcp( "cause some delay" );
    
    // jmut = "muted string!";
    
    // mci.removeObserver( clog2 );
    // std::cout << std::endl;

    //  start threaded messages

    std::thread worker( &parallelMessages );

    //  test json to tags_t conversion

    tags_t jmap;
    jmap = filterTags( "debug" );
    jmap = filterTags( { "debug", "radio" } );
    jmap = filterTags( { "debug", {{ "priority", 3.14 }} });
    jmap = filterTags( { "debug", { "priority", 3.14 } });  //  convenience conversion of array<string,json> to tag:value
    jmap = filterTags( { "debug", { 3.14, "priority" } });  //  invalid array<json,string>
    jmap = filterTags( {{ "debug" }} );   //  invalid array<array<>>
    

    //  test filter item 

    FilterItem item;

    ///  spacing
    item.parse( "value>5.4");
    item.parse( "!value>5.4");
    item.parse( " value>5.4");
    item.parse( "! value>5.4");
    item.parse( "value> 5.4");
    item.parse( "value >5.4");

    ///  key
    item.parse( "! amen brother > 5.4");    //  invalid
    item.parse( "! amen-brother > 5.4");
    item.parse( "! amen_brother > 5.4");

    //  values
    item.parse( "value=1" );
    item.parse( "value=1.0" );
    item.parse( "value=1.0f" ); //  string_t
    item.parse( "value=lel" );
    item.parse( "value={1, 2, {3, 4}}" );   //  invalid


    //  test boolean filter

    BooleanFilter filter;

    //  parsing

    filter.set( "debug");
    filter.set( "debug,network");
    filter.set( " debug , network ");
    filter.set( "debug radio, network");
    filter.set( " debug radio , network input     error ");
    filter.set( "priority>2");
    filter.set( "debug priority>2 file=main.cpp, priority<4 network");


    //  test formatter

    auto frmt = std::make_shared<AsciiFormatter>();
    frmt->setIndent( 2 );
    clog->setFormatter( frmt );
    
    mci.addObserver( clog );
    mcp( "info message", {{ "info", "message" }} );
    mcp( "success message", {{ "success", "message" }} );
    mcp( "warning message", {{ "warning", "message" }} );
    mcp( "error message", {{ "error", "message" }} );
    mcp( "debug message", {{ "debug", "message" }} );

    // std::cout << std::endl;


    //  test theme

    auto theme = std::make_shared<DefaultTheme>();
    frmt->setTheme( theme );

    // theme->set( "debug", "#ff98bc"s, "#ffdce8"s );
    // theme->set( "info", "#007aff"s, "#449dff"s );
    // theme->set( "success", "#20b684"s, "#3ddda8"s );
    // theme->set( "warning", "#c87b23"s, "#e09c4f"s );
    // theme->set( "error", "#d51f1a"s, "#e94e4a"s );

    mcp( "info message", {{ "info", "message" }} );
    mcp( "success message", {{ "success", "message" }} );
    mcp( "warning message", {{ "warning", "message" }} );
    mcp( "error message", {{ "error", "message" }} );
    mcp( "debug message", {{ "debug", "message" }} );

    mcp( "simple message" );
    mcp( "uncategorized tags", { "untagged", "nocategory", { "thuglife", "muchsad" } } );


    //  indent

    scopeTest();
    mc::st::simpleEnter();
    mc::st::taggedEnter();
    mc::st::nestedEnter();
    mc::st::deeperNesting();


    // MCS();
    // MCS() << "simple scope";
    // MCS() << "woah" << 4.2 << 3.14159265f << true << 1;
    // MCS() << obj;
    // MCS( "debug" ) << "lala";
    // MCS( "a", { "b", "c", 4, true } ) << "was geht?";
    // MCS( "a", { "b", "c", 4, true } ) << obj;

    // mc::ColorTable::printTestTable();

//    Formatter formatter;
//    formatter.setTagColor( "status", 213, 219 );
//    formatter.setIndent( 2 );

//    HtmlLogger htmlLogger;
//    htmlLogger.setFormatter( &formatter );
//    htmlLogger.createDefaultFile();
//    MC.addObserver( &htmlLogger );

//    ConsoleLogger clog2;
//    clog2.setFormatter( &formatter );
//    MC.addObserver( &clog2 );

//    JsonLogger jlog1;
//    jlog1.createDefaultFile();
//    MC.addObserver( &jlog1 );

//    MC_PS( "welcome to the world of tomorrow!", "intro" );

//    scopeTest();
//    typeTest();
//    negationTest();
//    colorTest();
//    timingTest();

    worker.join();
    // std::this_thread::sleep_for( 1s );

    return EXIT_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
void scopeTest() {
//    MC_ENTER;
   mcp( "i am a fuuunctiioooooonnn *pflätsch*" );
}


//////////////////////////////////////////////////////////////////////////////////
//void typeTest()
//{
//    MC_ENTER;

//    MC_P( "text only" );
//    MC_PS( "single tag", "panic" );
//    MC_PS( "list of tags", { "info", "ibims" } );
//    MC_PM( "rich tags", {{ "penis", "tiny" }, { "number", 3 }, { "simple", {} } });

//    MC_P( "recuuursiooon" );
//    scopeTest();
//}


//////////////////////////////////////////////////////////////////////////////////
//void negationTest() {
//    MC_ENTER;
//    MC_PS( "radio info", { "radio", "info" } );
//    MC_PS( "radio debug", { "radio", "debug" } );
//}


//////////////////////////////////////////////////////////////////////////////////
//void colorTest()
//{
//    MC_ENTER;

//    MC_PM( "info in blue", {{ "info", "ganztoll" }} );
//    MC_PM( "success in green", {{ "success", "wirklichsuper" }} );
//    MC_PM( "warning in yellow", {{ "warning", "undjetz" }} );
//    MC_PM( "error in red", {{ "error", "naklasse" }} );
//}


//////////////////////////////////////////////////////////////////////////////////
//void timingTest()
//{
//    MC_ENTER;

//    MC_PS( "timing test", { "debug", "timing" } );

//    for ( size_t i = 0; i < 100; i++ ) {
//        timingTestRandom();
//    }
//}


//////////////////////////////////////////////////////////////////////////////////
//void timingTestRandom() {
//    MC_ENTER;
//    MC_PS( "timing test", { "debug", "timing" } );
////    QThread::currentThread()->msleep( rand() % 10 );
//}
