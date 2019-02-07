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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/ReviewRelationCriterion.h>

namespace hoot
{

class RemoveElementsVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveElementsVisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runRecursiveTest);
  CPPUNIT_TEST(runNegatedFilterTest);
  CPPUNIT_TEST(runReviewRelationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveElementsVisitorTest()
  {
    TestUtils::mkpath("test-output/visitors");
  }

  void runTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveElementsVisitorInput.osm");

    boost::shared_ptr<PoiCriterion> elementCriterion(new PoiCriterion());
    RemoveElementsVisitor removeElementsVisitor(elementCriterion);
    removeElementsVisitor.setRecursive(false);
    removeElementsVisitor.setNegateCriterion(false);
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, "test-output/visitors/RemoveElementsVisitorOutput.osm");
    HOOT_FILE_EQUALS("test-files/visitors/RemoveElementsVisitorOutput.osm",
                     "test-output/visitors/RemoveElementsVisitorOutput.osm");
  }

  void runRecursiveTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveElementsVisitorInput.osm");

    boost::shared_ptr<HighwayCriterion> elementCriterion(new HighwayCriterion());
    RemoveElementsVisitor removeElementsVisitor(elementCriterion);
    removeElementsVisitor.setRecursive(true);
    removeElementsVisitor.setNegateCriterion(false);
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, "test-output/visitors/RemoveElementsVisitorRecursiveOutput.osm");
    HOOT_FILE_EQUALS("test-files/visitors/RemoveElementsVisitorRecursiveOutput.osm",
                     "test-output/visitors/RemoveElementsVisitorRecursiveOutput.osm");
  }

  void runNegatedFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveElementsVisitorInput.osm");

    boost::shared_ptr<PoiCriterion> elementCriterion(new PoiCriterion());
    RemoveElementsVisitor removeElementsVisitor(elementCriterion, true);
    removeElementsVisitor.setRecursive(false);
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, "test-output/visitors/RemoveElementsVisitorNegatedFilterOutput.osm");
    HOOT_FILE_EQUALS("test-files/visitors/RemoveElementsVisitorNegatedFilterOutput.osm",
                     "test-output/visitors/RemoveElementsVisitorNegatedFilterOutput.osm");
  }

  void runReviewRelationTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveElementsVisitorTest-reviewRelationTest.osm");

    boost::shared_ptr<ReviewRelationCriterion> elementCriterion(new ReviewRelationCriterion());
    RemoveElementsVisitor removeElementsVisitor(elementCriterion);
    removeElementsVisitor.setRecursive(false);
    removeElementsVisitor.setNegateCriterion(false);
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, "test-output/visitors/RemoveElementsVisitorTest-reviewRelationTestOut.osm");
    HOOT_FILE_EQUALS("test-files/visitors/RemoveElementsVisitorTest-reviewRelationTestOut.osm",
                     "test-output/visitors/RemoveElementsVisitorTest-reviewRelationTestOut.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveElementsVisitorTest, "quick");

}


