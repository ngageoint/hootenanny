
QT += sql \
    testlib \
    xml \
    network \

TARGET = HootCoreTest
TEMPLATE = lib

DEPENDPATH += \
    ../tbs/src/main/cpp/ \
    ../tgs/src/main/cpp/ \
    ../hoot-core/src/main/cpp/ \
    src/test/cpp/ \

INCLUDEPATH += \
  $${DEPENDPATH} \
  ../local/include/ \

CONFIG += rtti \
    debug \
    precompile_header

PRECOMPILED_HEADER = ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h

OTHER_FILES = \
    $$files(../rules/*.js, true) \
    ../conf/core/ConfigOptions.asciidoc \

include(../Configure.pri)

LIBS += -L../lib/ -lTgs -ltbs

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

cppunit {

# test files (all files in this project are test files)
SOURCES += $$files(src/*Test*.cpp, true)
HEADERS += $$files(src/*Test*.h, true)

# services test files
SOURCES -= $$files(src/Service*.cpp, true)
services:SOURCES += $$files(src/Service*.cpp, true)

# nodejs test files
SOURCES -= src/test/cpp/hoot/core/io/OgrReaderTest.cpp
SOURCES -= src/test/cpp/hoot/core/io/OgrWriterTest.cpp
SOURCES -= src/test/cpp/hoot/core/ops/CalculateStatsOpTest.cpp
SOURCES -= src/test/cpp/hoot/core/schema/TranslatedTagDifferencerTest.cpp
nodejs:SOURCES += src/test/cpp/hoot/core/io/OgrReaderTest.cpp
nodejs:SOURCES += src/test/cpp/hoot/core/io/OgrWriterTest.cpp
nodejs:SOURCES += src/test/cpp/hoot/core/ops/CalculateStatsOpTest.cpp
nodejs:SOURCES += src/test/cpp/hoot/core/schema/TranslatedTagDifferencerTest.cpp

}

SOURCES += \

HEADERS += \
