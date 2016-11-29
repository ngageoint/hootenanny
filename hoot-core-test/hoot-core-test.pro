# -------------------------------------------------
# Project created by QtCreator 2011-09-29T12:27:47
# -------------------------------------------------
QT += script \
    sql \
    testlib \
    xml \
    network \

TARGET = HootCoreTest
TEMPLATE = lib

DEPENDPATH += \
    ../tbs/src/main/cpp/ \
    ../tgs/src/main/cpp/ \
    ../hoot-core/src/main/cpp/ \

INCLUDEPATH += \
  $${DEPENDPATH} \
  ../local/include/ \

CONFIG += rtti \
    qtestlib \
    debug

OTHER_FILES = \
    ../conf/ConfigOptions.asciidoc \
    ../rules/LineStringGeneric.js \
    ../rules/LineStringGenericTest.js \
    ../rules/PoiGeneric.js \
    ../rules/lib/HootLib.js \

include(../Configure.pri)

LIBS += -L../lib/ -lTgs -ltbs

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
release:OBJECTS_DIR = tmp/release
else:OBJECTS_DIR = tmp/debug
DESTDIR = ../lib/

cppunit {
SOURCES += \
    src/test/cpp/hoot/core/ConflatorTest.cpp \
    src/test/cpp/hoot/core/algorithms/MaximalNearestSublineTest.cpp \
    src/test/cpp/hoot/core/io/OsmReaderTest.cpp \
    src/test/cpp/hoot/core/scoring/RasterComparatorTest.cpp \
    src/test/cpp/hoot/core/conflate/splitter/IntersectionSplitterTest.cpp \
    src/test/cpp/hoot/core/scoring/GraphComparatorTest.cpp \
    src/test/cpp/hoot/core/manipulators/DividedHighwayManipulationTest.cpp \
    src/test/cpp/hoot/core/manipulators/DividedHighwayMergerTest.cpp \
    src/test/cpp/hoot/core/conflate/splitter/DualWaySplitterTest.cpp \
    src/test/cpp/hoot/core/algorithms/LongestCommonNodeStringTest.cpp \
    src/test/cpp/hoot/core/conflate/DuplicateWayRemoverTest.cpp \
    src/test/cpp/hoot/core/OsmMapTest.cpp \
    src/test/cpp/hoot/core/ops/MergeNearbyNodesTest.cpp \
    src/test/cpp/hoot/core/algorithms/LevenshteinDistanceTest.cpp \
    src/test/cpp/hoot/core/schema/OsmSchemaTest.cpp \
    src/test/cpp/hoot/core/schema/SchemaCheckerTest.cpp \
    src/test/cpp/hoot/core/schema/TagComparatorTest.cpp \
    src/test/cpp/hoot/core/scoring/AttributeComparatorTest.cpp \
    src/test/cpp/hoot/core/index/ClosePointHashTest.cpp \
    src/test/cpp/hoot/core/conflate/SmallWayMergerTest.cpp \
    src/test/cpp/hoot/core/manipulators/WayMergeManipulationTest.cpp \
    src/test/cpp/hoot/core/TestUtils.cpp \
    src/test/cpp/hoot/core/conflate/TileConflatorTest.cpp \
    src/test/cpp/hoot/core/conflate/LargeWaySplitterTest.cpp \
    src/test/cpp/hoot/core/conflate/NodeReplacementsTest.cpp \
    src/test/cpp/hoot/core/conflate/TileBoundsCalculatorTest.cpp \
    src/test/cpp/hoot/core/io/PbfReaderTest.cpp \
    src/test/cpp/hoot/core/io/PbfWriterTest.cpp \
    src/test/cpp/hoot/core/ml/LogisticRegressionTest.cpp \
    src/test/cpp/hoot/core/conflate/NodeMatcherTest.cpp \
    src/test/cpp/hoot/core/util/SettingsTest.cpp \
    src/test/cpp/hoot/core/algorithms/AlphaShapeTest.cpp \
    src/test/cpp/hoot/core/conflate/MapCleanerTest.cpp \
    src/test/cpp/hoot/core/io/ObjectOutputStreamTest.cpp \
    src/test/cpp/hoot/core/io/ObjectInputStreamTest.cpp \
    src/test/cpp/hoot/core/ops/MapCropperTest.cpp \
    src/test/cpp/hoot/core/fourpass/FourPassManagerTest.cpp \
    src/test/cpp/hoot/core/ops/BuildingPartMergeOpTest.cpp \
    src/test/cpp/hoot/core/conflate/RubberSheetTest.cpp \
    src/test/cpp/hoot/core/algorithms/SoundexTest.cpp \
    src/test/cpp/hoot/core/algorithms/TranslatorTest.cpp \
    src/test/cpp/hoot/core/algorithms/MeanWordSetDistanceTest.cpp \
    src/test/cpp/hoot/core/schema/TranslateStringDistanceTest.cpp \
    src/test/cpp/hoot/core/conflate/match-graph/MatchGraphTest.cpp \
    src/test/cpp/hoot/core/conflate/polygon/BuildingMatchCreatorTest.cpp \
    src/test/cpp/hoot/core/conflate/polygon/BuildingMergerTest.cpp \
    src/test/cpp/hoot/core/ops/RecursiveElementRemoverTest.cpp \
    src/test/cpp/hoot/core/scoring/MatchComparatorTest.cpp \
    src/test/cpp/hoot/core/visitors/RemoveDuplicateAreaVisitorTest.cpp \
    src/test/cpp/hoot/core/conflate/polygon/extractors/BufferedOverlapExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/polygon/extractors/CentroidDistanceExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/polygon/extractors/EdgeDistanceExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/polygon/extractors/HausdorffDistanceExtractorTest.cpp \
    src/test/cpp/hoot/core/algorithms/MaximalSublineTest.cpp \
    src/test/cpp/hoot/core/algorithms/ExpectationIntersectionTest.cpp \
    src/test/cpp/hoot/core/algorithms/optimizer/IntegerProgrammingSolverTest.cpp \
    src/test/cpp/hoot/core/elements/NodeTest.cpp \
    src/test/cpp/hoot/core/perty/PertyNameVisitorTest.cpp \
    src/test/cpp/hoot/core/perty/PertyDuplicatePoiOpTest.cpp \
    src/test/cpp/hoot/core/perty/PertyRemoveTagVisitorTest.cpp \
    src/test/cpp/hoot/core/MapProjectorTest.cpp \
    src/test/cpp/hoot/core/util/GeometryUtilsTest.cpp \
    src/test/cpp/hoot/core/elements/TagsTest.cpp \
    src/test/cpp/hoot/core/perty/BigPertyOpTest.cpp \
    src/test/cpp/hoot/core/index/metric-hybrid/FqTreeTest.cpp \
    src/test/cpp/hoot/core/index/metric-hybrid/RTreeTest.cpp \
    src/test/cpp/hoot/core/index/metric-hybrid/RFqHybridTreeTest.cpp \
    src/test/cpp/hoot/core/schema/TagMergerFactoryTest.cpp \
    src/test/cpp/hoot/core/schema/OverwriteTagMergerTest.cpp \
    src/test/cpp/hoot/core/util/UuidHelperTest.cpp \
    src/test/cpp/hoot/core/conflate/highway/HighwayMatchTest.cpp \
    src/test/cpp/hoot/core/conflate/highway/HighwayExpertClassifierTest.cpp \
    src/test/cpp/hoot/core/algorithms/MaximalNearestSublineMatcherTest.cpp \
    src/test/cpp/hoot/core/algorithms/MaximalSublineStringMatcherTest.cpp \
    src/test/cpp/hoot/core/visitors/KeepNodesVisitorTest.cpp \
    src/test/cpp/hoot/core/algorithms/BufferedLineSegmentIntersectorTest.cpp \
    src/test/cpp/hoot/core/algorithms/linearreference/WayLocationTest.cpp \
    src/test/cpp/hoot/core/algorithms/linearreference/WaySublineStringTest.cpp \
    src/test/cpp/hoot/core/algorithms/zindex/ZValueTest.cpp \
    src/test/cpp/hoot/core/algorithms/zindex/ZCurveRangerTest.cpp \
    src/test/cpp/hoot/core/algorithms/zindex/BBoxTest.cpp \
    src/test/cpp/hoot/core/algorithms/zindex/LongBoxTest.cpp \
    src/test/cpp/hoot/core/algorithms/zindex/RangeTest.cpp \
    src/test/cpp/hoot/core/conflate/highway/HighwaySnapMergerTest.cpp \
    src/test/cpp/hoot/core/algorithms/MultiLineStringSplitterTest.cpp \
    src/test/cpp/hoot/core/algorithms/linearreference/WaySublineMatchStringTest.cpp \
    src/test/cpp/hoot/core/conflate/match-graph/OptimalConstrainedMatchesTest.cpp \
    src/test/cpp/hoot/core/conflate/match-graph/GreedyConstrainedMatchesTest.cpp \
    src/test/cpp/hoot/core/algorithms/linearreference/WaySublineTest.cpp \
    src/test/cpp/hoot/core/util/GeometryConverterTest.cpp \
    src/test/cpp/hoot/core/scoring/MatchFeatureExtractorTest.cpp \
    src/test/cpp/hoot/core/util/MultiPolygonCreatorTest.cpp \
    src/test/cpp/hoot/core/conflate/ReviewMarkerTest.cpp \
    src/test/cpp/hoot/core/visitors/TagCountVisitorTest.cpp \
    src/test/cpp/hoot/core/algorithms/linearreference/MultiLineStringLocationTest.cpp \
    src/test/cpp/hoot/core/perty/PertyWaySplitVisitorTest.cpp \
    src/test/cpp/hoot/core/ops/RefRemoveOpTest.cpp \
    src/test/cpp/hoot/core/ops/FindIntersectionsOpTest.cpp \
    src/test/cpp/hoot/core/perty/PertyWayGeneralizeVisitorTest.cpp \
    src/test/cpp/hoot/core/algorithms/RdpWayGeneralizerTest.cpp \
    src/test/cpp/hoot/core/visitors/RemoveRef2VisitorTest.cpp \
    src/test/cpp/hoot/core/perty/PertyRemoveRandomElementVisitorTest.cpp \
    src/test/cpp/hoot/core/visitors/TagRenameKeyVisitorTest.cpp \
    src/test/cpp/hoot/core/visitors/TagKeyCountVisitorTest.cpp \
    src/test/cpp/hoot/core/visitors/SplitNameVisitorTest.cpp \
    src/test/cpp/hoot/core/conflate/DuplicateNameRemoverTest.cpp \
    src/test/cpp/hoot/core/conflate/NoInformationElementRemoverTest.cpp \
    src/test/cpp/hoot/core/perty/PertyMatchScorerTest.cpp \
    src/test/cpp/hoot/core/algorithms/optimizer/SingleAssignmentProblemSolverTest.cpp \
    src/test/cpp/hoot/core/conflate/extractors/SampledAngleHistogramExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/extractors/WeightedMetricDistanceExtractorTest.cpp \
    src/test/cpp/hoot/core/io/ArffWriterTest.cpp \
    src/test/cpp/hoot/core/io/ArffReaderTest.cpp \
    src/test/cpp/hoot/core/conflate/WayCleanerTest.cpp \
    src/test/cpp/hoot/core/scoring/MapScoringStatusAndRefTagValidatorTest.cpp \
    src/test/cpp/hoot/core/ops/BuildingOutlineUpdateOpTest.cpp \
    src/test/cpp/hoot/core/conflate/highway/HighwayMatchCreatorTest.cpp \
    src/test/cpp/hoot/core/conflate/ConflateStatsHelperTest.cpp \
    src/test/cpp/hoot/core/visitors/MatchCandidateCountVisitorTest.cpp \
    src/test/cpp/hoot/core/visitors/KeepTagsVisitorTest.cpp \
    src/test/cpp/hoot/core/conflate/CookieCutterTest.cpp \
    src/test/cpp/hoot/core/conflate/AlphaShapeGeneratorTest.cpp \
    src/test/cpp/hoot/core/perty/PertyOpTest.cpp \
    src/test/cpp/hoot/core/ops/CookieCutterOpTest.cpp \
    src/test/cpp/hoot/core/visitors/RemoveElementsVisitorTest.cpp \
    src/test/cpp/hoot/core/visitors/AddGeometryTypeVisitorTest.cpp \
    src/test/cpp/hoot/core/visitors/TransliterateNameVisitorTest.cpp \
    src/test/cpp/hoot/core/io/ElementSorterTest.cpp \
    src/test/cpp/hoot/core/io/OsmChangesetXmlFileWriterTest.cpp \
    src/test/cpp/hoot/core/io/ChangesetDeriverTest.cpp \
    src/test/cpp/hoot/core/io/ElementComparerTest.cpp \
    src/test/cpp/hoot/core/visitors/RemoveInvalidReviewRelationsVisitorTest.cpp \
    src/test/cpp/hoot/core/filters/BuildingWayNodeCriterionTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/PoiPolygonMatchTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/PoiPolygonMergerTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/PoiPolygonMergerCreatorTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/PoiPolygonMatchCreatorTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/extractors/PoiPolygonAddressScoreExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/extractors/PoiPolygonAlphaShapeDistanceExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/extractors/PoiPolygonDistanceExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/extractors/PoiPolygonNameScoreExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/extractors/PoiPolygonTypeScoreExtractorTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/filters/PoiPolygonPolyCriterionTest.cpp \
    src/test/cpp/hoot/core/conflate/poi-polygon/filters/PoiPolygonPoiCriterionTest.cpp \
    src/test/cpp/hoot/core/filters/AreaCriterionTest.cpp \
    src/test/cpp/hoot/core/filters/WayCriterionTest.cpp \
    src/test/cpp/hoot/core/filters/NonBuildingAreaCriterionTest.cpp \
    src/test/cpp/hoot/core/visitors/CountManualMatchesVisitorTest.cpp

# These tests only succeed if the database has been populated by the java code.
services:SOURCES += \
    src/test/cpp/hoot/core/io/HootApiDbTest.cpp \
    src/test/cpp/hoot/core/io/ServicesDbTestUtils.cpp \
    src/test/cpp/hoot/core/io/HootApiDbReaderTest.cpp \
    src/test/cpp/hoot/core/io/HootApiDbWriterTest.cpp \
    src/test/cpp/hoot/core/io/PostgresqlDumpfileWriterTest.cpp \
    src/test/cpp/hoot/core/io/ServiceOsmApiDbSqlChangesetWriterTest.cpp \
    src/test/cpp/hoot/core/io/ServiceOsmApiDbChangesetSqlFileWriterTest.cpp \
    src/test/cpp/hoot/core/io/ServiceOsmApiDbTest.cpp \
    src/test/cpp/hoot/core/io/ServiceOsmApiDbReaderTest.cpp \

HEADERS += \
    src/test/cpp/hoot/core/algorithms/MaximalNearestSublineTest.h \
    src/test/cpp/hoot/core/TestUtils.h \
    src/test/cpp/hoot/core/io/ServicesDbTestUtils.h \


nodejs:SOURCES += \
    src/test/cpp/hoot/core/io/OgrReaderTest.cpp \
    src/test/cpp/hoot/core/io/OgrWriterTest.cpp \
    src/test/cpp/hoot/core/ops/CalculateStatsOpTest.cpp \
    src/test/cpp/hoot/core/perty/PertyTestRunnerTest.cpp \
    src/test/cpp/hoot/core/schema/TranslatedTagDifferencerTest.cpp \

}

