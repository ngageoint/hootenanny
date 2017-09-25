# -------------------------------------------------
# The Hoot R&D project is a place to store experimental code that isn't quite ready for prime time.
# At this point anthing below TRL5 [1] should be put in this project. This project is disabled by
# default which should help avoid introducing bugs into the core while experimenting.
#
# 1. TRL5 - Module and/or subsystem validation in a relevant environment
# -------------------------------------------------

exists ("../../Configure.pri") {
    include("../../Configure.pri")
}
else {
    include("../Configure.pri")
}

QT += script \
    sql \
    testlib \
    xml \

TARGET = HootSwig
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

PRECOMPILED_HEADER = src/main/cpp/hoot/swig/HootSwigStable.h

CONFIG += rtti \
    qtestlib \
    precompile_header

OTHER_FILES += README.txt

include(../Configure.pri)

QMAKE_CXXFLAGS = $$QMAKE_CXXFLAGS

LIBS += -L../lib/ -lTgs -ltbs -lHootCore -lv8

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

# This is needed to make the swig code happy.
QMAKE_CXXFLAGS += -fno-strict-aliasing

SWIG_FILES = src/main/cpp/hoot/swig/swig.i

#swig.name=swig
##swig.files=src/main/cpp/hoot/swig
#swig.depends=$$files(src/*.i, true)
#swig.commands=swig -c++ -java -package hoot \
#  -I../hoot-core/src/main/cpp/ \
#  -outdir src/main/java-swig/hoot/ \
#  -o tmp/swig/HootSwigJni.cxx src/main/cpp/hoot/swig/swig.i
#swig.input=SWIG_FILES
#swig.variable_out=dum
#swig.output=tmp/swig/HootSwigJni.cxx
#swig.target=HootSwig_wrap.cxx

SWIG_FILES = src/main/cpp/hoot/swig/swig.i

swig.files=src/main/cpp/hoot/swig
swig.commands=swig -c++ -java -package hoot -I../hoot-core/src/main/cpp/ -outdir src/main/java/pp/ -o tmp/swig/HootSwigJni.cxx src/main/cpp/hoot/swig/swig.i
swig.input=SWIG_FILES
swig.variable_out=dum
swig.output=tmp/swig/HootSwigJni.cxx
swig.name=swig

QMAKE_EXTRA_COMPILERS += swig
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
