QT += qml quick quickcontrols2
CONFIG += c++14

SOURCES += \
    main.cpp

RESOURCES += \
    qml.qrc

include( $$PWD/messagecenter.pri )
include( $$PWD/messagecenterexamples.pri )
include( $$PWD/dependencies/json/json.pri )