SOURCES += \
    src/test/cpp/hoot/core/conflate/SearchRadiusCalculatorTest.cpp \
    src/test/cpp/hoot/core/io/ElementCacheLruTest.cpp \
    src/test/cpp/hoot/core/algorithms/string/TextFileWordWeightDictionaryTest.cpp \
    src/test/cpp/hoot/core/schema/ScoreMatrixTest.cpp \
    src/test/cpp/hoot/core/algorithms/string/WeightedWordDistanceTest.cpp \
    src/test/cpp/hoot/core/algorithms/string/SqliteWeightedWordDistanceTest.cpp \
    src/test/cpp/hoot/core/filters/BuildingCriterionTest.cpp \
    src/test/cpp/hoot/core/visitors/SplitLongLinearWaysVisitorTest.cpp \
    src/test/cpp/hoot/core/elements/WayTest.cpp \
    src/test/cpp/hoot/core/algorithms/string/StringTokenizerTest.cpp \
    src/test/cpp/hoot/core/schema/TagCategoryDifferencerTest.cpp \
    src/test/cpp/hoot/core/schema/TagAncestorDifferencerTest.cpp \
    src/test/cpp/hoot/core/elements/RelationTest.cpp \
    src/test/cpp/hoot/core/algorithms/string/MostEnglishNameTest.cpp \
    src/test/cpp/hoot/core/visitors/MedianNodeVisitorTest.cpp \
    src/test/cpp/hoot/core/ops/BuildingOutlineRemoveOpTest.cpp \
    src/test/cpp/hoot/core/ops/ReprojectToGeographicOpTest.cpp \
    src/test/cpp/hoot/core/ops/ReprojectToPlanarOpTest.cpp \
    src/test/cpp/hoot/core/ops/WaySplitterOpTest.cpp \
    src/test/cpp/hoot/core/ops/TrivialOpTest.cpp \
    src/test/cpp/hoot/core/conflate/extractors/HistogramTest.cpp \
    src/test/cpp/hoot/core/algorithms/WayMatchStringMergerTest.cpp \
    src/test/cpp/hoot/core/algorithms/linearreference/WayMatchStringMappingConverterTest.cpp \
    src/test/cpp/hoot/core/io/OsmJsonReaderTest.cpp \
    src/test/cpp/hoot/core/algorithms/KskipBigramDistanceTest.cpp \
    src/test/cpp/hoot/core/test/ConflateCaseTest.cpp \
    src/test/cpp/hoot/core/test/ConflateCaseTestSuite.cpp \
    src/test/cpp/hoot/core/algorithms/linearreference/WayStringTest.cpp

HEADERS += \
    src/test/cpp/hoot/core/test/ConflateCaseTest.h \
    src/test/cpp/hoot/core/test/ConflateCaseTestSuite.h


