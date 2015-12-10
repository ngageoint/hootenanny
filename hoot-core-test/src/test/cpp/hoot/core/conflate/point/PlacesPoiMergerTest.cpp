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
#include <hoot/core/conflate/point/PlacesPoiMatch.h>
#include <hoot/core/conflate/point/PlacesPoiMatchCreator.h>
#include <hoot/core/conflate/point/PlacesPoiMergerCreator.h>
#include <hoot/core/conflate/point/PlacesPoiMerger.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/util/Log.h>
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

class PlacesPoiMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PlacesPoiMergerTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:
  int _i;

  NodePtr createNode(OsmMapPtr map, double x, double y, double acc, QString name,
    QString note = "")
  {
    NodePtr result(new Node(Status::Unknown1, map->createNextNodeId(), x, y, acc));
    map->addNode(result);

    result->getTags()["poi"] = "yes";
    result->getTags()["name"] = name;
    if (note != "")
    {
      result->getTags()["note"] = note;
    }
    else
    {
      result->getTags()["note"] = QString("No Match: %1 (%2)").arg(name).arg(_i++);
    }

    return result;
  }

  void runTest()
  {
    _i = 0;
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0.0;
    env.MinY = 0.0;
    env.MaxX = 1.0;
    env.MaxY = 1.0;
    MapProjector::projectToPlanar(map, env);

    int matchCount = 0;

    vector<Merger*> mergers;
    MatchSet matches;

    PlacesPoiMergerCreator creator;

    // Match
    matchCount++;
    shared_ptr<Node> n1 = createNode(map, 0.0, 0.0, 250.0, "McDonalds", "McDonalds");
    shared_ptr<Node> n2 = createNode(map, 200.0, 0.0, 220.0, "Mc Donalds", "McDonalds");
    matches.insert(
      new PlacesPoiMatch(map, n1->getElementId(), n2->getElementId(), shared_ptr<MatchThreshold>()));
    creator.createMergers(matches, mergers);
    CPPUNIT_ASSERT_EQUAL(2, (int)mergers.back()->getImpactedElementIds().size());

    // Match b/c only one letter difference & < 5 characters in name
    matchCount++;
    shared_ptr<Node> n3 = createNode(map, 1000.0, 0.0, 10.0, "a", "a");
    shared_ptr<Node> n4 = createNode(map, 1000.0, 1.0, 20.0, "b", "a");
    matches.clear();
    matches.insert(
      new PlacesPoiMatch(map, n3->getElementId(), n4->getElementId(), shared_ptr<MatchThreshold>()));
    creator.createMergers(matches, mergers);
    CPPUNIT_ASSERT_EQUAL(2, (int)mergers.back()->getImpactedElementIds().size());

    // All these get matched together into a chain.
    matchCount += 3;
    shared_ptr<Node> n5 = createNode(map, -1000.0, 0.0, 19.0, "Chain", "Chain");
    shared_ptr<Node> n6 = createNode(map, -1000.0, 20.0, 20.0, "Chain", "Chain");
    shared_ptr<Node> n7 = createNode(map, -1000.0, 40.0, 20.0, "Chain", "Chain");
    shared_ptr<Node> n8 = createNode(map, -1000.0, 60.0, 20.0, "Chain", "Chain");
    matches.clear();
    matches.insert(
      new PlacesPoiMatch(map, n5->getElementId(), n6->getElementId(), shared_ptr<MatchThreshold>()));
    matches.insert(
      new PlacesPoiMatch(map, n6->getElementId(), n7->getElementId(), shared_ptr<MatchThreshold>()));
    matches.insert(
      new PlacesPoiMatch(map, n7->getElementId(), n8->getElementId(), shared_ptr<MatchThreshold>()));
    creator.createMergers(matches, mergers);
    CPPUNIT_ASSERT_EQUAL(4, (int)mergers.back()->getImpactedElementIds().size());

    CPPUNIT_ASSERT_EQUAL(3, (int)mergers.size());

    vector< pair<ElementId, ElementId> > replaced;
    for (size_t i = 0; i < mergers.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(true, mergers[i]->isValid(map));

      mergers[i]->apply(map, replaced);
    }

    CPPUNIT_ASSERT_EQUAL(3, (int)map->getNodeMap().size());

    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(n2->getId()));
    CPPUNIT_ASSERT_EQUAL(string("Mc Donalds"), n2->getTags()["name"].toStdString());
    CPPUNIT_ASSERT_EQUAL(string("McDonalds"), n2->getTags()["alt_name"].toStdString());

    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(n3->getId()));
    CPPUNIT_ASSERT_EQUAL(string("a"), n3->getTags()["name"].toStdString());
    CPPUNIT_ASSERT_EQUAL(string("b"), n3->getTags()["alt_name"].toStdString());

    CPPUNIT_ASSERT_EQUAL(true, map->containsNode(n5->getId()));
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PlacesPoiMergerTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PlacesPoiMergerTest, "quick");

}
