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
 * The only thing we're not testing here is the conversion of the map arg from a js object to a
   hoot object.  That gets tested by the mocha plugin test.

   See notes in ElementMergerJs about features passed in without constituents.
 */
class ElementMergerJsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ElementMergerJsTest);
  CPPUNIT_TEST(poiPolyMergeWayAsPolyTest);
  CPPUNIT_TEST(poiPolyMergeWayAsPolyNoConstituentsTest);
  CPPUNIT_TEST(poiPolyMergeRelationAsPolyTest);
  CPPUNIT_TEST(poiPolyMergeRelationAsPolyNoConstituentsTest);
  CPPUNIT_TEST(poiPolyMergeMissingPoiInputTest);
  CPPUNIT_TEST(poiPolyMergeMissingPolyInputTest);
  CPPUNIT_TEST(poiPolyMergeMoreThanOnePolyInputTest);
  CPPUNIT_TEST(poiPolyMergeMoreThanOnePoiInputTest);
  CPPUNIT_TEST(poiPolyMergeMissingTargetTagTest);
  CPPUNIT_TEST(poiPolyMergeDuplicateTargetTagTest);
  CPPUNIT_TEST(poiPolyMergeExtraNonPolyWayTest);
  CPPUNIT_TEST(poiPolyMergeExtraNonPolyRelationTest);
  CPPUNIT_TEST(poiPolyMergeExtraNonPoiNodeTest);
  CPPUNIT_TEST(mergeTwoPoisTest);
  CPPUNIT_TEST(mergeMoreThanTwoPoisTest);
  CPPUNIT_TEST(mergeTooFewPoisTest);
  CPPUNIT_TEST(mergePoiMissingTargetTagTest);
  CPPUNIT_TEST(mergePoiDuplicateTargetTagTest);
  CPPUNIT_TEST(mergePoiExtraNonPoiNodeTest);
//  CPPUNIT_TEST(mergeTwoWayAreasTest);
//  CPPUNIT_TEST(mergeTwoWayAreasNoConstituentsTest);
//  CPPUNIT_TEST(mergeTwoRelationAreasTest);
//  CPPUNIT_TEST(mergeTwoRelationAreasNoConstituentsTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsWayAreaTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsWayAreaNoConstituentsTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsRelationAreaTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsRelationAreaNoConstituentsTest);
//  CPPUNIT_TEST(mergeMoreThanTwoAreasTest);
//  CPPUNIT_TEST(mergeMoreThanTwoAreasNoConstituentsTest);
//  CPPUNIT_TEST(mergeTooFewAreasTest);
//  CPPUNIT_TEST(mergeAreaMissingTargetTagTest);
//  CPPUNIT_TEST(mergeAreaDuplicateTargetTagTest);
//  CPPUNIT_TEST(mergeExtraNonAreaWayTest);
//  CPPUNIT_TEST(mergeExtraNonAreaRelationTest);
//  CPPUNIT_TEST(mergeTwoWayBuildingsNoConstituentsTest);
//  CPPUNIT_TEST(mergeTwoWayBuildingsTest);
//  CPPUNIT_TEST(mergeTwoRelationBuildingsTest);
//  CPPUNIT_TEST(mergeTwoRelationBuildingsNoConstituentsTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsWayBuildingTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsWayBuildingNoConstituentsTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsRelationBuildingTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsRelationBuildingNoConstituentsTest);
//  CPPUNIT_TEST(mergeMoreThanTwoBuildingsTest);
//  CPPUNIT_TEST(mergeTooFewBuildingsTest);
//  CPPUNIT_TEST(mergeBuildingMissingTargetTagTest);
//  CPPUNIT_TEST(mergeBuildingDuplicateTargetTagTest);
//  CPPUNIT_TEST(mergeExtraNonBuildingWayTest);
//  CPPUNIT_TEST(mergeExtraNonBuildingRelationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/js/conflate/ElementMergerJsTest");
  }

  //POI TO POLYGON

  void poiPolyMergeWayAsPolyTest()
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

  void poiPolyMergeWayAsPolyNoConstituentsTest()
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

  void poiPolyMergeRelationAsPolyTest()
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

  void poiPolyMergeRelationAsPolyNoConstituentsTest()
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

  void poiPolyMergeMissingPoiInputTest()
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

  void poiPolyMergeMissingPolyInputTest()
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

  void poiPolyMergeMoreThanOnePolyInputTest()
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

  void poiPolyMergeMoreThanOnePoiInputTest()
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

  void poiPolyMergeMissingTargetTagTest()
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

  void poiPolyMergeDuplicateTargetTagTest()
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

  void poiPolyMergeExtraNonPolyWayTest()
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

  void poiPolyMergeExtraNonPolyRelationTest()
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

  void poiPolyMergeExtraNonPoiNodeTest()
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

  void mergeTwoPoisTest()
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

  void mergeMoreThanTwoPoisTest()
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

  void mergeTooFewPoisTest()
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

  void mergePoiMissingTargetTagTest()
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

  void mergePoiDuplicateTargetTagTest()
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

  void mergePoiExtraNonPoiNodeTest()
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



  //BUILDING TO BUILDING


};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementMergerJsTest, "quick");

}
