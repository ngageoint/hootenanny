
QT += sql \
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
    precompile_header

OTHER_FILES += \
    binding.gyp \
    README.txt

include(../Configure.pri)

QMAKE_CXXFLAGS = -I/usr/include/node -I/usr/local/include/node -Wno-unused-result $$QMAKE_CXXFLAGS

QMAKE_POST_LINK = cp -l ../lib/libHootJs.so.1.0.0 ../lib/HootJs.node 2>/dev/null || cp ../lib/libHootJs.so.1.0.0 ../lib/HootJs.node

LIBS += -L../lib/ -lTgs -ltbs -lHootCore 
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
