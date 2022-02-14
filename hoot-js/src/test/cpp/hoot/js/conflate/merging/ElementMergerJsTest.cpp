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
 * elements. For those tests we're setting the log at error, so as not to see the missing element
 * warnings.
 * 3) missing feature inputs - all merging requires at least two features
 * 4) more than two feature inputs - some merging can support more than two input features
 * 5) duplicated/missing target tag - merging requires a particular tag to denote which feature
 * gets other features merged into it; this applies to all types of merging except poi/poly
 * 6) invalid feature combinations - merging doesn't allow input with more than one mergeable
 * feature type associated with different types of conflation; e.g. an input with both buildings
 * and POIs in it.
 * 7) input features with a conflated status - A conflated status only causes problems with POI to
 * Poly and Building to Building conflation generally, however tests have been added for it for all
 * conflation types to be safe. The aforementioned types of conflation both use building merging,
 * which expects all inputs to have a status of either Unknown1 or Unknown2.
 *
 * Many, but not all, possible combinations of inputs are covered here. The main functionality
 * we're not testing here in the merging of features through the js bindings workflow is the
 * conversion of the map arg from a nodejs object to a hoot object. That gets tested by the element
 * merge service mocha plugin test, translations/test/ElementMergerServer.js.
 *
 * @note The conflate output for the ref rails test doesn't cleanly merge the inputs, so it may be
 * worthwhile to pick a different input for it.
 *
 * @todo The invalid feature combination tests still need to be completed for all feature types
 * except railways.
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
  CPPUNIT_TEST(poiToPolyPolyInputWithConflatedStatusTest);
  CPPUNIT_TEST(poiToPolyPoiInputWithConflatedStatusTest);

  // POI
  CPPUNIT_TEST(poiMergeTwoPoisTest);
  CPPUNIT_TEST(poiMergeMoreThanTwoPoisTest);
  CPPUNIT_TEST(poiMergeTooFewPoisTest);
  CPPUNIT_TEST(poiMergeMissingTargetTagTest);
  CPPUNIT_TEST(poiMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(poiInputWithConflatedStatusTest);

  // AREA
  CPPUNIT_TEST(areaMergeTwoWaysTest);
  CPPUNIT_TEST(areaMergeTwoWaysNoConstituentsTest);
  CPPUNIT_TEST(areaMergeTwoRelationsTest);
  CPPUNIT_TEST(areaMergeTwoRelationsNoConstituentsTest);
  CPPUNIT_TEST(areaMergeOneWayOneRelationTargetAsWayTest);
  CPPUNIT_TEST(areaMergeOneWayOneRelationTargetAsWayNoConstituentsTest);
  CPPUNIT_TEST(areaMergeOneWayOneRelationTargetAsRelationTest);
  CPPUNIT_TEST(areaMergeOneWayOneRelationTargetAsRelationNoConstituentsTest);
  CPPUNIT_TEST(areaMergeMoreThanTwoAreasTest);
  CPPUNIT_TEST(areaMergeMoreThanTwoAreasNoConstituentsTest);
  CPPUNIT_TEST(areaMergeTooFewAreasTest);
  CPPUNIT_TEST(areaMergeMissingTargetTagTest);
  CPPUNIT_TEST(areaMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(areaInputWithConflatedStatusTest);

  // BUILDING
  CPPUNIT_TEST(buildingMergeTwoWaysTest);
  CPPUNIT_TEST(buildingMergeTwoWaysNoConstituentsTest);
  CPPUNIT_TEST(buildingMergeTwoRelationsTest);
  CPPUNIT_TEST(buildingMergeTwoRelationsNoConstituentsTest);
  CPPUNIT_TEST(buildingMergeOneWayOneRelationTargetAsWayTest);
  CPPUNIT_TEST(buildingMergeOneWayOneRelationTargetAsWayNoConstituentsTest);
  CPPUNIT_TEST(buildingMergeOneWayOneRelationTargetAsRelationTest);
  CPPUNIT_TEST(buildingMergeOneWayOneRelationTargetAsRelationNoConstituentsTest);
  CPPUNIT_TEST(buildingMergeMoreThanTwoBuildingsTest);
  CPPUNIT_TEST(buildingMergeMoreThanTwoBuildingsNoConstituentsTest);
  CPPUNIT_TEST(buildingMergeTooFewBuildingsTest);
  CPPUNIT_TEST(buildingMergeMissingTargetTagTest);
  CPPUNIT_TEST(buildingMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(buildingInputWithConflatedStatusTest);

  // RAILWAY
  CPPUNIT_TEST(railwayMergeTwoWaysTest);
  CPPUNIT_TEST(railwayMergeTooFewRailwaysTest);
  CPPUNIT_TEST(railwayMergeMissingTargetTagTest);
  CPPUNIT_TEST(railwayMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(railwayMergeExtraNonRailwayTest);
  CPPUNIT_TEST(railwayInputWithConflatedStatusTest);

  // RAILWAY ONE-TO-MANY
  CPPUNIT_TEST(railwayOneToManyMergeTwoWaysTest);
  CPPUNIT_TEST(railwayOneToManyMergeTooFewRailwaysTest);
  CPPUNIT_TEST(railwayOneToManyMergeMissingOneToManyFeatureTest);
  CPPUNIT_TEST(railwayOneToManyMergeDuplicateOneToManyFeatureTest);
  CPPUNIT_TEST(railwayOneToManyMergeExtraNonRailwayTest);
  CPPUNIT_TEST(railwayOneToManyInputWithConflatedStatusTest);

  CPPUNIT_TEST_SUITE_END();

public:

  ElementMergerJsTest() :
  HootTestFixture(
    "test-files/js/conflate/merging/ElementMergerJsTest/",
    "test-output/js/conflate/merging/ElementMergerJsTest/")
  {
    setResetType(ResetEnvironment);
  }

  // POI TO POLYGON

  void poiToPolyMergeWayAsPolyTest()
  {
    _testMerge("poi-poly-way-poly-in.osm", "poi-poly-way-poly-out.osm");
  }

  void poiToPolyMergeWayAsPolyNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "poi-poly-way-poly-no-constituents-in.osm", "poi-poly-way-poly-no-constituents-out.osm");
  }

  void poiToPolyMergeRelationAsPolyTest()
  {
    _testMerge("poi-poly-relation-poly-in.osm", "poi-poly-relation-poly-out.osm");
  }

  void poiToPolyMergeRelationAsPolyNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "poi-poly-relation-poly-no-constituents-in.osm",
      "poi-poly-relation-poly-no-constituents-out.osm");
  }

  void poiToPolyMergeMissingPoiInputTest()
  {
    _testMerge(
      "poi-poly-missing-poi-in.osm", "poi-poly-missing-poi-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void poiToPolyMergeMissingPolyInputTest()
  {
    _testMerge(
      "poi-poly-missing-poly-in.osm", "poi-poly-missing-poly-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void poiToPolyMergeMoreThanOnePolyInputTest()
  {
    _testMerge(
      "poi-poly-more-than-one-poly-in.osm", "poi-poly-more-than-one-poly-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void poiToPolyMergeMoreThanOnePoiInputTest()
  {
    _testMerge(
      "poi-poly-more-than-one-poi-in.osm", "poi-poly-more-than-one-poi-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void poiToPolyPolyInputWithConflatedStatusTest()
  {
    _testMerge("poi-poly-way-poly-conflated-1-in.osm", "poi-poly-poly-input-conflated-out.osm");
  }

  void poiToPolyPoiInputWithConflatedStatusTest()
  {
    _testMerge(
      "poi-poly-way-poly-conflated-2-in.osm", "poi-poly-poi-input-conflated-out.osm");
  }

  // POI

  void poiMergeTwoPoisTest()
  {
    _testMerge("poi-two-in.osm", "poi-two-out.osm");
  }

  void poiMergeMoreThanTwoPoisTest()
  {
    _testMerge("poi-more-than-two-in.osm", "poi-more-than-two-out.osm");
  }

  void poiMergeTooFewPoisTest()
  {
    _testMerge(
      "poi-too-few-in.osm", "poi-too-few-out.osm", "Invalid inputs passed to the element merger");
  }

  void poiMergeMissingTargetTagTest()
  {
    _testMerge(
      "poi-missing-target-tag-in.osm", "poi-missing-target-tag-out.osm",
      "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void poiMergeDuplicateTargetTagTest()
  {
    _testMerge(
      "poi-duplicate-target-tag-in.osm", "poi-duplicate-target-tag-out.osm",
      "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void poiInputWithConflatedStatusTest()
  {
    _testMerge("poi-two-conflated-in.osm", "poi-two-out.osm");
  }

  // AREA

  void areaMergeTwoWaysTest()
  {
    _testMerge("area-two-ways-in.osm", "area-two-ways-out.osm");
  }

  void areaMergeTwoWaysNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge("area-two-ways-no-constituents-in.osm", "area-two-ways-no-constituents-out.osm");
  }

  void areaMergeTwoRelationsTest()
  {
    _testMerge("area-two-relations-in.osm", "area-two-relations-out.osm");
  }

  void areaMergeTwoRelationsNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "area-two-relations-no-constituents-in.osm", "area-two-relations-no-constituents-out.osm");
  }

  void areaMergeOneWayOneRelationTargetAsWayTest()
  {
    _testMerge(
      "area-one-way-one-relation-target-as-way-in.osm",
      "area-one-way-one-relation-target-as-way-out.osm");
  }

  void areaMergeOneWayOneRelationTargetAsWayNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "area-one-way-one-relation-target-as-way-no-constituents-in.osm",
      "area-one-way-one-relation-target-as-way-no-constituents-out.osm");
  }

  void areaMergeOneWayOneRelationTargetAsRelationTest()
  {
    _testMerge(
      "area-one-way-one-relation-target-as-relation-in.osm",
      "area-one-way-one-relation-target-as-relation-out.osm");
  }

  void areaMergeOneWayOneRelationTargetAsRelationNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "area-one-way-one-relation-target-as-relation-no-constituents-in.osm",
      "area-one-way-one-relation-target-as-relation-no-constituents-out.osm");
  }

  void areaMergeMoreThanTwoAreasTest()
  {
    _testMerge("area-more-than-two-in.osm", "area-more-than-two-out.osm");
  }

  void areaMergeMoreThanTwoAreasNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "area-more-than-two-no-constituents-in.osm", "area-more-than-two-no-constituents-out.osm");
  }

  void areaMergeTooFewAreasTest()
  {
    _testMerge(
      "area-too-few-in.osm", "area-too-few-out.osm", "Invalid inputs passed to the element merger");
  }

  void areaMergeMissingTargetTagTest()
  {
    _testMerge(
      "area-missing-target-tag-in.osm", "area-missing-target-tag-out.osm",
      "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void areaMergeDuplicateTargetTagTest()
  {
    _testMerge(
      "area-duplicate-target-tag-in.osm", "area-duplicate-target-tag-out.osm",
      "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void areaInputWithConflatedStatusTest()
  {
    _testMerge("area-two-ways-conflated-in.osm", "area-two-ways-out.osm");
  }

  // BUILDING

  void buildingMergeTwoWaysTest()
  {
    _testMerge("building-two-ways-in.osm", "building-two-ways-out.osm");
  }

  void buildingMergeTwoWaysNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "building-two-ways-no-constituents-in.osm", "building-two-ways-no-constituents-out.osm");
  }

  void buildingMergeTwoRelationsTest()
  {
    _testMerge("building-two-relations-in.osm", "building-two-relations-out.osm");
  }

  void buildingMergeTwoRelationsNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "building-two-relations-no-constituents-in.osm",
      "building-two-relations-no-constituents-out.osm");
  }

  void buildingMergeOneWayOneRelationTargetAsWayTest()
  {
    _testMerge(
      "building-one-way-one-relation-target-as-way-in.osm",
      "building-one-way-one-relation-target-as-way-out.osm");
  }

  void buildingMergeOneWayOneRelationTargetAsWayNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "building-one-way-one-relation-target-as-way-no-constituents-in.osm",
      "building-one-way-one-relation-target-as-way-no-constituents-out.osm");
  }

  void buildingMergeOneWayOneRelationTargetAsRelationTest()
  {
    _testMerge(
      "building-one-way-one-relation-target-as-relation-in.osm",
      "building-one-way-one-relation-target-as-relation-out.osm");
  }

  void buildingMergeOneWayOneRelationTargetAsRelationNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "building-one-way-one-relation-target-as-relation-no-constituents-in.osm",
      "building-one-way-one-relation-target-as-relation-no-constituents-out.osm");
  }

  void buildingMergeMoreThanTwoBuildingsTest()
  {
    _testMerge("building-more-than-two-in.osm", "building-more-than-two-out.osm");
  }

  void buildingMergeMoreThanTwoBuildingsNoConstituentsTest()
  {
    DisableLog dl;
    _testMerge(
      "building-more-than-two-no-constituents-in.osm",
      "building-more-than-two-no-constituents-out.osm");
  }

  void buildingMergeTooFewBuildingsTest()
  {
    _testMerge(
      "building-too-few-in.osm", "building-too-few-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void buildingMergeMissingTargetTagTest()
  {
      _testMerge(
        "building-missing-target-tag-in.osm", "building-missing-target-tag-out.osm",
        "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void buildingMergeDuplicateTargetTagTest()
  {
      _testMerge(
        "building-missing-target-tag-in.osm", "building-missing-target-tag-out.osm",
        "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void buildingInputWithConflatedStatusTest()
  {
    _testMerge("building-two-ways-conflated-in.osm", "building-two-ways-out.osm");
  }

  // RAILWAY

  void railwayMergeTwoWaysTest()
  {
    _disableOneToManyRailwayConflation();
    _testMerge("railway-two-in.osm", "railway-two-out.osm");
  }

  void railwayMergeTooFewRailwaysTest()
  {
    _disableOneToManyRailwayConflation();
    _testMerge(
      "railway-too-few-in.osm", "railway-too-few-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void railwayMergeMissingTargetTagTest()
  {
    _disableOneToManyRailwayConflation();
    _testMerge(
      "railway-missing-target-tag-in.osm", "railway-missing-target-tag-out.osm",
      "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void railwayMergeDuplicateTargetTagTest()
  {
    _disableOneToManyRailwayConflation();
    _testMerge(
      "railway-duplicate-target-tag-in.osm", "railway-duplicate-target-tag-out.osm",
      "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void railwayMergeExtraNonRailwayTest()
  {
    _disableOneToManyRailwayConflation();
    _testMerge("railway-extra-non-railway-in.osm", "railway-extra-non-railway-out.osm",
    "Invalid inputs passed to the element merger");
  }

  void railwayInputWithConflatedStatusTest()
  {
    _disableOneToManyRailwayConflation();
    _testMerge("railway-two-conflated-in.osm", "railway-two-conflated-out.osm");
  }

  // RAILWAY ONE-TO-MANY

  void railwayOneToManyMergeTwoWaysTest()
  {
    _enableOneToManyRailwayConflation();
    _testMerge("railway-one-to-many-two-in.osm", "railway-one-to-many-two-out.osm");
  }

  void railwayOneToManyMergeTooFewRailwaysTest()
  {
    _enableOneToManyRailwayConflation();
    _testMerge(
      "railway-one-to-many-too-few-in.osm", "railway-one-to-many-too-few-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void railwayOneToManyMergeMissingOneToManyFeatureTest()
  {
    _enableOneToManyRailwayConflation();
    _testMerge(
      "railway-one-to-many-missing-one-to-many-feature-in.osm",
      "railway-one-to-many-missing-one-to-many-feature-out.osm",
      "Input map must have exactly one feature marked with a hoot:merge:target tag");
  }

  void railwayOneToManyMergeDuplicateOneToManyFeatureTest()
  {
    _enableOneToManyRailwayConflation();
    _testMerge(
      "railway-one-to-many-duplicate-one-to-many-feature-in.osm",
      "railway-one-to-many-duplicate-one-to-many-feature-out.osm",
      "Input map for railway one-to-many merging must have exactly one qualifying "
      "secondary feature.");
  }

  void railwayOneToManyMergeExtraNonRailwayTest()
  {
    _enableOneToManyRailwayConflation();
    _testMerge(
      "railway-one-to-many-extra-non-railway-in.osm",
      "railway-one-to-many-extra-non-railway-out.osm",
      "Invalid inputs passed to the element merger");
  }

  void railwayOneToManyInputWithConflatedStatusTest()
  {
    _enableOneToManyRailwayConflation();
    _testMerge(
      "railway-one-to-many-two-conflated-in.osm", "railway-one-to-many-two-conflated-out.osm");
  }

private:

  void _enableOneToManyRailwayConflation() const
  {
    conf().set(ConfigOptions::getRailwayOneToManyMatchKey(), true);
    conf().set(ConfigOptions::getRailwayOneToManyIdentifyingKeysKey(), "lines;lanes");
    conf().set(ConfigOptions::getRailwayOneToManyTransferKeysKey(), "name;arrangement");
  }

  void _disableOneToManyRailwayConflation() const
  {
    conf().set(ConfigOptions::getRailwayOneToManyMatchKey(), false);
    conf().set(ConfigOptions::getRailwayOneToManyIdentifyingKeysKey(), "");
    conf().set(ConfigOptions::getRailwayOneToManyTransferKeysKey(), "");
  }

  void _testMerge(
    const QString& inFileName, const QString& outFileName,
    const QString& expectedExceptionMsgContains = "") const
  {
    // Poi/poly test output is based on this config for now, despite it being different than the
    // default config.
    conf().set(ConfigOptions::getAddressMatchEnabledKey(), "false");
    conf().set(ConfigOptions::getPoiPolygonTagMergerKey(), "OverwriteTag2Merger");

    LOG_VART(inFileName);
    LOG_VART(outFileName);
    LOG_VART(expectedExceptionMsgContains);

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, true, true, _inputPath + inFileName);

    QString exceptionMsg("");
    try
    {
      ElementMergerJs::_merge(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);

    if (!expectedExceptionMsgContains.isEmpty())
    {
      if (exceptionMsg.isEmpty())
      {
        throw HootException(
          "Expected exception substring: " + expectedExceptionMsgContains + "; Actual exception: " +
          exceptionMsg);
      }
      else
      {
        CPPUNIT_ASSERT(exceptionMsg.contains(expectedExceptionMsgContains));
      }
    }
    else
    {
      MapProjector::projectToWgs84(map);
      OsmMapWriterFactory::write(map, _outputPath + outFileName);

      HOOT_FILE_EQUALS(_inputPath + outFileName, _outputPath + outFileName);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementMergerJsTest, "quick");

}
