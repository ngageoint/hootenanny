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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/conflate/matching/ScriptMatch.h>
#include <hoot/js/conflate/matching/ScriptMatchCreator.h>

// Qt
#include <QVariant>

#include <iostream>
#include <string>

using namespace v8;

namespace hoot
{

class ScriptMatchCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ScriptMatchCreatorTest);
  CPPUNIT_TEST(runIsCandidateTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ScriptMatchCreatorTest()
    : HootTestFixture("test-files/algorithms/js/",
                      UNUSED_PATH)
  {
  }

  void runIsCandidateTest()
  {
    ScriptMatchCreator uut;
    uut.setArguments(QStringList() << "LineTest.js");

    ConfigOptions co;
    conf().set(co.getUuidHelperRepeatableKey(), true);
    conf().set(co.getReaderUseFileStatusKey(), true);
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "ScriptMatchTest.osm", true);
    MapProjector::projectToPlanar(map);

    //to be a candidate: needs to be determined a candidate by the JS rules file...which for this
    //particular rules file, the element must be linear
    NodePtr node1 = TestUtils::createNode(map, Status::Unknown1, 10.0, 10.0, 250.0);
    CPPUNIT_ASSERT(!uut.isMatchCandidate(node1, map));

    NodePtr node2 = TestUtils::createNode(map, Status::Unknown1, 10.0, 10.0, 250.0);
    QList<NodePtr> wayNodes;
    wayNodes.append(node1);
    wayNodes.append(node2);
    WayPtr way1 = TestUtils::createWay(map, wayNodes, Status::Unknown1);
    way1->getTags().appendValue("highway", "road");
    CPPUNIT_ASSERT(uut.isMatchCandidate(way1, map));

    way1 = TestUtils::createWay(map, wayNodes, Status::Unknown1);
    way1->setStatus(Status::Unknown1);
    way1->getTags().clear();
    HOOT_STR_EQUALS(false, uut.isMatchCandidate(way1, map));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ScriptMatchCreatorTest, "quick");

}
