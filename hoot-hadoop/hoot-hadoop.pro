CONFIG += debug

include(../Configure.pri)

QT += testlib \
    xml \

TEMPLATE = lib

PRECOMPILED_HEADER = ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h

CONFIG += rtti \
    testlib \
    precompile_header

TARGET = HootHadoop

DEPENDPATH += \
  ../hoot-core/src/main/cpp \
  ../hoot-core/src/main/cpp/hoot/core/ \
  ../hoot-core-test/src/test/cpp/ \
  ../tgs/src/main/cpp \
  ../tgs/src/main/cpp/tgs \
  src/main/cpp/ \
  ../pretty-pipes/pp-lib/src/main/cpp \

INCLUDEPATH += \
  $${DEPENDPATH} \

UI_DIR = tmp/ui
MOC_DIR = tmp/moctest
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

LIBS += -L../lib/ -lHootCore \
  -L../pretty-pipes/lib/ -lPrettyPipes \

# This avoids a conflict with an "emit" function defined in Hadoop Pipes.
DEFINES += QT_NO_EMIT

SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true)

# test files
SOURCES -= $$files(src/*Test*.cpp, true)
SOURCES -= $$files(src/*Test*.h, true)
cppunit {
SOURCES += $$files(src/*Test*.cpp, true)
SOURCES += $$files(src/*Test*.h, true)

# services test files
SOURCES -= $$files(src/Service*Test.cpp, true)
services:SOURCES += $$files(src/Service*Test.cpp, true)

}
