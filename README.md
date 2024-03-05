# bugle 📯
A tag-based, filterable, colorful and thread-safe cpp logging library

```
This document is out of date and based on old Qt/Qml version.
Will rewrite soon™.
```


## Content

- [Content](#content)
- [Motivation](#motivation)
- [Design Goals](#design-goals)
- [Integration](#integration)
- [Examples](#examples)
- [Discussion](#discussion)


## Motivation
Why another logging framework? Well, I looked around, and I found none whatsoever that provides the capability and ease of use I imagine. So I set out and built my own.

Enjoy and let me know what you think!
\- Gurki


## Design Goals

- **Easy integration.** The MC is designed to be usable out of the box with no hassle for set-up and usage in production.
- **Tags.** They make organizing messages and setting up different channels easier than ever.
- **Colors.** Colorized output makes reading and understanding console messages easier, which gives you the ability to spot relevant messages faster.
- **Customizable.** Create your own message observers by simply implementing the slick interface and whatever you want with your messages.
- **Threaded.** All messages are sent via signals / slots. Eventually, these should be completely thread safe. Not just there yet ;).


## Integration

Include ```messagecenter.pri``` in your project file and add ```<bugle/messagecenter.h``` to the files where you want to use it.

To register a custom message observer, instantiate it from the ```Observer``` interface and add it to the MC.

```cpp
//  consolelogger.h
class ConsoleLogger : public QObject, public Observer
{
    Q_OBJECT

    public:

        ConsoleLogger( QObject* parent = nullptr );

    public slots:

        virtual void notify( const Message& message );
};

//  main.cpp
ConsoleLogger radioConsoleLogger;
MC.addObserver( &radioConsoleLogger, { "radio", "!debug" } );
MC_POST( "new radio module connected", { "radio", "info" } );
```


## Examples

#### Messages

The MC provides a macro to automatically store the file name, function name and line number in the emitted message object.

```cpp
MC.post( "without call stack information" );
MC_POST( "this message contains file, function and line of this very call" );
```

If a message is indexed or not can be seen via ```Message::isIndexed()``` and a convenient string with the call stack information retrieved using ```Message::info()```.

#### Tags

Messages can be sent (aka posted) using four different arguments.
```cpp
MC_POST( "text only" );
MC_POST( "single tag", "panic" );
MC_POST( "list of tags", { "misanmia", "ibims" } );
MC_POST( "rich tags", {{ "priority", 3 }, { "info", {} }} );
```

Take a look at the ```ConsoleLogger``` and ```JsonLogger``` classes for example implementations.

#### Observers

Observers that comply with the ```Observer``` interface can be registered to the MC.
They can be hooked to a specific set of tags, which can even be boolean combinations of multiple sets and negated tags.
```cpp
ConsoleLogger log1, log2, log3;

//  log1 will output every message
MC.addObserver( &log1 );
//  log2 outputs all messages that are tagged radio but not debug (message can have more tags)
MC.addObserver( &log2, { "radio", "!debug" } );
//  log3 listens to exactly the network messages with priority = 2.
MC.addObserver( &log3, {{ "priority", 2 }, { "network", {} }} );
//  log3 now _also_ listens to debug messages
MC.addObserver( &log3, { "debug" } )
```

#### Colors

The MC provides color capability for supported consoles. Utility functions for color conversion are available through ```messagecolors.h```. A set of pre-defined colors is automatically registered with each MC instance. These highlight ```info```, ```success```, ```warning``` and ```error``` tags in blue, green, yellow and red, respectively.

Automatic tag colors can be set using ```MessageCenter::setColor(...)``` and cleared with ```MessageCenter::clearColor(...)```.

**Note:** Color functionality will be moved to a separate class at some point.

#### Filters

Filters are a way to restrict which messages are emitted in the first place.
For example, one might want disable all but some messages. Similar to observers, multiple sets of tags can be set to filter message by tags before notifying observers.

```cpp
//  only notifies of debug messages
MC.addFilter( { "debug" } );
//  now only notifies of debug message with priority=2, that do not come from the radio module
MC.addFilter( {{ "priority", 2 }, { "!radio", {} }} );
```

**Note:** Not implemented yet.

#### QML

The MC is fully compatible with QML and can be registered in the following way.

```cpp
//  main.cpp
QQmlApplicationEngine engine;
auto context = engine.rootContext();
context->setContextProperty( "messageCenter", &MC );
engine.load( QUrl( QLatin1String( "qrc:/main.qml" ) ));

//  main.qml
messageCenter.post( "text only" )
messageCenter.post( "single tags", "info" )
messageCenter.post( "multiple tags", [ "qml", "fuckyes" ] )
messageCenter.post( "rich tags", { priority:2, detail:"badass" } )
```

However, message indexing is not available from QML.


## Discussion

Logging frameworks should of course be designed to be efficient and light-weight. The MC tries to do that, but also aims to provide a very flexible and powerful logging mechanism. As such, there is a tradeoff to be made.

For data intensive logging tasks, consider writing a custom system independent of the MC.
