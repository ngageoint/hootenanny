
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

SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true)

OTHER_FILES = \
    $$files(../test-files/cmd/*.sh, true) \
    ../test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off \
    ../test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off \
    $$files(../scripts/core/*.sh, true) \
    $$files(../scripts/valgrind/*, true) \
