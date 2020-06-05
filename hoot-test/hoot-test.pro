# -------------------------------------------------
# Exposes the HootTest command line interface
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
  ../hoot-js/src/main/cpp/ \
  ../hoot-test/src/main/cpp \

INCLUDEPATH += \
  $${DEPENDPATH}

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../bin/

LIBS += \
    -L../lib/ -lHootCore -lHootCoreTest -lHootJs -lTgs -ltbs -lnode \

HEADERS += \
    src/main/cpp/hoot/test/ScriptTestSuite.h \
    src/main/cpp/hoot/test/ScriptTest.h \
    src/main/cpp/hoot/test/ProcessPool.h

SOURCES += src/main/cpp/hoot/test/main.cpp \
    src/main/cpp/hoot/test/ScriptTestSuite.cpp \
    src/main/cpp/hoot/test/ScriptTest.cpp \
    src/main/cpp/hoot/test/ProcessPool.cpp

OTHER_FILES = \
    $$files(../test-files/cmd/*.sh, true) \
    $$files(../scripts/core/*.sh, true) \
    $$files(../scripts/valgrind/*.sh, true) \
