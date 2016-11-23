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

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/visitors/MatchCandidateCountVisitor.h>
using namespace hoot;

// Boost
using namespace boost;

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

#include "../TestUtils.h"

namespace hoot
{

class MatchCandidateCountVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MatchCandidateCountVisitorTest);
  CPPUNIT_TEST(runBuildingMatchCandidateCountTest);
  CPPUNIT_TEST(runHighwayMatchCandidateCountTest);
  CPPUNIT_TEST(runCombinedMatchCandidateCountTest);
  CPPUNIT_TEST(runScriptMatchCreatorTest);
  CPPUNIT_TEST(runMultipleScriptMatchCreatorTest);
  CPPUNIT_TEST(runDualPoiScriptMatchCreatorTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }

  void runBuildingMatchCandidateCountTest()
  {
    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    QStringList matchCreators;
    matchCreators.append("hoot::BuildingMatchCreator");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)18, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL((long)18, matchCandidateCountsByMatchCreator["hoot::BuildingMatchCreator"]);
  }

  void runHighwayMatchCandidateCountTest()
  {
    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    QStringList matchCreators;
    matchCreators.append("hoot::HighwayMatchCreator");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)8, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL((long)8, matchCandidateCountsByMatchCreator["hoot::HighwayMatchCreator"]);
  }

  void runCombinedMatchCandidateCountTest()
  {
    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    QStringList matchCreators;
    matchCreators.append("hoot::BuildingMatchCreator");
    matchCreators.append("hoot::HighwayMatchCreator");
    matchCreators.append("hoot::ScriptMatchCreator,LineStringGeneric.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);

    CPPUNIT_ASSERT_EQUAL((int)34, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(uut.getData());
    CPPUNIT_ASSERT_EQUAL(3, matchCandidateCountsByMatchCreator.size());
    //These don't add up to the total...is there some overlap here?
    CPPUNIT_ASSERT_EQUAL((long)18, matchCandidateCountsByMatchCreator["hoot::BuildingMatchCreator"]);
    CPPUNIT_ASSERT_EQUAL((long)8, matchCandidateCountsByMatchCreator["hoot::HighwayMatchCreator"]);
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      matchCandidateCountsByMatchCreator["hoot::hoot::ScriptMatchCreator,LineStringGenericTest.js"]);
  }

  //Script match creators are handled a little differently during match candidate count creation than
  //regular match creators.  This test is specifically checking that the match creators used by the
  //visitor are the correct ones that were specified in the configuration.
  void runScriptMatchCreatorTest()
  {
    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    TestUtils::resetEnvironment();

    QStringList matchCreators;
    matchCreators.append("hoot::ScriptMatchCreator,PoiGeneric.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)21, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL(
      (long)21, matchCandidateCountsByMatchCreator["hoot::ScriptMatchCreator,PoiGeneric.js"]);
  }

  void runMultipleScriptMatchCreatorTest()
  {
    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    TestUtils::resetEnvironment();

    QStringList matchCreators;
    matchCreators.append("hoot::ScriptMatchCreator,LinearWaterway.js");
    matchCreators.append("hoot::ScriptMatchCreator,PoiGeneric.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)21, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    LOG_VARD(matchCandidateCountsByMatchCreator);
    CPPUNIT_ASSERT_EQUAL(
      (long)21, matchCandidateCountsByMatchCreator["hoot::ScriptMatchCreator,PoiGeneric.js"]);
  }

  void runDualPoiScriptMatchCreatorTest()
  {
    OsmReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    TestUtils::resetEnvironment();

    QStringList matchCreators;
    matchCreators.append("hoot::ScriptMatchCreator,PoiGeneric.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)21, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL(
      (long)21, matchCandidateCountsByMatchCreator["hoot::ScriptMatchCreator,PoiGeneric.js"]);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchCandidateCountVisitorTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchCandidateCountVisitorTest, "current");

}
