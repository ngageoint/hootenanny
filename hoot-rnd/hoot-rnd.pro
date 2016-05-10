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

TARGET = HootRnd
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

CONFIG += rtti \
    qtestlib \
    debug

include(../Configure.pri)

QMAKE_CXXFLAGS = -I/usr/include/nodejs $$QMAKE_CXXFLAGS

LIBS += -L../lib/ -lTgs -ltbs -lHootCore

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

cppunit {
SOURCES += \
    src/test/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatchTest.cpp \
    src/test/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMergerTest.cpp \
    src/test/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMergerCreatorTest.cpp \
    src/test/cpp/hoot/rnd/visitors/MatchCandidateCountVisitorRndTest.cpp \
    src/test/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatchCreatorTest.cpp \

HEADERS += \

}

SOURCES += \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatchCreator.cpp \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatch.cpp \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMerger.cpp \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMergerCreator.cpp \
    src/main/cpp/hoot/rnd/ops/KeepPoiAndBuildingRefOp.cpp \

HEADERS += \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatchCreator.h \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatch.h \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMerger.h \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMergerCreator.h \
    src/main/cpp/hoot/rnd/ops/KeepPoiAndBuildingRefOp.h

FORMS +=

OTHER_FILES +=
