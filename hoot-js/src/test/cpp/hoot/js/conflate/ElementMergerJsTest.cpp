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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/conflate/ElementMergerJs.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/MapProjector.h>

// Std
#include <string>

using namespace v8;

namespace hoot
{

/**
 * Tests merging two or more features
 *
 * The only thing we're not testing here in the merging of features through the js bindings workflow
 * is the conversion of the map arg from a js object to a hoot object.  That gets tested by the
 * element merge service mocha plugin test.
 *
 *  See notes in ElementMergerJs about features passed in without constituents.
 */
class ElementMergerJsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ElementMergerJsTest);
  CPPUNIT_TEST(poiToPolyMergeWayAsPolyTest);
  CPPUNIT_TEST(poiToPolyMergeWayAsPolyNoConstituentsTest);
  CPPUNIT_TEST(poiToPolyMergeRelationAsPolyTest);
  CPPUNIT_TEST(poiToPolyMergeRelationAsPolyNoConstituentsTest);
  CPPUNIT_TEST(poiToPolyMergeMissingPoiInputTest);
  CPPUNIT_TEST(poiToPolyMergeMissingPolyInputTest);
  CPPUNIT_TEST(poiToPolyMergeMoreThanOnePolyInputTest);
  CPPUNIT_TEST(poiToPolyMergeMoreThanOnePoiInputTest);
  CPPUNIT_TEST(poiToPolyMergeMissingTargetTagTest);
  CPPUNIT_TEST(poiToPolyMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(poiToPolyMergeExtraNonPolyWayTest);
  CPPUNIT_TEST(poiToPolyMergeExtraNonPolyRelationTest);
  CPPUNIT_TEST(poiToPolyMergeExtraNonPoiNodeTest);
  CPPUNIT_TEST(poiToPoiMergeTwoPoisTest);
  CPPUNIT_TEST(poiToPoiMergeMoreThanTwoPoisTest);
  CPPUNIT_TEST(poiToPoiMergeTooFewPoisTest);
  CPPUNIT_TEST(poiToPoiMergeMissingTargetTagTest);
  CPPUNIT_TEST(poiToPoiMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(poiToPoiMergeExtraNonPoiNodeTest);
  CPPUNIT_TEST(areaToAreaMergeTwoWaysTest);
  CPPUNIT_TEST(areaToAreaMergeTwoWaysNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeTwoRelationsTest);
  CPPUNIT_TEST(areaToAreaMergeTwoRelationsNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeOneWayOneRelationTargetAsWayTest);
  //TODO: fix
  CPPUNIT_TEST(areaToAreaMergeOneWayOneRelationTargetAsWayNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeOneWayOneRelationTargetAsRelationTest);
  //TODO: fix
  CPPUNIT_TEST(areaToAreaMergeOneWayOneRelationTargetAsRelationNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeMoreThanTwoAreasTest);
  CPPUNIT_TEST(areaToAreaMergeMoreThanTwoAreasNoConstituentsTest);
  CPPUNIT_TEST(areaToAreaMergeTooFewAreasTest);
  CPPUNIT_TEST(areaToAreaMergeMissingTargetTagTest);
  CPPUNIT_TEST(areaToAreaMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(areaToAreaMergeExtraNonAreaWayTest);
  CPPUNIT_TEST(areaToAreaMergeExtraNonAreaRelationTest);
//  CPPUNIT_TEST(buildingToBuildingMergeTwoWaysNoConstituentsTest);
//  CPPUNIT_TEST(buildingToBuildingMergeTwoWaysTest);
//  CPPUNIT_TEST(buildingToBuildingMergeTwoRelationsTest);
//  CPPUNIT_TEST(buildingToBuildingMergeTwoRelationsNoConstituentsTest);
//  CPPUNIT_TEST(buildingToBuildingMergeOneWayOneRelationTargetAsWayTest);
//  CPPUNIT_TEST(buildingToBuildingMergeOneWayOneRelationTargetAsWayNoConstituentsTest);
//  CPPUNIT_TEST(buildingToBuildingMergeOneWayOneRelationTargetAsRelationTest);
//  CPPUNIT_TEST(buildingToBuildingMergeOneWayOneRelationTargetAsRelationNoConstituentsTest);
//  CPPUNIT_TEST(buildingToBuildingMergeMoreThanTwoBuildingsTest);
//  CPPUNIT_TEST(buildingToBuildingMergeTooFewBuildingsTest);
//  CPPUNIT_TEST(buildingToBuildingMergeMissingTargetTagTest);
//  CPPUNIT_TEST(buildingToBuildingMergeDuplicateTargetTagTest);
//  CPPUNIT_TEST(buildingToBuildingMergeExtraNonBuildingWayTest);
//  CPPUNIT_TEST(buildingToBuildingMergeExtraNonBuildingRelationTest);
  //CPPUNIT_TEST(invalidFeatureCombinationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/js/conflate/ElementMergerJsTest");
  }

  //POI TO POLYGON

  void poiToPolyMergeWayAsPolyTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-way-poly-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-way-poly-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-way-poly-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-way-poly-out.osm");
  }

  void poiToPolyMergeWayAsPolyNoConstituentsTest()
  {
    //Incomplete maps are being sent in, so this is needed to suppress the warnings in the test.
    DisableLog dl;

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-way-poly-no-constituents-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-way-poly-no-constituents-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-way-poly-no-constituents-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-way-poly-no-constituents-out.osm");
  }

  void poiToPolyMergeRelationAsPolyTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());

    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-relation-poly-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-relation-poly-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-relation-poly-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-relation-poly-out.osm");
  }

  void poiToPolyMergeRelationAsPolyNoConstituentsTest()
  {
    //Incomplete maps are being sent in, so this is needed to suppress the warnings in the test.
    DisableLog dl;

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    //see comments in mergeWayAsPolyTest
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-relation-poly-no-constituents-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-relation-poly-no-constituents-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-relation-poly-no-constituents-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-relation-poly-no-constituents-out.osm");
  }

  void poiToPolyMergeMissingPoiInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-poly-missing-poi-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid inputs to element merger"));
  }

  void poiToPolyMergeMissingPolyInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-poly-missing-poly-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid inputs to element merger"));
  }

  void poiToPolyMergeMoreThanOnePolyInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-poly-more-than-one-poly-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid inputs to element merger"));
  }

  void poiToPolyMergeMoreThanOnePoiInputTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-poly-more-than-one-poi-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid inputs to element merger"));
  }

  void poiToPolyMergeMissingTargetTagTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-poly-missing-target-tag-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Input map must have one feature marked with a hoot:merge:target tag.",
      exceptionMsg.toStdString());
  }

  void poiToPolyMergeDuplicateTargetTagTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-poly-duplicate-target-tag-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Input map must have one feature marked with a hoot:merge:target tag.",
      exceptionMsg.toStdString());
  }

  void poiToPolyMergeExtraNonPolyWayTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poly-way-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poly-way-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poly-way-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poly-way-out.osm");
  }

  void poiToPolyMergeExtraNonPolyRelationTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poly-relation-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poly-relation-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poly-relation-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poly-relation-out.osm");
  }

  void poiToPolyMergeExtraNonPoiNodeTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poi-node-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poi-node-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poi-node-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-extra-non-poi-node-out.osm");
  }

  //POI TO POI

  void poiToPoiMergeTwoPoisTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-two-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-two-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-two-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-two-out.osm");
  }

  void poiToPoiMergeMoreThanTwoPoisTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-more-than-two-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-more-than-two-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-more-than-two-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-more-than-two-out.osm");
  }

  void poiToPoiMergeTooFewPoisTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-too-few-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid inputs to element merger"));
  }

  void poiToPoiMergeMissingTargetTagTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-missing-target-tag-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Input map must have one feature marked with a hoot:merge:target tag.",
      exceptionMsg.toStdString());
  }

  void poiToPoiMergeDuplicateTargetTagTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/poi-duplicate-target-tag-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Input map must have one feature marked with a hoot:merge:target tag.",
      exceptionMsg.toStdString());
  }

  void poiToPoiMergeExtraNonPoiNodeTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-extra-non-poi-node-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-extra-non-poi-node-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-extra-non-poi-node-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-extra-non-poi-node-out.osm");
  }

  //AREA TO AREA

  void areaToAreaMergeTwoWaysTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-two-ways-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-two-ways-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-two-ways-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-two-ways-out.osm");
  }

  void areaToAreaMergeTwoWaysNoConstituentsTest()
  {
    //Incomplete maps are being sent in, so this is needed to suppress the warnings in the test.
    DisableLog dl;

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-two-ways-no-constituents-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-two-ways-no-constituents-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-two-ways-no-constituents-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-two-ways-no-constituents-out.osm");
  }

  void areaToAreaMergeTwoRelationsTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-two-relations-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-two-relations-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-two-relations-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-two-relations-out.osm");
  }

  void areaToAreaMergeTwoRelationsNoConstituentsTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-two-relations-no-constituents-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-two-relations-no-constituents-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-two-relations-no-constituents-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-two-relations-no-constituents-out.osm");
  }

  void areaToAreaMergeOneWayOneRelationTargetAsWayTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-way-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-way-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-way-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-way-out.osm");
  }

  //X
  void areaToAreaMergeOneWayOneRelationTargetAsWayNoConstituentsTest()
  {
    //Incomplete maps are being sent in, so this is needed to suppress the warnings in the test.
    //DisableLog dl;

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-way-no-constituents-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-way-no-constituents-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-way-no-constituents-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-way-no-constituents-out.osm");
  }

  void areaToAreaMergeOneWayOneRelationTargetAsRelationTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-relation-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-relation-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-relation-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-relation-out.osm");
  }

  //X
  void areaToAreaMergeOneWayOneRelationTargetAsRelationNoConstituentsTest()
  {
    //Incomplete maps are being sent in, so this is needed to suppress the warnings in the test.
    DisableLog dl;

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-relation-no-constituents-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-relation-no-constituents-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-relation-no-constituents-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-one-way-one-relation-target-as-relation-no-constituents-out.osm");
  }

  void areaToAreaMergeMoreThanTwoAreasTest()
  {
      OsmMap::resetCounters();
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/area-more-than-two-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

      MapProjector::projectToWgs84(map);
      OsmMapWriterFactory::getInstance().write(map,
        "test-output/js/conflate/ElementMergerJsTest/area-more-than-two-out.osm");

      HOOT_FILE_EQUALS(
        "test-files/js/conflate/ElementMergerJsTest/area-more-than-two-out.osm",
        "test-output/js/conflate/ElementMergerJsTest/area-more-than-two-out.osm");
  }

  void areaToAreaMergeMoreThanTwoAreasNoConstituentsTest()
  {
    //Incomplete maps are being sent in, so this is needed to suppress the warnings in the test.
    DisableLog dl;

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-more-than-two-no-constituents-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-more-than-two-no-constituents-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-more-than-two-no-constituents-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-more-than-two-no-constituents-out.osm");
  }

  void areaToAreaMergeTooFewAreasTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/area-too-few-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid inputs to element merger"));
  }

  void areaToAreaMergeMissingTargetTagTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/area-missing-target-tag-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Input map must have one feature marked with a hoot:merge:target tag.",
      exceptionMsg.toStdString());
  }

  void areaToAreaMergeDuplicateTargetTagTest()
  {
    QString exceptionMsg("");
    try
    {
      OsmMapPtr map(new OsmMap());
      OsmMapReaderFactory::read(
        map,
        "test-files/js/conflate/ElementMergerJsTest/area-duplicate-target-tag-in.osm",
        false,
        Status::Unknown1);

      ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(
      "Input map must have one feature marked with a hoot:merge:target tag.",
      exceptionMsg.toStdString());
  }

  void areaToAreaMergeExtraNonAreaWayTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-extra-non-area-way-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-extra-non-area-way-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-extra-non-area-way-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-extra-non-area-way-out.osm");
}

  void areaToAreaMergeExtraNonAreaRelationTest()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/area-extra-non-area-relation-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, v8::Isolate::GetCurrent());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/area-extra-non-area-relation-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/area-extra-non-area-relation-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/area-extra-non-area-relation-out.osm");
  }

  //BUILDING TO BUILDING

