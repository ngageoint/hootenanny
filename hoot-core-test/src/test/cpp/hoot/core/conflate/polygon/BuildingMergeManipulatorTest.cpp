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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// Hoot
#include <hoot/core/Conflator.h>
#include <hoot/core/MapReprojector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/polygon/BuildingMergeManipulator.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>

// Tgs
#include <tgs/StreamUtils.h>

#include "../../TestUtils.h"

namespace hoot
{

class BuildingMergeManipulatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BuildingMergeManipulatorTest);
  CPPUNIT_TEST(runConflateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConstWayPtr getWay(ConstOsmMapPtr map, const QString& key, const QString& value)
  {
    std::vector<long> wids = map->findWays(key, value);
    CPPUNIT_ASSERT_EQUAL((size_t)1, wids.size());
    return map->getWay(wids[0]);
  }

  void runConflateTest()
  {
    OsmReader reader;

    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);

    MapReprojector::reprojectToPlanar(map);

    WayMap wm = map->getWays();
    for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
    {
      const ConstWayPtr& w = it->second;
      const Tags& t = w->getTags();
      if (t["REF1"] != "Target" && t["REF2"] != "Target")
      {
        map->removeWay(it->first);
      }
    }

    Conflator uut;

    shared_ptr<Manipulator> m(new BuildingMergeManipulator());
    deque< shared_ptr<Manipulator> > manipulators;
    manipulators.push_back(m);
    uut.setManipulators(manipulators);

    uut.loadSource(map);
    uut.conflate();

    shared_ptr<OsmMap> out(new OsmMap(uut.getBestMap()));
    MapReprojector::reprojectToWgs84(out);

    OsmWriter writer;
    writer.setIncludeIds(true);
    writer.write(out, "test-output/BuildingConflatorTest.osm");

    ConstWayPtr cheddars = getWay(out, "REF1", "Cheddar's");
    CPPUNIT_ASSERT_EQUAL(string("Cheddar's"), cheddars->getTags()["REF2"].toStdString());
    HOOT_STR_EQUALS(Status::Conflated, cheddars->getStatus());

    ConstWayPtr biondi = getWay(out, "REF1", "Biondi");
    CPPUNIT_ASSERT_EQUAL(string("Biondi"), biondi->getTags()["REF2"].toStdString());
    HOOT_STR_EQUALS(Status::Conflated, biondi->getStatus());

    ConstWayPtr freddys = getWay(out, "REF1", "Freddy's");
    CPPUNIT_ASSERT_EQUAL(false, freddys->getTags().contains("REF2"));
    HOOT_STR_EQUALS(Status::Unknown1, freddys->getStatus());

    ConstWayPtr target = getWay(out, "REF1", "Target");
    CPPUNIT_ASSERT_EQUAL(string("Target"), biondi->getTags()["REF2"].toStdString());
    HOOT_STR_EQUALS(Status::Unknown1, target->getStatus());

    CPPUNIT_ASSERT_EQUAL((size_t)9, out->getWays().size());
  }
};

/// @todo disabled while refactoring. See #2376 and See ticket #2872
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingMergeManipulatorTest, "current");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingMergeManipulatorTest, "quick");

}

