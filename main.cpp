#include "messagecenter.h"
#include "colortable.h"
#include "consolelogger.h"
#include "jsonlogger.h"
#include "messagelist.h"
#include "messageformatter.h"
#include "messagelistproxy.h"
#include "htmllogger.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QColor>
#include <QQuickStyle>


void scopeTest();
void typeTest();
void negationTest();
void colorTest();
void timingTest();
void timingTestRandom();


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
    QCoreApplication::setApplicationName( "Message Center" );
    QCoreApplication::setApplicationVersion( "1.0.0" );
    QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
    QGuiApplication app( argc, argv );

    MC.setTrace( false );

    QThread* mcThread = new QThread();
    MC.moveToThread( mcThread );
    mcThread->start();

    MessageFormatter formatter;
    formatter.setTagColor( "status", 213, 219 );
    formatter.setIndent( 2 );

    MessageList messageList;
    messageList.setFormatter( &formatter );
    MC.addObserver( &messageList, "!debug" );

    MessageListProxy messageListProxy;
    messageListProxy.setSourceModel( &messageList );

    QObject::connect(
        &messageList, &MessageList::hasBeenNotified,
        &messageListProxy, &MessageListProxy::invalidateFilter
    );

    HtmlLogger htmlLogger;
    htmlLogger.setFormatter( &formatter );
    htmlLogger.createDefaultFile();
    MC.addObserver( &htmlLogger );

    QThread* htmlThread = new QThread();
    htmlLogger.moveToThread( htmlThread );
    htmlThread->start();

    ConsoleLogger clog2;
    clog2.setFormatter( &formatter );
    MC.addObserver( &clog2 );

    QThread* consoleThread = new QThread();
    clog2.moveToThread( consoleThread );
    consoleThread->start();

    JsonLogger jlog1;
    jlog1.createDefaultFile();
    MC.addObserver( &jlog1 );

    MC_PS( "welcome to the world of tomorrow!", "intro" );

    scopeTest();
    typeTest();
    negationTest();
    colorTest();
//    timingTest();

    QQmlApplicationEngine engine;

    auto context = engine.rootContext();
    context->setContextProperty( "messageCenter", &MC );
    context->setContextProperty( "messageList", &messageList );
    context->setContextProperty( "messageListProxy", &messageListProxy );
    engine.load( QUrl( QLatin1String( "qrc:/main.qml" ) ));

    return app.exec();
}


////////////////////////////////////////////////////////////////////////////////
void scopeTest() {
    MC_ENTER;
    MC_P( "i am a fuuunctiioooooonnn *pflätsch*" );
}


////////////////////////////////////////////////////////////////////////////////
void typeTest()
{
    MC_ENTER;

    MC_P( "text only" );
    MC_PS( "single tag", "panic" );
    MC_PS( "list of tags", { "info", "ibims" } );
    MC_PM( "rich tags", {{ "penis", "tiny" }, { "number", 3 }, { "simple", {} } });

    MC_P( "recuuursiooon" );
    scopeTest();
}


////////////////////////////////////////////////////////////////////////////////
void negationTest() {
    MC_ENTER;
    MC_PS( "radio info", { "radio", "info" } );
    MC_PS( "radio debug", { "radio", "debug" } );
}


////////////////////////////////////////////////////////////////////////////////
void colorTest()
{
    MC_ENTER;

    MC_PM( "info in blue", {{ "info", "ganztoll" }} );
    MC_PM( "success in green", {{ "success", "wirklichsuper" }} );
    MC_PM( "warning in yellow", {{ "warning", "undjetz" }} );
    MC_PM( "error in red", {{ "error", "naklasse" }} );
}


////////////////////////////////////////////////////////////////////////////////
void timingTest()
{
    MC_ENTER;

    MC_PS( "timing test", { "debug", "timing" } );

    for ( size_t i = 0; i < 100; i++ ) {
        timingTestRandom();
    }
}


////////////////////////////////////////////////////////////////////////////////
void timingTestRandom() {
    MC_ENTER;
    MC_PS( "timing test", { "debug", "timing" } );
    QThread::currentThread()->msleep( rand() % 10 );
}
