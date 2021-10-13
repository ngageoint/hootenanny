
QT += testlib \
    xml \
    concurrent \

TARGET = hoot.bin
TEMPLATE = app
DEPENDPATH += \
    ../tgs/src/main/cpp/ \
    ../tgs/src/main/cpp/tgs/ \
    ../hoot-core/src/main/cpp/hoot/core \
    ../hoot-core/src/main/cpp/ \
    ../hoot-js/src/main/cpp/ \

INCLUDEPATH += \
    $${DEPENDPATH}

CONFIG += rtti \
    debug

include(../Configure.pri)

LIBS += -L../tgs/ -lTgs -L../lib/ -lHootCore -lHootJs -lHootJosm -ltbs -lnode

cppunit {
  LIBS += -lHootCoreTest
}

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../bin/

SOURCES += src/main/cpp/hoot/cmd/main.cpp \
    src/main/cpp/hoot/cmd/HelpCmd.cpp

HEADERS += \

FORMS +=

