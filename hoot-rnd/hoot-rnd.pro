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
    src/test/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatchCreatorTest.cpp \
    src/test/cpp/hoot/rnd/visitors/MatchCandidateCountVisitorRndTest.cpp \
    src/test/cpp/hoot/rnd/conflate/network/IterativeNetworkMatcherTest.cpp \
    src/test/cpp/hoot/rnd/conflate/network/VagabondNetworkMatcherTest.cpp \
    src/test/cpp/hoot/rnd/conflate/network/NetworkMergerTest.cpp \
    src/test/cpp/hoot/rnd/conflate/network/OsmNetworkExtractorTest.cpp \

HEADERS += \

}

SOURCES += \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatchCreator.cpp \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatch.cpp \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMerger.cpp \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMergerCreator.cpp \
    src/main/cpp/hoot/rnd/ops/KeepPoiAndBuildingRefOp.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMatchCreator.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMatch.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMerger.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMergerCreator.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkEdge.cpp \
    src/main/cpp/hoot/rnd/conflate/network/OsmNetworkExtractor.cpp \
    src/main/cpp/hoot/rnd/conflate/network/OsmNetwork.cpp \
    src/main/cpp/hoot/rnd/conflate/network/IterativeNetworkMatcher.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkVertex.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkDetails.cpp \
    src/main/cpp/hoot/rnd/conflate/network/DebugNetworkMapCreator.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkEdgeScore.cpp \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMatcher.cpp \
    src/main/cpp/hoot/rnd/conflate/network/VagabondNetworkMatcher.cpp \
    src/main/cpp/hoot/rnd/conflate/network/IterativeEdgeMatcher.cpp \
    src/main/cpp/hoot/rnd/conflate/network/EdgeMatch.cpp \
    src/main/cpp/hoot/rnd/conflate/network/EdgeString.cpp \
    src/main/cpp/hoot/rnd/conflate/network/EdgeMatchSetFinder.cpp \
    src/main/cpp/hoot/rnd/conflate/network/InvertedEdgeMatchSet.cpp \
    src/main/cpp/hoot/rnd/conflate/network/IndexedEdgeMatchSet.cpp \
    src/main/cpp/hoot/rnd/conflate/network/EdgeLinkCreator.cpp \
    src/main/cpp/hoot/rnd/conflate/network/EdgePair.cpp \
    src/main/cpp/hoot/rnd/conflate/network/IndexedEdgeLinks.cpp \
    src/main/cpp/hoot/rnd/conflate/network/LegacyVertexMatcher.cpp \
    src/test/cpp/hoot/rnd/conflate/network/LegacyVertexMatcherTest.cpp

HEADERS += \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatchCreator.h \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMatch.h \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMerger.h \
    src/main/cpp/hoot/rnd/conflate/poi-polygon/PoiPolygonMergerCreator.h \
    src/main/cpp/hoot/rnd/ops/KeepPoiAndBuildingRefOp.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMatchCreator.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMatch.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMerger.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMergerCreator.h \
    src/main/cpp/hoot/rnd/conflate/network/OsmNetworkExtractor.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkEdge.h \
    src/main/cpp/hoot/rnd/conflate/network/OsmNetwork.h \
    src/main/cpp/hoot/rnd/conflate/network/IterativeNetworkMatcher.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkVertex.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkDetails.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkEdgeScore.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkVertexScore.h \
    src/main/cpp/hoot/rnd/conflate/network/DebugNetworkMapCreator.h \
    src/main/cpp/hoot/rnd/conflate/network/NetworkMatcher.h \
    src/main/cpp/hoot/rnd/conflate/network/VagabondNetworkMatcher.h \
    src/main/cpp/hoot/rnd/conflate/network/IterativeEdgeMatcher.h \
    src/main/cpp/hoot/rnd/conflate/network/EdgeMatch.h \
    src/main/cpp/hoot/rnd/conflate/network/EdgeString.h \
    src/main/cpp/hoot/rnd/conflate/network/EdgeMatchSetFinder.h \
    src/main/cpp/hoot/rnd/conflate/network/MatchScoreProvider.h \
    src/main/cpp/hoot/rnd/conflate/network/EdgeMatchSet.h \
    src/main/cpp/hoot/rnd/conflate/network/InvertedEdgeMatchSet.h \
    src/main/cpp/hoot/rnd/conflate/network/IndexedEdgeMatchSet.h \
    src/main/cpp/hoot/rnd/conflate/network/EdgeLinkCreator.h \
    src/main/cpp/hoot/rnd/conflate/network/EdgePair.h \
    src/main/cpp/hoot/rnd/conflate/network/IndexedEdgeLinks.h \
    src/main/cpp/hoot/rnd/conflate/network/LegacyVertexMatcher.h \
    src/main/cpp/hoot/rnd/conflate/network/SearchRadiusProvider.h \
    src/test/cpp/hoot/rnd/conflate/network/LegacyVertexMatcherTest.h

FORMS +=

OTHER_FILES +=
