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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/conflate/WayCleaner.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
using namespace hoot;

// Qt
#include <QDebug>
#include <QDir>
#include <QTest>

#include "../TestUtils.h"

namespace hoot
{

class WayCleanerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(WayCleanerTest);
  CPPUNIT_TEST(runDuplicateNodesTest);
  CPPUNIT_TEST(runDuplicateCoordsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runDuplicateNodesTest()
  {
    OsmXmlReader reader;
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/WayCleanerTest/DuplicateNodesTest.osm", map);

    WayPtr cleanedWay(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]).get()));
    WayCleaner::cleanWay(cleanedWay, map);
    CPPUNIT_ASSERT(!WayCleaner::hasDuplicateNodes(cleanedWay));

    cleanedWay.reset(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "2")[0]).get()));
    WayCleaner::cleanWay(cleanedWay, map);
    CPPUNIT_ASSERT(!WayCleaner::hasDuplicateNodes(cleanedWay));

    cleanedWay.reset(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "3")[0]).get()));
    WayCleaner::cleanWay(cleanedWay, map);
    CPPUNIT_ASSERT(!WayCleaner::hasDuplicateNodes(cleanedWay));

    cleanedWay.reset(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "4")[0]).get()));
    WayCleaner::cleanWay(cleanedWay, map);
    CPPUNIT_ASSERT(!WayCleaner::hasDuplicateNodes(cleanedWay));

    cleanedWay.reset(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "5")[0]).get()));
    QString exceptionMsg = "";
    try
    {
      WayCleaner::cleanWay(cleanedWay, map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.getWhat();
    }
    CPPUNIT_ASSERT(exceptionMsg.toLower().contains("cannot clean zero length way"));
  }

  void runDuplicateCoordsTest()
  {
    OsmXmlReader reader;
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/WayCleanerTest/DuplicateCoordsTest.osm", map);

    WayPtr cleanedWay(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]).get()));
    WayCleaner::cleanWay(cleanedWay, map);
    CPPUNIT_ASSERT(!WayCleaner::hasDuplicateCoords(cleanedWay, *map));

    cleanedWay.reset(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "2")[0]).get()));
    WayCleaner::cleanWay(cleanedWay, map);
    CPPUNIT_ASSERT(!WayCleaner::hasDuplicateCoords(cleanedWay, *map));

    cleanedWay.reset(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "3")[0]).get()));
    WayCleaner::cleanWay(cleanedWay, map);
    CPPUNIT_ASSERT(!WayCleaner::hasDuplicateCoords(cleanedWay, *map));

    cleanedWay.reset(new Way(*map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "4")[0]).get()));
    QString exceptionMsg = "";
    try
    {
      WayCleaner::cleanWay(cleanedWay, map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.getWhat();
    }
    CPPUNIT_ASSERT(exceptionMsg.toLower().contains("cannot clean zero length way"));
  }
};

}

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayCleanerTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayCleanerTest, "quick");

