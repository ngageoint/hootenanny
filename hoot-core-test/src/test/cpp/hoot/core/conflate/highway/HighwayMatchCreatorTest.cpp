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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/highway/HighwayMatchCreator.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>


#include "../../TestUtils.h"

namespace hoot
{

class HighwayMatchCreatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(HighwayMatchCreatorTest);
  CPPUNIT_TEST(runIsCandidateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runIsCandidateTest()
  {
    HighwayMatchCreator uut;

    OsmXmlReader reader;
    boost::shared_ptr<OsmMap> map(new OsmMap());

    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(
      !uut.isMatchCandidate(
        map->getWay(FindWaysVisitor::findWaysByTag(map, "name", "Panera Bread")[0]), map));

    OsmMap::resetCounters();
    map.reset(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(
      uut.isMatchCandidate(map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]), map));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwayMatchCreatorTest, "quick");

}
