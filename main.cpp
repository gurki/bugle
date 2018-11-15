#include "messagecenter/messagecenter.h"
#include "messagecenter/consolelogger.h"
#include "messagecenter/messagescope.h"
#include "messagecenter/colortable.h"
#include "messagecenter/messageformatter.h"
//#include "consolelogger.h"
//#include "jsonlogger.h"
//#include "messageformatter.h"
//#include "htmllogger.h"

#include <iostream>

//void scopeTest();
//void typeTest();
//void negationTest();
//void colorTest();
//void timingTest();
//void timingTestRandom();


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    // auto frmt = std::make_shared<mc::MessageFormatter>();
    // auto clog = std::make_shared<mc::ConsoleLogger>();
    // clog->setFormatter( frmt );

    // MC.addObserver( clog, "" );

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

    // MC_POST( "message only" );
    // MC_PROST( "single tag", "awesome tag" );
    // MC_PROST( "was geht?", { "a", { "b", "c", 4, true } } );
    // MCS();
    // MCS() << "simple scope";
    // MCS() << "woah" << 4.2 << 3.14159265f << true << 1;
    // MCS() << obj;
    // MCS( "debug" ) << "lala";
    // MCS( "a", { "b", "c", 4, true } ) << "was geht?";
    // MCS( "a", { "b", "c", 4, true } ) << obj;

    mc::ColorTable::printTestTable();

//    MessageFormatter formatter;
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