//  void buildingToBuildingMergeTwoWaysNoConstituentsTest()
//  {

//  }

//  void buildingToBuildingMergeTwoWaysTest()
//  {

//  }

//  void buildingToBuildingMergeTwoRelationsTest()
//  {

//  }

//  void buildingToBuildingMergeTwoRelationsNoConstituentsTest()
//  {

//  }

//  void buildingToBuildingMergeOneWayOneRelationTargetAsWayTest()
//  {

//  }

//  void buildingToBuildingMergeOneWayOneRelationTargetAsWayNoConstituentsTest()
//  {

//  }

//  void buildingToBuildingMergeOneWayOneRelationTargetAsRelationTest()
//  {

//  }

//  void buildingToBuildingMergeOneWayOneRelationTargetAsRelationNoConstituentsTest()
//  {

//  }

//  void buildingToBuildingMergeMoreThanTwoBuildingsTest()
//  {

//  }

//  void buildingToBuildingMergeTooFewBuildingsTest()
//  {

//  }

//  void buildingToBuildingMergeMissingTargetTagTest()
//  {

//  }

//  void buildingToBuildingMergeDuplicateTargetTagTest()
//  {

//  }

//  void buildingToBuildingMergeExtraNonBuildingWayTest()
//  {

//  }

//  void buildingToBuildingMergeExtraNonBuildingRelationTest()
//  {

//  }

  //COMBO

//  void invalidFeatureCombinationTest()
//  {

//  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementMergerJsTest, "quick");

}
