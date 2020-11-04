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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/elements/MapProjector.h>
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
  CPPUNIT_TEST(runMultipleCriteriaTest);
  // TODO (will need to modify the input file):
  //CPPUNIT_TEST(runChainCriteriaTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveElementsVisitorTest()
    : HootTestFixture("test-files/visitors/RemoveElementsVisitorTest/",
                      "test-output/visitors/RemoveElementsVisitorTest/")
  {
  }

  void runTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "RemoveElementsVisitorInput.osm");

    RemoveElementsVisitor removeElementsVisitor(false);
    removeElementsVisitor.setRecursive(false);
    removeElementsVisitor.addCriterion(ElementCriterionPtr(new PoiCriterion()));
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RemoveElementsVisitorOutput.osm");
    HOOT_FILE_EQUALS(_inputPath + "RemoveElementsVisitorOutput.osm",
                     _outputPath + "RemoveElementsVisitorOutput.osm");
  }

  void runRecursiveTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "RemoveElementsVisitorInput.osm");

    RemoveElementsVisitor removeElementsVisitor(false);
    removeElementsVisitor.setRecursive(true);
    removeElementsVisitor.addCriterion(ElementCriterionPtr(new HighwayCriterion(map)));
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RemoveElementsVisitorRecursiveOutput.osm");
    HOOT_FILE_EQUALS(_inputPath + "RemoveElementsVisitorRecursiveOutput.osm",
                     _outputPath + "RemoveElementsVisitorRecursiveOutput.osm");
  }

  void runNegatedFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "RemoveElementsVisitorInput.osm");

    RemoveElementsVisitor removeElementsVisitor(true);
    removeElementsVisitor.setRecursive(false);
    removeElementsVisitor.addCriterion(ElementCriterionPtr(new PoiCriterion()));
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RemoveElementsVisitorNegatedFilterOutput.osm");
    HOOT_FILE_EQUALS(_inputPath + "RemoveElementsVisitorNegatedFilterOutput.osm",
                     _outputPath + "RemoveElementsVisitorNegatedFilterOutput.osm");
  }

  void runReviewRelationTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "RemoveElementsVisitorTest-reviewRelationTest.osm");

    RemoveElementsVisitor removeElementsVisitor(false);
    removeElementsVisitor.setRecursive(false);
    removeElementsVisitor.addCriterion(ElementCriterionPtr(new ReviewRelationCriterion()));
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RemoveElementsVisitorTest-reviewRelationTestOut.osm");
    HOOT_FILE_EQUALS( _inputPath + "RemoveElementsVisitorTest-reviewRelationTestOut.osm",
                     _outputPath + "RemoveElementsVisitorTest-reviewRelationTestOut.osm");
  }

  void runMultipleCriteriaTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "RemoveElementsVisitorInput.osm");

    RemoveElementsVisitor removeElementsVisitor(false);
    removeElementsVisitor.setRecursive(false);
    removeElementsVisitor.addCriterion(ElementCriterionPtr(new PoiCriterion()));
    removeElementsVisitor.addCriterion(ElementCriterionPtr(new HighwayCriterion(map)));
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RemoveElementsVisitorTest-runMultipleCriteriaTestOut.osm");
    HOOT_FILE_EQUALS(_inputPath + "RemoveElementsVisitorTest-runMultipleCriteriaTestOut.osm",
                     _outputPath + "RemoveElementsVisitorTest-runMultipleCriteriaTestOut.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveElementsVisitorTest, "quick");

}


