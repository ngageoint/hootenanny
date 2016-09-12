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

CONFIG += rtti \
    qtestlib \
    debug

include(../Configure.pri)

QMAKE_CXXFLAGS = -I/usr/include/nodejs $$QMAKE_CXXFLAGS

QMAKE_POST_LINK = cp -l ../lib/libHootJs.so.1.0.0 ../lib/HootJs.node 2>/dev/null || cp ../lib/libHootJs.so.1.0.0 ../lib/HootJs.node

LIBS += -L../lib/ -lTgs -ltbs -lHootCore -lv8

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

cppunit {
SOURCES += \
    src/test/cpp/hoot/js/util/DataConvertJsTest.cpp \
    src/test/cpp/hoot/js/PluginContextTest.cpp \
    src/test/cpp/hoot/js/util/UuidHelperJsTest.cpp \
    src/test/cpp/hoot/js/JavaScriptTranslatorTest.cpp \
    src/test/cpp/hoot/js/conflate/js/ScriptMatchTest.cpp \
    src/test/cpp/hoot/js/conflate/js/ScriptMatchCreatorTest.cpp

HEADERS += \

}

SOURCES += \
    src/main/cpp/hoot/js/HelloWorld.cpp \
    src/main/cpp/hoot/js/OsmMapJs.cpp \
    src/main/cpp/hoot/js/MapIoJs.cpp \
    src/main/cpp/hoot/js/JsRegistrar.cpp \
    src/main/cpp/hoot/js/IdGeneratorJs.cpp \
    src/main/cpp/hoot/js/ops/OsmMapOperationJs.cpp \
    src/main/cpp/hoot/js/util/SettingsJs.cpp \
    src/main/cpp/hoot/js/filter/ElementCriterionJs.cpp \
    src/main/cpp/hoot/js/util/PopulateConsumersJs.cpp \
    src/main/cpp/hoot/js/visitors/ElementVisitorJs.cpp \
    src/main/cpp/hoot/js/PluginContext.cpp \
    src/main/cpp/hoot/js/conflate/js/ScriptMatch.cpp \
    src/main/cpp/hoot/js/conflate/js/ScriptMatchCreator.cpp \
    src/main/cpp/hoot/js/elements/NodeJs.cpp \
    src/main/cpp/hoot/js/elements/ElementJs.cpp \
    src/main/cpp/hoot/js/schema/OsmSchemaJs.cpp \
    src/main/cpp/hoot/js/conflate/extractors/FeatureExtractorJs.cpp \
    src/main/cpp/hoot/js/elements/WayJs.cpp \
    src/main/cpp/hoot/js/elements/RelationJs.cpp \
    src/main/cpp/hoot/js/conflate/js/ScriptMerger.cpp \
    src/main/cpp/hoot/js/conflate/js/ScriptMergerCreator.cpp \
    src/main/cpp/hoot/js/elements/TagsJs.cpp \
    src/main/cpp/hoot/js/schema/TagMergerFactoryJs.cpp \
    src/main/cpp/hoot/js/util/LogJs.cpp \
    src/main/cpp/hoot/js/algorithms/SublineStringMatcherJs.cpp \
    src/main/cpp/hoot/js/algorithms/linearreference/WaySublineMatchStringJs.cpp \
    src/main/cpp/hoot/js/conflate/highway/HighwaySnapMergerJs.cpp \
    src/main/cpp/hoot/js/elements/ElementIdJs.cpp \
    src/main/cpp/hoot/js/util/ElementConverterJs.cpp \
    src/main/cpp/hoot/js/util/HootExceptionJs.cpp \
    src/main/cpp/hoot/js/util/UuidHelperJs.cpp \
    src/main/cpp/hoot/js/JavaScriptTranslator.cpp \
    src/main/cpp/hoot/js/util/PrintJs.cpp \
    src/main/cpp/hoot/js/util/RequireJs.cpp \
    src/main/cpp/hoot/js/filter/JsFunctionCriterion.cpp \
    src/main/cpp/hoot/js/algorithms/StringDistanceJs.cpp \
    src/main/cpp/hoot/js/conflate/js/ScriptStatsComposer.cpp \
    src/main/cpp/hoot/js/algorithms/aggregator/ValueAggregatorJs.cpp \
    src/main/cpp/hoot/js/io/OsmWriterJs.cpp \
    src/main/cpp/hoot/js/conflate/MatchFactoryJs.cpp \
    src/main/cpp/hoot/js/conflate/MergerFactoryJs.cpp \
    src/main/cpp/hoot/js/conflate/PoiMergerJs.cpp \
    src/main/cpp/hoot/js/conflate/ReviewMarkerJs.cpp \
    src/main/cpp/hoot/js/visitors/JsFunctionVisitor.cpp \
    src/main/cpp/hoot/js/schema/TagDifferencerJs.cpp \
    src/main/cpp/hoot/js/schema/JsonOsmSchemaLoader.cpp \
    src/main/cpp/hoot/js/algorithms/string/MostEnglishNameJs.cpp \
    src/main/cpp/hoot/js/conflate/PoiPolygonMergerJs.cpp

