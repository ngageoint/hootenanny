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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/filters/HighwayCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmXmlWriter.h>

// Qt
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

class RemoveElementsVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RemoveElementsVisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runRecursiveTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(
      map, "test-files/visitors/RemoveElementsVisitorInput.osm");

    boost::shared_ptr<PoiCriterion> elementCriterion(new PoiCriterion());
    RemoveElementsVisitor removeElementsVisitor(elementCriterion);
    removeElementsVisitor.setRecursive(false);
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    QDir().mkpath("test-output/visitors");
    OsmXmlWriter writer;
    writer.write(map, "test-output/visitors/RemoveElementsVisitorOutput.osm");
    HOOT_FILE_EQUALS("test-files/visitors/RemoveElementsVisitorOutput.osm",
                     "test-output/visitors/RemoveElementsVisitorOutput.osm");
  }

  void runRecursiveTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(
      map, "test-files/visitors/RemoveElementsVisitorInput.osm");

    boost::shared_ptr<HighwayCriterion> elementCriterion(new HighwayCriterion());
    RemoveElementsVisitor removeElementsVisitor(elementCriterion);
    removeElementsVisitor.setRecursive(true);
    map->visitRw(removeElementsVisitor);

    MapProjector::projectToWgs84(map);
    QDir().mkpath("test-output/visitors");
    OsmXmlWriter writer;
    writer.write(map, "test-output/visitors/RemoveElementsVisitorRecursiveOutput.osm");
    HOOT_FILE_EQUALS("test-files/visitors/RemoveElementsVisitorRecursiveOutput.osm",
                     "test-output/visitors/RemoveElementsVisitorRecursiveOutput.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveElementsVisitorTest, "quick");

}


