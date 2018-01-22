# -------------------------------------------------
# The Hoot R&D project is a place to store experimental code that isn't quite ready for prime time.
# At this point anthing below TRL5 [1] should be put in this project. This project is disabled by
# default which should help avoid introducing bugs into the core while experimenting.
#
# 1. TRL5 - Module and/or subsystem validation in a relevant environment
# -------------------------------------------------
QT += script \
    sql \
    testlib \
    xml \

TARGET = HootJs
TEMPLATE = lib

DEPENDPATH += \
    ../tbs/src/main/cpp/ \
    ../tgs/src/main/cpp/ \
    ../hoot-core/src/main/cpp/ \
    ../hoot-core-test/src/test/cpp/ \
    src/main/cpp/ \

INCLUDEPATH += \
  $${DEPENDPATH} \
  /usr/include/node \
  ../local/include/ \

PRECOMPILED_HEADER = src/main/cpp/hoot/js/HootJsStable.h

CONFIG += rtti \
    qtestlib \
    precompile_header

OTHER_FILES += \
    binding.gyp \
    README.txt

include(../Configure.pri)

QMAKE_CXXFLAGS = -I/usr/include/node $$QMAKE_CXXFLAGS

QMAKE_POST_LINK = cp -l ../lib/libHootJs.so.1.0.0 ../lib/HootJs.node 2>/dev/null || cp ../lib/libHootJs.so.1.0.0 ../lib/HootJs.node

LIBS += -L../lib/ -lTgs -ltbs -lHootCore -lnode
LIBS -= -lhdfs

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

SOURCES += $$files(src/main/*.cpp, true)
HEADERS += $$files(src/main/*.h, true)

# test files
cppunit {
SOURCES += $$files(src/test/*.cpp, true)
SOURCES += $$files(src/test/*.h, true)

LIBS += -lHootCoreTest
}
