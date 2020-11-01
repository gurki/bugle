#include "scopetest.h"

#include <bugle/core/filteritem.h>
#include <bugle/core/message.h>
#include <bugle/core/messagecenter.h>
#include <bugle/core/scope.h>
#include <bugle/core/tags.h>
#include <bugle/format/colortable.h>
#include <bugle/format/formatter.h>
#include <bugle/format/theme.h>
#include <bugle/sinks/consolelogger.h>
#include <bugle/utility/datetime.h>

#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <utility>


void scopeTest();
//void typeTest();
//void negationTest();
//void colorTest();
//void timingTest();
//void timingTestRandom();


using namespace bugle;
using namespace std::chrono_literals;
using namespace std::string_literals;


////////////////////////////////////////////////////////////////////////////////
void parallelMessages()
{
    bglp( "parallel message", "parallel" );

    std::this_thread::sleep_for( 200ms );
    bglp( "later one", { "parallel", "no2" } );

    std::this_thread::sleep_for( 200ms );
    bglp( "another one", { "parallel", "no3" } );
}


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    bugle::ColorTable::printTestTable();

    //  test message post

    auto clog = std::make_shared<ConsoleLogger>();
    // bgli.addObserver( clog, "debug voltage>3,a, !  awesometag" );
    bgli.addObserver( clog );


    static const nlohmann::json obj = {
        { "pi", 3.141 },
        { "happy", true },
        { "nothing", nullptr },
        { "list", { 1, 0, 2 } },
        { "object", {
            { "name", "Niels" },
            { "value", 42.99 },
            { "currency", "USD" },
            { "answer", {
                { "everything", 42 }
            }}
        }}
    };

    //  different parameters
    bglp( "message only" );
    bglp( "single tag", "awesometag" );
    bglp( "multiple tags", { "imatag", "metoo" } );
    bglp( "array", { "a", 4 } );
    bglp( "object", {{ "a", 4 }} );
    bglp( "object in array", { "a", {{ "b", 4 }} });
    bglp( "array value", { "a", { "b", { "c", 4, true } }});
    bglp( "large object", obj );
    bglp( "emojis", { "⭐⭐⭐", "🚩", "🙈" } );

    //  filtering
    bglp( "debug message", "debug" );
    bglp( "radio message", "radio" );
    bglp( "trick message", "!debug" );   //  likely non-intended behaviour, can't filter for '!debug'
    bglp( "multiple tags", { "radio", { "voltage", 2.41 }, { "debug", 14 } });

    //  start threaded messages

    std::thread worker( &parallelMessages );

    //  test formatter

    auto frmt = std::make_shared<AsciiFormatter>();
    frmt->setIndent( 2 );
    clog->setFormatter( frmt );

    bglp( "info message", {{ "info", "message" }} );
    bglp( "success message", {{ "success", "message" }} );
    bglp( "warning message", {{ "warning", "message" }} );
    bglp( "error message", {{ "error", "message" }} );
    bglp( "debug message", {{ "debug", "message" }} );

    // std::cout << std::endl;


    //  test theme

    std::this_thread::sleep_for( 100ms );

    auto theme = std::make_shared<DefaultTheme>();
    frmt->setTheme( theme );

    theme->set( "debug", "#ff98bc"s, "#ffdce8"s );
    theme->set( "info", "#007aff"s, "#449dff"s );
    theme->set( "success", "#20b684"s, "#3ddda8"s );
    theme->set( "warning", "#c87b23"s, "#e09c4f"s );
    theme->set( "error", "#d51f1a"s, "#e94e4a"s );

    bglp( "info message", {{ "info", "message" }} );
    bglp( "success message", {{ "success", "message" }} );
    bglp( "warning message", {{ "warning", "message" }} );
    bglp( "error message", {{ "error", "message" }} );
    bglp( "debug message", {{ "debug", "message" }} );

    bglp( "uncategorized tags", { "untagged", "nocategory", { "thuglife", "muchsad" } } );
    bglp( "simple message" );


    //  indent

    scopeTest();
    bugle::st::simpleEnter();
    bugle::st::taggedEnter();
    bugle::st::nestedEnter();
    bugle::st::deeperNesting();


    // bgls();
    // bgls() << "simple scope";
    // bgls() << "woah" << 4.2 << 3.14159265f << true << 1;
    // bgls() << obj;
    // bgls( "debug" ) << "lala";
    // bgls( "a", { "b", "c", 4, true } ) << "was geht?";
    // bgls( "a", { "b", "c", 4, true } ) << obj;

//    Formatter formatter;
//    formatter.setTagColor( "status", 213, 219 );
//    formatter.setIndent( 2 );

//    HtmlLogger htmlLogger;
//    htmlLogger.setFormatter( &formatter );
//    htmlLogger.createDefaultFile();
//    BUGLE_INSTANCE.addObserver( &htmlLogger );

//    ConsoleLogger clog2;
//    clog2.setFormatter( &formatter );
//    BUGLE_INSTANCE.addObserver( &clog2 );

//    JsonLogger jlog1;
//    jlog1.createDefaultFile();
//    BUGLE_INSTANCE.addObserver( &jlog1 );

//    MC_PS( "welcome to the world of tomorrow!", "intro" );

//    scopeTest();
//    typeTest();
//    negationTest();
//    colorTest();
//    timingTest();

    worker.join();
    std::this_thread::sleep_for( 100ms );

    return EXIT_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
void scopeTest() {
    bgls();
    bglp( "i am a fuuunctiioooooonnn *pflätsch*" );
}


//////////////////////////////////////////////////////////////////////////////////
void unitTests()
{
    //  color

    Color cols[] = {
        "#abc"s,
        "#aabbcc"s,
        "unknown"s,
        "#1a2b3c"s,
        17
    };

    for ( const auto& col : cols ) {
        std::cout << col.name() << std::endl;
    }

    std::cout << std::endl;


    //  datetime

    auto dt = DateTime::now();
    std::cout << dt.info( DateTime::Microseconds ) << std::endl;

    nlohmann::json jdt = dt;
    DateTime dt2 = jdt;

    std::cout << dt2.info( DateTime::Microseconds ) << std::endl;
    std::cout << std::endl;


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
