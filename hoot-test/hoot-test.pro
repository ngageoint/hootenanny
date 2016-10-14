# -------------------------------------------------
# Project created by QtCreator 2011-09-29T12:27:47
# -------------------------------------------------
include(../Configure.pri)

TEMPLATE = app

CONFIG += rtti \
    debug

TARGET = HootTest.bin

DEPENDPATH += \
  ../tgs/src/main/cpp/ \
  ../tgs/src/main/cpp/tgs/ \
  ../hoot-core/src/main/cpp/ \
  ../hoot-core-test/src/test/cpp/ \
  ../hoot-hadoop/src/main/cpp/ \
  ../hoot-test/src/main/cpp/hoot/test \
  ../pretty-pipes/pp-lib/src/main/cpp/ \
  ../pretty-pipes/example/src/main/cpp/ \

INCLUDEPATH += \
  $${DEPENDPATH}

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../bin/

LIBS += \
    -L../lib/ -lHootCore -lHootCoreTest -lTgs -ltbs \

HEADERS += \
    src/main/cpp/hoot/test/ScriptTestSuite.h \
    src/main/cpp/hoot/test/ScriptTest.h \

SOURCES += src/main/cpp/hoot/test/main.cpp \
    src/main/cpp/hoot/test/ScriptTestSuite.cpp \
    src/main/cpp/hoot/test/ScriptTest.cpp \


