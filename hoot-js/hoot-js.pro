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
  /usr/include/nodejs \
  ../local/include/ \

PRECOMPILED_HEADER = src/main/cpp/hoot/js/HootJsStable.h

CONFIG += rtti \
    qtestlib \
    precompile_header

OTHER_FILES += \
    binding.gyp \
    README.txt

include(../Configure.pri)

QMAKE_CXXFLAGS = -I/usr/include/nodejs $$QMAKE_CXXFLAGS

QMAKE_POST_LINK = cp -l ../lib/libHootJs.so.1.0.0 ../lib/HootJs.node 2>/dev/null || cp ../lib/libHootJs.so.1.0.0 ../lib/HootJs.node

LIBS += -L../lib/ -lTgs -ltbs -lHootCore -lnode

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true)

# test files
SOURCES -= $$files(src/*Test*.cpp, true)
SOURCES -= $$files(src/*Test*.h, true)
cppunit {
SOURCES += $$files(src/*Test*.cpp, true)
SOURCES += $$files(src/*Test*.h, true)
}
