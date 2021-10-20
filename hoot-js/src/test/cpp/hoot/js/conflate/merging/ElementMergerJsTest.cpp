/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/elements/MapProjector.h>

#include <hoot/js/HootJsStable.h>
#include <hoot/js/conflate/merging/ElementMergerJs.h>

using namespace v8;

namespace hoot
{

/**
 * Tests merging two or more features
 *
 * Types of tests:
 *
 * 1) standard - tests successful merging
 * 2) w/o constituents - See notes in ElementMergerJs about features passed in without constituent
 * elements.  For those tests, we're setting the log at error, so as not to see the missing element
 * warnings.
 * 3) missing feature inputs - all merging requires at least two features
 * 4) extra unrelated features - any features that aren't mergeable should pass through to the
 * output
 * 5) more than two feature inputs - some merging can support more than two input features
 * 6) duplicated/missing target tag - merging requires a particular tag to denote which feature
 * gets other features merged into it; this applies to all types of merging except poi/poly
 * 7) invalid feature combinations - merging doesn't allow input with more than one mergeable
 * feature type associated with different types of conflation; e.g. an input with both buildings
 * and POIs in it
 * 8) input features with a conflated status - A conflated status only causes problems with POI to
 * Poly and Building to Building conflation generally, however tests have been added for it for all
 * conflation types to be safe. The aforementioned types of conflation both use building merging,
 * which expects all inputs to have a status of either Unknown1 or Unknown2.
 *
 * Many, but not all, possible combinations of inputs are covered here.  The main functionality
 * we're not testing here in the merging of features through the js bindings workflow is the
 * conversion of the map arg from a nodejs object to a hoot object.  That gets tested by the element
 * merge service mocha plugin test, translations/test/ElementMergerServer.js.
 */
class ElementMergerJsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ElementMergerJsTest);

  // POI TO POLYGON
  CPPUNIT_TEST(poiToPolyMergeWayAsPolyTest);
  CPPUNIT_TEST(poiToPolyMergeWayAsPolyNoConstituentsTest);
  CPPUNIT_TEST(poiToPolyMergeRelationAsPolyTest);
  CPPUNIT_TEST(poiToPolyMergeRelationAsPolyNoConstituentsTest);
  CPPUNIT_TEST(poiToPolyMergeMissingPoiInputTest);
  CPPUNIT_TEST(poiToPolyMergeMissingPolyInputTest);
  CPPUNIT_TEST(poiToPolyMergeMoreThanOnePolyInputTest);
  CPPUNIT_TEST(poiToPolyMergeMoreThanOnePoiInputTest);
  CPPUNIT_TEST(poiToPolyMergeExtraNonPolyWayTest);
  CPPUNIT_TEST(poiToPolyMergeExtraNonPolyRelationTest);
  CPPUNIT_TEST(poiToPolyMergeExtraNonPoiNodeTest);
  CPPUNIT_TEST(poiToPolyPolyInputWithConflatedStatusTest);
  CPPUNIT_TEST(poiToPolyPoiInputWithConflatedStatusTest);

  // POI TO POI
  CPPUNIT_TEST(poiToPoiMergeTwoPoisTest);
  CPPUNIT_TEST(poiToPoiMergeMoreThanTwoPoisTest);
  CPPUNIT_TEST(poiToPoiMergeTooFewPoisTest);
  CPPUNIT_TEST(poiToPoiMergeMissingTargetTagTest);
  CPPUNIT_TEST(poiToPoiMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(poiToPoiMergeExtraNonPoiNodeTest);
  CPPUNIT_TEST(poiToPoiInputWithConflatedStatusTest);

  // AREA TO AREA
  CPPUNIT_TEST(areaToAreaMergeTwoWaysTest);
  CPPUNIT_TEST(areaToAreaMergeTwoWaysNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeTwoRelationsTest);
  CPPUNIT_TEST(areaToAreaMergeTwoRelationsNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeOneWayOneRelationTargetAsWayTest);
  CPPUNIT_TEST(areaToAreaMergeOneWayOneRelationTargetAsWayNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeOneWayOneRelationTargetAsRelationTest);
  CPPUNIT_TEST(areaToAreaMergeOneWayOneRelationTargetAsRelationNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeMoreThanTwoAreasTest);
  CPPUNIT_TEST(areaToAreaMergeMoreThanTwoAreasNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeTooFewAreasTest);
  CPPUNIT_TEST(areaToAreaMergeMissingTargetTagTest);
  CPPUNIT_TEST(areaToAreaMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(areaToAreaMergeExtraNonAreaWayTest);
  CPPUNIT_TEST(areaToAreaMergeExtraNonAreaRelationTest);
  CPPUNIT_TEST(areaToAreaInputWithConflatedStatusTest);

  // BUILDING TO BUILDING
  CPPUNIT_TEST(buildingToBuildingMergeTwoWaysTest);
  CPPUNIT_TEST(buildingToBuildingMergeTwoWaysNoConstituentsTest);
  CPPUNIT_TEST(buildingToBuildingMergeTwoRelationsTest);
  CPPUNIT_TEST(buildingToBuildingMergeTwoRelationsNoConstituentsTest);
  CPPUNIT_TEST(buildingToBuildingMergeOneWayOneRelationTargetAsWayTest);
  CPPUNIT_TEST(buildingToBuildingMergeOneWayOneRelationTargetAsWayNoConstituentsTest);
  CPPUNIT_TEST(buildingToBuildingMergeOneWayOneRelationTargetAsRelationTest);
  CPPUNIT_TEST(buildingToBuildingMergeOneWayOneRelationTargetAsRelationNoConstituentsTest);
  CPPUNIT_TEST(buildingToBuildingMergeMoreThanTwoBuildingsTest);
  CPPUNIT_TEST(buildingToBuildingMergeMoreThanTwoBuildingsNoConstituentsTest);
  CPPUNIT_TEST(buildingToBuildingMergeTooFewBuildingsTest);
  CPPUNIT_TEST(buildingToBuildingMergeMissingTargetTagTest);
  CPPUNIT_TEST(buildingToBuildingMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(buildingToBuildingMergeExtraNonBuildingWayTest);
  CPPUNIT_TEST(buildingToBuildingMergeExtraNonBuildingRelationTest);
  CPPUNIT_TEST(buildingToBuildingInputWithConflatedStatusTest);

  // TODO: add railway to railway

  // INVALID FEATURE COMBOS
  CPPUNIT_TEST(invalidFeatureCombinationTest1);
  CPPUNIT_TEST(invalidFeatureCombinationTest2);
  CPPUNIT_TEST(invalidFeatureCombinationTest3);
  CPPUNIT_TEST(invalidFeatureCombinationTest4);

  CPPUNIT_TEST_SUITE_END();

public:

  ElementMergerJsTest() :
  HootTestFixture(
    "test-files/js/conflate/ElementMergerJsTest/", "test-output/js/conflate/ElementMergerJsTest/")
  {
    setResetType(ResetEnvironment);
  }

  void testMerge(const QString& inFileName, const QString& outFileName,
                 const QString& expectedExceptionMsgContains = "")
  {
    // Poi/poly test output is based on this config for now, despite it being different than the
    // default config.
    conf().set(ConfigOptions::getAddressMatchEnabledKey(), "false");
    conf().set(ConfigOptions::getPoiPolygonTagMergerKey(), "OverwriteTag2Merger");

    LOG_VART(inFileName);
    LOG_VART(outFileName);
    LOG_VART(expectedExceptionMsgContains);
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map = std::make_shared<OsmMap>();
      OsmMapReaderFactory::read(map, true, true, _inputPath + inFileName);

      ElementMergerJs::_merge(map, v8::Isolate::GetCurrent());

      MapProjector::projectToWgs84(map);
      OsmMapWriterFactory::write(map, _outputPath + outFileName);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);

    if (!exceptionMsg.isEmpty())
    {
      if (!expectedExceptionMsgContains.isEmpty())
      {
        CPPUNIT_ASSERT(exceptionMsg.contains(expectedExceptionMsgContains));
      }
      else
      {
        throw HootException(exceptionMsg);
      }
    }
    else
    {
      HOOT_FILE_EQUALS(_inputPath + outFileName, _outputPath + outFileName);
    }
  }

  //POI TO POLYGON

  void poiToPolyMergeWayAsPolyTest()
  {
    testMerge("poi-poly-way-poly-in.osm", "poi-poly-way-poly-out.osm");
  }

  void poiToPolyMergeWayAsPolyNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "poi-poly-way-poly-no-constituents-in.osm", "poi-poly-way-poly-no-constituents-out.osm");
  }

  void poiToPolyMergeRelationAsPolyTest()
  {
    testMerge("poi-poly-relation-poly-in.osm", "poi-poly-relation-poly-out.osm");
  }

  void poiToPolyMergeRelationAsPolyNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "poi-poly-relation-poly-no-constituents-in.osm",
      "poi-poly-relation-poly-no-constituents-out.osm");
  }

  void poiToPolyMergeMissingPoiInputTest()
  {
    testMerge(
      "poi-poly-missing-poi-in.osm", "poi-poly-missing-poi-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void poiToPolyMergeMissingPolyInputTest()
  {
    testMerge(
      "poi-poly-missing-poly-in.osm", "poi-poly-missing-poly-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void poiToPolyMergeMoreThanOnePolyInputTest()
  {
    testMerge(
      "poi-poly-more-than-one-poly-in.osm", "poi-poly-more-than-one-poly-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void poiToPolyMergeMoreThanOnePoiInputTest()
  {
    testMerge(
      "poi-poly-more-than-one-poi-in.osm", "poi-poly-more-than-one-poi-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void poiToPolyMergeExtraNonPolyWayTest()
  {
    testMerge("poi-poly-extra-non-poly-way-in.osm", "poi-poly-extra-non-poly-way-out.osm");
  }

  void poiToPolyMergeExtraNonPolyRelationTest()
  {
    testMerge("poi-poly-extra-non-poly-relation-in.osm", "poi-poly-extra-non-poly-relation-out.osm");
  }

  void poiToPolyMergeExtraNonPoiNodeTest()
  {
    testMerge("poi-poly-extra-non-poi-node-in.osm", "poi-poly-extra-non-poi-node-out.osm");
  }

  void poiToPolyPolyInputWithConflatedStatusTest()
  {
    testMerge("poi-poly-way-poly-conflated-1-in.osm", "poi-poly-poly-input-conflated-out.osm");
  }

  void poiToPolyPoiInputWithConflatedStatusTest()
  {
    testMerge(
      "poi-poly-way-poly-conflated-2-in.osm", "poi-poly-poi-input-conflated-out.osm");
  }

  //POI TO POI

  void poiToPoiMergeTwoPoisTest()
  {
    testMerge("poi-two-in.osm", "poi-two-out.osm");
  }

  void poiToPoiMergeMoreThanTwoPoisTest()
  {
    testMerge("poi-more-than-two-in.osm", "poi-more-than-two-out.osm");
  }

  void poiToPoiMergeTooFewPoisTest()
  {
    testMerge(
      "poi-too-few-in.osm", "poi-too-few-out.osm", "Invalid inputs passed to the element merger");
  }

  void poiToPoiMergeMissingTargetTagTest()
  {
    testMerge(
      "poi-missing-target-tag-in.osm", "poi-missing-target-tag-out.osm",
      "Input map must have one feature marked with a hoot:merge:target tag");
  }

  void poiToPoiMergeDuplicateTargetTagTest()
  {
    testMerge(
      "poi-duplicate-target-tag-in.osm", "poi-duplicate-target-tag-out.osm",
      "Input map must have one feature marked with a hoot:merge:target tag");
  }

  void poiToPoiMergeExtraNonPoiNodeTest()
  {
    testMerge("poi-extra-non-poi-node-in.osm", "poi-extra-non-poi-node-out.osm");
  }

  void poiToPoiInputWithConflatedStatusTest()
  {
    testMerge("poi-two-conflated-in.osm", "poi-two-out.osm");
  }

  //AREA TO AREA

  void areaToAreaMergeTwoWaysTest()
  {
    testMerge("area-two-ways-in.osm", "area-two-ways-out.osm");
  }

  void areaToAreaMergeTwoWaysNoConstituentsTest()
  {
    DisableLog dl;
    testMerge("area-two-ways-no-constituents-in.osm", "area-two-ways-no-constituents-out.osm");
  }

  void areaToAreaMergeTwoRelationsTest()
  {
    testMerge("area-two-relations-in.osm", "area-two-relations-out.osm");
  }

  void areaToAreaMergeTwoRelationsNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "area-two-relations-no-constituents-in.osm", "area-two-relations-no-constituents-out.osm");
  }

  void areaToAreaMergeOneWayOneRelationTargetAsWayTest()
  {
    testMerge(
      "area-one-way-one-relation-target-as-way-in.osm",
      "area-one-way-one-relation-target-as-way-out.osm");
  }

  void areaToAreaMergeOneWayOneRelationTargetAsWayNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "area-one-way-one-relation-target-as-way-no-constituents-in.osm",
      "area-one-way-one-relation-target-as-way-no-constituents-out.osm");
  }

  void areaToAreaMergeOneWayOneRelationTargetAsRelationTest()
  {
    testMerge(
      "area-one-way-one-relation-target-as-relation-in.osm",
      "area-one-way-one-relation-target-as-relation-out.osm");
  }

  void areaToAreaMergeOneWayOneRelationTargetAsRelationNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "area-one-way-one-relation-target-as-relation-no-constituents-in.osm",
      "area-one-way-one-relation-target-as-relation-no-constituents-out.osm");
  }

  void areaToAreaMergeMoreThanTwoAreasTest()
  {
    testMerge("area-more-than-two-in.osm", "area-more-than-two-out.osm");
  }

  void areaToAreaMergeMoreThanTwoAreasNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "area-more-than-two-no-constituents-in.osm", "area-more-than-two-no-constituents-out.osm");
  }

  void areaToAreaMergeTooFewAreasTest()
  {
    testMerge(
      "area-too-few-in.osm", "area-too-few-out.osm", "Invalid inputs passed to the element merger");
  }

  void areaToAreaMergeMissingTargetTagTest()
  {
    testMerge(
      "area-missing-target-tag-in.osm", "area-missing-target-tag-out.osm",
      "Input map must have one feature marked with a hoot:merge:target tag");
  }

  void areaToAreaMergeDuplicateTargetTagTest()
  {
    testMerge(
      "area-duplicate-target-tag-in.osm", "area-duplicate-target-tag-out.osm",
      "Input map must have one feature marked with a hoot:merge:target tag");
  }

  void areaToAreaMergeExtraNonAreaWayTest()
  {
    testMerge("area-extra-non-area-way-in.osm", "area-extra-non-area-way-out.osm");
  }

  void areaToAreaMergeExtraNonAreaRelationTest()
  {
    testMerge("area-extra-non-area-relation-in.osm", "area-extra-non-area-relation-out.osm");
  }

  void areaToAreaInputWithConflatedStatusTest()
  {
    testMerge("area-two-ways-conflated-in.osm", "area-two-ways-out.osm");
  }

  //BUILDING TO BUILDING

  void buildingToBuildingMergeTwoWaysTest()
  {
    testMerge("building-two-ways-in.osm", "building-two-ways-out.osm");
  }

  void buildingToBuildingMergeTwoWaysNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "building-two-ways-no-constituents-in.osm", "building-two-ways-no-constituents-out.osm");
  }

  void buildingToBuildingMergeTwoRelationsTest()
  {
    testMerge("building-two-relations-in.osm", "building-two-relations-out.osm");
  }

  void buildingToBuildingMergeTwoRelationsNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "building-two-relations-no-constituents-in.osm",
      "building-two-relations-no-constituents-out.osm");
  }

  void buildingToBuildingMergeOneWayOneRelationTargetAsWayTest()
  {
    testMerge(
      "building-one-way-one-relation-target-as-way-in.osm",
      "building-one-way-one-relation-target-as-way-out.osm");
  }

  void buildingToBuildingMergeOneWayOneRelationTargetAsWayNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "building-one-way-one-relation-target-as-way-no-constituents-in.osm",
      "building-one-way-one-relation-target-as-way-no-constituents-out.osm");
  }

  void buildingToBuildingMergeOneWayOneRelationTargetAsRelationTest()
  {
    testMerge(
      "building-one-way-one-relation-target-as-relation-in.osm",
      "building-one-way-one-relation-target-as-relation-out.osm");
  }

  void buildingToBuildingMergeOneWayOneRelationTargetAsRelationNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "building-one-way-one-relation-target-as-relation-no-constituents-in.osm",
      "building-one-way-one-relation-target-as-relation-no-constituents-out.osm");
  }

  void buildingToBuildingMergeMoreThanTwoBuildingsTest()
  {
    testMerge("building-more-than-two-in.osm", "building-more-than-two-out.osm");
  }

  void buildingToBuildingMergeMoreThanTwoBuildingsNoConstituentsTest()
  {
    DisableLog dl;
    testMerge(
      "building-more-than-two-no-constituents-in.osm",
      "building-more-than-two-no-constituents-out.osm");
  }

  void buildingToBuildingMergeTooFewBuildingsTest()
  {
    testMerge(
      "building-too-few-in.osm", "building-too-few-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void buildingToBuildingMergeMissingTargetTagTest()
  {
      testMerge(
        "building-missing-target-tag-in.osm", "building-missing-target-tag-out.osm",
        "Input map must have one feature marked with a hoot:merge:target tag");
  }

  void buildingToBuildingMergeDuplicateTargetTagTest()
  {
      testMerge(
        "building-missing-target-tag-in.osm", "building-missing-target-tag-out.osm",
        "Input map must have one feature marked with a hoot:merge:target tag");
  }

  void buildingToBuildingMergeExtraNonBuildingWayTest()
  {
    testMerge("building-extra-non-building-way-in.osm", "building-extra-non-building-way-out.osm");
  }

  void buildingToBuildingMergeExtraNonBuildingRelationTest()
  {
    testMerge(
      "building-extra-non-building-relation-in.osm",
      "building-extra-non-building-relation-out.osm");
  }

  void buildingToBuildingInputWithConflatedStatusTest()
  {
    testMerge(
      "building-two-ways-conflated-in.osm", "building-two-ways-out.osm",
      "Elements being merged must have an Unknown1 or Unknown2 status.");
  }

  //MISC

  void invalidFeatureCombinationTest1()
  {
    testMerge("building-too-few-in.osm", "", "Invalid inputs passed to the element merger");
  }

  void invalidFeatureCombinationTest2()
  {
    testMerge("building-too-few-in.osm", "", "Invalid inputs passed to the element merger");
  }

  void invalidFeatureCombinationTest3()
  {
    testMerge("building-too-few-in.osm", "", "Invalid inputs passed to the element merger");
  }

  void invalidFeatureCombinationTest4()
  {
    testMerge("building-too-few-in.osm", "", "Invalid inputs passed to the element merger");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementMergerJsTest, "quick");

}
