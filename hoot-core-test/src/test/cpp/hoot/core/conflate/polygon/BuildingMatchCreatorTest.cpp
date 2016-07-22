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

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/ops/RemoveWayOp.h>
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

class BuildingMatchCreatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BuildingMatchCreatorTest);
  CPPUNIT_TEST(runMatchTest);
  CPPUNIT_TEST(runIsCandidateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConstWayPtr getWay(ConstOsmMapPtr map, const QString& key, const QString& value)
  {
    std::vector<long> wids = FindWaysVisitor::findWaysByTag(map, key, value);
    CPPUNIT_ASSERT_EQUAL((size_t)1, wids.size());
    return map->getWay(wids[0]);
  }

  bool contains(const vector<const Match*>& matches, ElementId eid1, ElementId eid2)
  {
    bool result = false;
    for (size_t i = 0; i < matches.size(); i++)
    {
      set< pair<ElementId, ElementId> > s = matches[i]->getMatchPairs();
      if (matches[i]->getProbability() > 0.5)
      {
        for (set< pair<ElementId, ElementId> >::const_iterator it = s.begin(); it != s.end(); ++it)
        {
          if (it->first == eid1 && it->second == eid2)
          {
            result = true;
          }
        }
      }
    }

    return result;
  }

  void runMatchTest()
  {
    OsmReader reader;

    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/ToyBuildingsTestB.osm", map);

    MapProjector::projectToPlanar(map);

    WayMap wm = map->getWays();
    for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
    {
      const ConstWayPtr& w = it->second;
      const Tags& t = w->getTags();
      if (t["REF1"] != "Target" && t["REF2"] != "Target")
      {
        RemoveWayOp::removeWay(map, it->first);
      }
    }
    BuildingMatchCreator uut;
    vector<const Match*> matches;

    shared_ptr<const MatchThreshold> threshold(new MatchThreshold(0.6, 0.6));
    uut.createMatches(map, matches, threshold);

    CPPUNIT_ASSERT_EQUAL(3, int(matches.size()));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-15)));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-14)));
    CPPUNIT_ASSERT_EQUAL(true, contains(matches, ElementId::way(-7), ElementId::way(-13)));
  }

  void runIsCandidateTest()
  {
    BuildingMatchCreator uut;

    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());

    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyBuildingsTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(uut.isMatchCandidate(map->getWay(FindWaysVisitor::findWaysByTag(map, "name", "Panera Bread")[0]), map));

    OsmMap::resetCounters();
    map.reset(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(!uut.isMatchCandidate(map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "1")[0]), map));
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingMatchCreatorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BuildingMatchCreatorTest, "quick");

}
