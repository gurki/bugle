#include "messagecenter/messagecenter.h"
#include "messagecenter/consolelogger.h"
#include "messagecenter/messagescope.h"
#include "messagecenter/colortable.h"
#include "messagecenter/formatter.h"
//#include "consolelogger.h"
//#include "jsonlogger.h"
//#include "formatter.h"
//#include "htmllogger.h"

#include "messagecenter/filteritem.h"
#include "messagecenter/datetime.h"

#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <variant>
#include <utility>

//void scopeTest();
//void typeTest();
//void negationTest();
//void colorTest();
//void timingTest();
//void timingTestRandom();

using namespace mc;


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    auto frmt = std::make_shared<Formatter>();
    auto clog = std::make_shared<ConsoleLogger>();
    clog->setFormatter( frmt );

    mci.addObserver( clog, "!discard" );

    nlohmann::json obj = {
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

    mc_post( "message only" );
    mc_post( "single tag", "awesometag" );
    mc_post( "multiple tags", { "imatag", "metoo" } );
    mc_post( "array", { "a", 4 } );
    mc_post( "object", {{ "a", 4 }} );
    mc_post( "object in array", { "a", {{ "b", 4 }} });
    mc_post( "array value", { "a", { "b", { "c", 4, true } }});
    mc_post( "large object", obj );

    mc_post( "discarded message", "discard" );


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

    //  execution

    // filter.set( "debug" );
    // const bool suc = filter.passes( { "radio", "debug", { "priority", 3.14 } } );
    // std::cout << suc << std::endl;

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

    return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////////
//void scopeTest() {
//    MC_ENTER;
//    MC_P( "i am a fuuunctiioooooonnn *pflätsch*" );
//}


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
