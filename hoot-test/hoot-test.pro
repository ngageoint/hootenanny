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
  ../hoot-js/src/main/cpp/ \
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
    -L../lib/ -lHootCore -lHootCoreTest -lHootJs -lTgs -ltbs \

SOURCES += $$files(src/main/cpp/hoot/test/*.cpp, true)
HEADERS += $$files(src/main/cpp/hoot/test/*.h, true)