HEADERS += \
    src/main/cpp/hoot/js/HelloWorld.h \
    src/main/cpp/hoot/js/OsmMapJs.h \
    src/main/cpp/hoot/js/MapIoJs.h \
    src/main/cpp/hoot/js/JsRegistrar.h \
    src/main/cpp/hoot/js/IdGeneratorJs.h \
    src/main/cpp/hoot/js/ops/OsmMapOperationJs.h \
    src/main/cpp/hoot/js/util/SettingsJs.h \
    src/main/cpp/hoot/js/util/StringUtilsJs.h \
    src/main/cpp/hoot/js/filter/ElementCriterionJs.h \
    src/main/cpp/hoot/js/util/PopulateConsumersJs.h \
    src/main/cpp/hoot/js/visitors/ElementVisitorJs.h \
    src/main/cpp/hoot/js/PluginContext.h \
    src/main/cpp/hoot/js/util/StreamUtilsJs.h \
    src/main/cpp/hoot/js/conflate/js/ScriptMatch.h \
    src/main/cpp/hoot/js/conflate/js/ScriptMatchCreator.h \
    src/main/cpp/hoot/js/elements/NodeJs.h \
    src/main/cpp/hoot/js/elements/ElementJs.h \
    src/main/cpp/hoot/js/schema/OsmSchemaJs.h \
    src/main/cpp/hoot/js/conflate/extractors/FeatureExtractorJs.h \
    src/main/cpp/hoot/js/elements/WayJs.h \
    src/main/cpp/hoot/js/elements/RelationJs.h \
    src/main/cpp/hoot/js/conflate/js/ScriptMerger.h \
    src/main/cpp/hoot/js/conflate/js/ScriptMergerCreator.h \
    src/main/cpp/hoot/js/elements/TagsJs.h \
    src/main/cpp/hoot/js/schema/TagMergerFactoryJs.h \
    src/main/cpp/hoot/js/util/LogJs.h \
    src/main/cpp/hoot/js/algorithms/SublineStringMatcherJs.h \
    src/main/cpp/hoot/js/algorithms/linearreference/WaySublineMatchStringJs.h \
    src/main/cpp/hoot/js/conflate/highway/HighwaySnapMergerJs.h \
    src/main/cpp/hoot/js/elements/ElementIdJs.h \
    src/main/cpp/hoot/js/util/DataConvertJs.h \
    src/main/cpp/hoot/js/util/ElementConverterJs.h \
    src/main/cpp/hoot/js/util/HootExceptionJs.h \
    src/main/cpp/hoot/js/util/UuidHelperJs.h \
    src/main/cpp/hoot/js/SystemNodeJs.h \
    src/main/cpp/hoot/js/JavaScriptTranslator.h \
    src/main/cpp/hoot/js/util/PrintJs.h \
    src/main/cpp/hoot/js/util/RequireJs.h \
    src/main/cpp/hoot/js/filter/JsFunctionCriterion.h \
    src/main/cpp/hoot/js/util/JsFunctionConsumer.h \
    src/main/cpp/hoot/js/algorithms/StringDistanceJs.h \
    src/main/cpp/hoot/js/conflate/js/ScriptStatsComposer.h \
    src/main/cpp/hoot/js/algorithms/aggregator/ValueAggregatorJs.h \
    src/main/cpp/hoot/js/io/OsmWriterJs.h \
    src/main/cpp/hoot/js/conflate/MatchFactoryJs.h \
    src/main/cpp/hoot/js/conflate/MergerFactoryJs.h \
    src/main/cpp/hoot/js/conflate/PoiMergerJs.h \
    src/main/cpp/hoot/js/conflate/ReviewMarkerJs.h \
    src/main/cpp/hoot/js/visitors/JsFunctionVisitor.h \
    src/main/cpp/hoot/js/schema/TagDifferencerJs.h \
    src/main/cpp/hoot/js/schema/JsonOsmSchemaLoader.h \
    src/main/cpp/hoot/js/algorithms/string/MostEnglishNameJs.h \
    src/main/cpp/hoot/js/conflate/PoiPolygonMergerJs.h

FORMS +=

OTHER_FILES += \
    binding.gyp \
    README.txt
