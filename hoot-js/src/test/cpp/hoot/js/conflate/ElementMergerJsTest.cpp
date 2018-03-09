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

class ElementMergerJsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ElementMergerJsTest);
  CPPUNIT_TEST(mergeWayAsPolyTest);
//  CPPUNIT_TEST(mergeRelationAsPolyTest);
//  CPPUNIT_TEST(mergeMissingPoiInputTest);
//  CPPUNIT_TEST(mergeMissingPolyInputTest);
//  CPPUNIT_TEST(mergeMoreThanOnePoiInputTest);
//  CPPUNIT_TEST(mergeMoreThanOnePolyInputTest);
//  CPPUNIT_TEST(mergePoiPolyMissingTargetTagTest);
//  CPPUNIT_TEST(mergeTwoWayAreasTest);
//  CPPUNIT_TEST(mergeTwoRelationAreasTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsWayAreaTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsRelationAreaTest);
//  CPPUNIT_TEST(mergeTooManyAreasTest);
//  CPPUNIT_TEST(mergeTooFewAreasTest);
//  CPPUNIT_TEST(mergeAreaMissingTargetTagTest);
//  CPPUNIT_TEST(mergeTwoWayBuildingsTest);
//  CPPUNIT_TEST(mergeTwoRelationBuildingsTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsWayBuildingTest);
//  CPPUNIT_TEST(mergeOneWayOneRelationTargetAsRelationBuildingTest);
//  CPPUNIT_TEST(mergeTooManyBuildingsTest);
//  CPPUNIT_TEST(mergeTooFewBuildingsTest);
//  CPPUNIT_TEST(mergeBuildingMissingTargetTagTest);
//  CPPUNIT_TEST(mergePoisTest);
//  CPPUNIT_TEST(mergeTooManyPoisTest);
//  CPPUNIT_TEST(mergeTooFewPoisTest);
//  CPPUNIT_TEST(mergeBuildingMissingTargetTagTest);
//  CPPUNIT_TEST(mergeInvalidFeatureInputsComboTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/js/conflate/ElementMergerJsTest");
  }

  //The only thing we're not testing here is the actual conversion of the map arg from js.  That
  //gets tested in the mocha plugin test.

  void mergeWayAsPolyTest()
  {
    Isolate* current = v8::Isolate::GetCurrent();
    //HandleScope handleScope(current);
    //Local<Context> context = Context::New(current);
    //Context::Scope context_scope(context);

    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map,
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-way-poly-in.osm",
      false,
      Status::Unknown1);

    ElementMergerJs::_mergeElements(map, current);

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-way-poly-out.osm");

    HOOT_FILE_EQUALS(
      "test-files/js/conflate/ElementMergerJsTest/poi-poly-way-poly-out.osm",
      "test-output/js/conflate/ElementMergerJsTest/poi-poly-way-poly-out.osm");
  }

//  void mergeRelationAsPolyTest()
//  {
//    OsmMap::resetCounters();
//    OsmMapPtr map(new OsmMap());
//    //see comments in mergeWayAsPolyTest
//    OsmMapReaderFactory::read(
//      map, "test-files/conflate/poi-polygon/poi-poly-relation-poly-in.osm", false,
//      Status::Unknown1);

//    const ElementId polyId = PoiPolygonMerger::merge(map);
//    CPPUNIT_ASSERT_EQUAL((long)-1, polyId.getId());
//    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, polyId.getType().getEnum());

//    MapProjector::projectToWgs84(map);
//    OsmMapWriterFactory::getInstance().write(map,
//      "test-output/conflate/poi-polygon/poi-poly-relation-poly-out.osm");

//    HOOT_FILE_EQUALS(
//      "test-files/conflate/poi-polygon/poi-poly-relation-poly-out.osm",
//      "test-output/conflate/poi-polygon/poi-poly-relation-poly-out.osm");
//  }

//  void mergeMissingPoiInputTest()
//  {
//    QString exceptionMsg("");
//    try
//    {
//      OsmMapPtr map(new OsmMap());
//      OsmMapReaderFactory::read(
//        map, "test-files/conflate/poi-polygon/poi-poly-missing-poi-in.osm", false,
//        Status::Unknown1);

//      PoiPolygonMerger::merge(map);
//    }
//    catch (const HootException& e)
//    {
//      exceptionMsg = e.what();
//    }
//    HOOT_STR_EQUALS("No POI passed to POI/Polygon merger.", exceptionMsg.toStdString());
//  }

//  void mergeMissingPolyInputTest()
//  {
//    QString exceptionMsg("");
//    try
//    {
//      OsmMapPtr map(new OsmMap());
//      OsmMapReaderFactory::read(
//        map, "test-files/conflate/poi-polygon/poi-poly-missing-poly-in.osm", false,
//        Status::Unknown1);

//      PoiPolygonMerger::merge(map);
//    }
//    catch (const HootException& e)
//    {
//      exceptionMsg = e.what();
//    }
//    HOOT_STR_EQUALS("No polygon passed to POI/Polygon merger.", exceptionMsg.toStdString());
//  }

//  void mergeMoreThanOnePoiInputTest()
//  {
//    QString exceptionMsg("");
//    try
//    {
//      OsmMapPtr map(new OsmMap());
//      OsmMapReaderFactory::read(
//        map, "test-files/conflate/poi-polygon/poi-poly-more-than-one-poi-in.osm", false,
//        Status::Unknown1);

//      PoiPolygonMerger::merge(map);
//    }
//    catch (const HootException& e)
//    {
//      exceptionMsg = e.what();
//    }
//    HOOT_STR_EQUALS(
//      "More than one POI passed to POI/Polygon merger.", exceptionMsg.toStdString());
//  }

//  void mergeMoreThanOnePolyInputTest()
//  {
//    QString exceptionMsg("");
//    try
//    {
//      OsmMapPtr map(new OsmMap());
//      OsmMapReaderFactory::read(
//        map, "test-files/conflate/poi-polygon/poi-poly-more-than-one-poly-in.osm", false,
//        Status::Unknown1);

//      PoiPolygonMerger::merge(map);
//    }
//    catch (const HootException& e)
//    {
//      exceptionMsg = e.what();
//    }
//    HOOT_STR_EQUALS(
//      "More than one polygon passed to POI/Polygon merger.", exceptionMsg.toStdString());
//  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ElementMergerJsTest, "quick");

}
