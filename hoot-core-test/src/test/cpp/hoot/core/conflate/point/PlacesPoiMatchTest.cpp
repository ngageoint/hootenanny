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
#include <hoot/core/conflate/point/PlacesPoiMatch.h>
#include <hoot/core/conflate/point/PlacesPoiMatchCreator.h>
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

class PlacesPoiMatchTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PlacesPoiMatchTest);
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

    // Match
    matchCount++;
    createNode(map, 0.0, 0.0, 250.0, "McDonalds", "McDonalds");
    createNode(map, 200.0, 0.0, 250.0, "Mc Donalds", "McDonalds");

    // no match
    createNode(map, 0.0, 0.0, 250.0, "Wendy's");

    // no match b/c of big difference in names
    createNode(map, 1000.0, 0.0, 250.0, "Chipotle");
    createNode(map, 1000.0, 200.0, 250.0, "Chipotle Mexican Grill");

    // no match b/c of distance & circular error
    createNode(map, 1000.0, 0.0, 50.0, "Pizza Hut");
    createNode(map, 1000.0, 200.0, 50.0, "Pizza Hut");

    // Match
    matchCount++;
    createNode(map, 1000.0, 0.0, 200.0, "Roy Rogers Restaurant", "Roy Rogers");
    createNode(map, 1000.0, 50.0, 200.0, QString::fromUtf8("Рой Роджерс ресторан"), "Roy Rogers");

    // Miss b/c one letter difference & < 5 characters in name
    createNode(map, 1000.0, 0.0, 20.0, "a", "a");
    createNode(map, 1000.0, 1.0, 20.0, "b", "a miss");

    // no match b/c two letter difference & < 5 characters in name
    createNode(map, 2000.0, 0.0, 20.0, "abc", "b");
    createNode(map, 2000.0, 1.0, 20.0, "bbb", "b miss");

    // Match b/c only 20% difference & more than 5 letters
    matchCount++;
    createNode(map, 5000.0, 0.0, 20.0, "1234567890", "123");
    createNode(map, 5000.0, 1.0, 20.0, "12456790", "123");

    // All these get matched together into a chain.
    matchCount += 3;
    createNode(map, -1000.0, 0.0, 30.0, "Chain", "Chain");
    createNode(map, -1000.0, 20.0, 30.0, "Chain", "Chain");
    createNode(map, -1000.0, 40.0, 30.0, "Chain", "Chain");
    createNode(map, -1000.0, 60.0, 30.0, "Chain", "Chain");


    // Match b/c at least one name pair is a match
    matchCount++;
    shared_ptr<Node> n1 = createNode(map, 5000.0, 0.0, 50.0, "Deli", "Schlotzsky's");
    n1->getTags()["alt_name"] = "Schlotzsky's";
    shared_ptr<Node> n2 = createNode(map, 5000.0, 1.0, 50.0, "Schlotzsky", "Schlotzsky's");
    n2->getTags()["alt_name"] = "Sandwiches";

    shared_ptr<const MatchThreshold> threshold(new MatchThreshold(0.6, 0.6));
    PlacesPoiMatchCreator creator;
    vector<const Match*> matches;
    creator.createMatches(map, matches, threshold);

    for (size_t i = 0; i < matches.size(); i++)
    {
      //LOG_INFO(matches[i]->toString());
      set< pair<ElementId, ElementId> > pairs = matches[i]->getMatchPairs();
      for (set< pair<ElementId, ElementId> >::iterator it = pairs.begin(); it != pairs.end(); ++it)
      {
        shared_ptr<const Element> e1 = map->getElement(it->first);
        shared_ptr<const Element> e2 = map->getElement(it->second);
        //LOG_INFO(e1->getTags()["note"] << " <=> " << e2->getTags()["note"]);
        CPPUNIT_ASSERT_EQUAL(e1->getTags()["note"].toStdString(), e2->getTags()["note"].toStdString());
      }
    }

    CPPUNIT_ASSERT_EQUAL(matchCount, (int)matches.size());
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PlacesPoiMatchTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PlacesPoiMatchTest, "quick");

}
