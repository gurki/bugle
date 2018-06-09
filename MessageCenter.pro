TEMPLATE = app

CONFIG += c++14

CONFIG -= \
    app_bundle \
    qt

SOURCES += \
    main.cpp

include( $$PWD/messagecenter.pri )
include( $$PWD/messagecenterexamples.pri )
include( $$PWD/dependencies/json.pri )
