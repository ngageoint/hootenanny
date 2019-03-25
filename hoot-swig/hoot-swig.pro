# -------------------------------------------------
# Code for the SWIG interface to hoot
# -------------------------------------------------

exists ("../../Configure.pri") {
    include("../../Configure.pri")
}
else {
    include("../Configure.pri")
}

TARGET = HootSwig
TEMPLATE = lib

DEPENDPATH += \
    ../tbs/src/main/cpp/ \
    ../tgs/src/main/cpp/ \
    ../hoot-core/src/main/cpp/ \
    ../hoot-core-test/src/test/cpp/ \
    ../hoot-rnd/src/main/cpp/ \
    src/main/cpp/ \

INCLUDEPATH += \
  $${DEPENDPATH} \
  /usr/include/nodejs \
  ../local/include/ \

PRECOMPILED_HEADER = src/main/cpp/hoot/swig/HootSwigStable.h

CONFIG += rtti \
    testlib \
    precompile_header

OTHER_FILES += README.txt \
    src/main/cpp/hoot/swig/util/GeosDataConvert.i \
    src/main/cpp/hoot/swig/TestUtils.i

include(../Configure.pri)

QMAKE_CXXFLAGS = $$QMAKE_CXXFLAGS

LIBS += -L../lib/ -lTgs -ltbs -lHootCore -lHootRnd -lHootJs -lv8

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

# This is needed to make the swig code happy.
QMAKE_CXXFLAGS += -fno-strict-aliasing

SWIG_FILES = $$files(src/*.i, true)

swig.files=src/main/cpp/hoot/swig
swig.commands=swig -c++ -java -package hoot -DGEOS_DLL= $$find(QMAKE_CXXFLAGS, -I.*) -I../hoot-core/src/main/cpp/ -I../hoot-rnd/src/main/cpp/ -outdir src/main/java-swig/hoot/ -o tmp/swig/HootSwigJni.cxx src/main/cpp/hoot/swig/swig.i
swig.input=src/main/cpp/hoot/swig/swig.i
swig.variable_out=dum
swig.output=tmp/swig/HootSwigJni.cxx
swig.name=swig

message($$QMAKE_CXXFLAGS)

PRE_TARGETDEPS=swig

QMAKE_EXTRA_TARGETS += swig
QMAKE_CLEAN += $${swig.output}

SOURCES += $$files(src/*.cpp, true)
# Swig generated files.
SOURCES += tmp/swig/HootSwigJni.cxx
HEADERS += $$files(src/*.h, true)

# test files
SOURCES -= $$files(src/*Test*.cpp, true)
SOURCES -= $$files(src/*Test*.h, true)

cppunit {
SOURCES += $$files(src/*Test*.cpp, true)
SOURCES += $$files(src/*Test*.h, true)
}

OTHER_FILES += $$files(src/*.i, true)
OTHER_FILES += $$files(tmp/swig/*, true)
OTHER_FILES += $$files(src/*.java, true)
OTHER_FILES += pom.xml
