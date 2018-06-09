DEFINES += \
    ISS_MODULE_MESSAGE_CENTER

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/include/MessageCenter

SOURCES += \
    $$PWD/src/booleanfilter.cpp \
    $$PWD/src/colortable.cpp \
    $$PWD/src/filteritem.cpp \
    $$PWD/src/message.cpp \
    $$PWD/src/messagecenter.cpp \
    $$PWD/src/messageformatter.cpp \
    $$PWD/src/messagelist.cpp \
    $$PWD/src/messagelistproxy.cpp \
    $$PWD/src/messagescope.cpp \
    $$PWD/src/messageutility.cpp \
    $$PWD/src/systeminfo.cpp

HEADERS += \
    $$PWD/include/MessageCenter/booleanfilter.h \
    $$PWD/include/MessageCenter/colortable.h \
    $$PWD/include/MessageCenter/dependencies.h \
    $$PWD/include/MessageCenter/filteritem.h \
    $$PWD/include/MessageCenter/message.h \
    $$PWD/include/MessageCenter/messagecenter.h \
    $$PWD/include/MessageCenter/messageformatter.h \
    $$PWD/include/MessageCenter/messageinfo.h \
    $$PWD/include/MessageCenter/messagelist.h \
    $$PWD/include/MessageCenter/messagelistproxy.h \
    $$PWD/include/MessageCenter/messageobserver.h \
    $$PWD/include/MessageCenter/messagescope.h \
    $$PWD/include/MessageCenter/messageutility.h \
    $$PWD/include/MessageCenter/systeminfo.h \
    $$PWD/include/MessageCenter/dependencies.h

RESOURCES += \
    $$PWD/data.qrc

#  versioning

ISS_MESSAGE_CENTER_GIT_VERSION = \
    $$system( git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags )

isEmpty( GIT_VERSION ) {
  ISS_MESSAGE_CENTER_GIT_VERSION = n/a
}

DEFINES += ISS_MESSAGE_CENTER_VERSION=\\\"$$ISS_MESSAGE_CENTER_GIT_VERSION\\\"
