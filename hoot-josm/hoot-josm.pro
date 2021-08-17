# -------------------------------------------------
# Any code that makes calls to JOSM via JNI should go into this project. JOSM integration is the only functionality that currently uses JOSM.
# If that changes at any point, then JNI classes may need to be moved to their own project.
# -------------------------------------------------

QT += testlib \
    xml \

TARGET = HootJosm
QMAKE_POST_LINK += ../scripts/core/make-hoot-josm-java.sh
QMAKE_CLEAN += target/hoot-josm.jar
TEMPLATE = lib

DEPENDPATH += \
    ../tbs/src/main/cpp/ \
    ../tgs/src/main/cpp/ \
    ../hoot-core/src/main/cpp/ \
    ../hoot-core-test/src/test/cpp/ \
    src/main/cpp/ \
    src/test/cpp/ \

INCLUDEPATH += \
  $${DEPENDPATH} \
  /usr/include/nodejs \
  ../local/include/ \

PRECOMPILED_HEADER = ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h

CONFIG += rtti \
    precompile_header

include(../Configure.pri)

QMAKE_CXXFLAGS = -I/usr/include/nodejs $$QMAKE_CXXFLAGS

LIBS += -L../lib/ -lTgs -ltbs -lHootCore -lHootCoreTest
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
HEADERS += $$files(src/test/*.h, true)
}
