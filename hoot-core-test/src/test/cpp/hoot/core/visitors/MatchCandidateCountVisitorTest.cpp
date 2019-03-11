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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/MatchCandidateCountVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class MatchCandidateCountVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MatchCandidateCountVisitorTest);
  CPPUNIT_TEST(runBuildingMatchCandidateCountTest);
  CPPUNIT_TEST(runHighwayMatchCandidateCountTest);
  CPPUNIT_TEST(runCombinedMatchCandidateCountTest);
  CPPUNIT_TEST(runScriptMatchCreatorTest);
  CPPUNIT_TEST(runMultipleScriptMatchCreatorTest);
  CPPUNIT_TEST(runDualPoiScriptMatchCreatorTest);
  CPPUNIT_TEST(runFilteredPoiMatchCreatorTest);
  CPPUNIT_TEST(runFilteredMultipleMatchCreatorTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MatchCandidateCountVisitorTest()
  {
    setResetType(ResetAll);
  }

  void runBuildingMatchCandidateCountTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
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
      boost::any_cast<QMap<QString, long>>(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL((long)18, matchCandidateCountsByMatchCreator["hoot::BuildingMatchCreator"]);
  }

  void runHighwayMatchCandidateCountTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
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
      boost::any_cast<QMap<QString, long>>(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL((long)8, matchCandidateCountsByMatchCreator["hoot::HighwayMatchCreator"]);
  }

  void runCombinedMatchCandidateCountTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
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
      boost::any_cast<QMap<QString, long>>(uut.getData());
    CPPUNIT_ASSERT_EQUAL(3, matchCandidateCountsByMatchCreator.size());
    //These don't add up to the total...is there some overlap here?
    CPPUNIT_ASSERT_EQUAL((long)18, matchCandidateCountsByMatchCreator["hoot::BuildingMatchCreator"]);
    CPPUNIT_ASSERT_EQUAL((long)8, matchCandidateCountsByMatchCreator["hoot::HighwayMatchCreator"]);
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      matchCandidateCountsByMatchCreator["hoot::hoot::ScriptMatchCreator,LineStringGenericTest.js"]);
  }

  void runScriptMatchCreatorTest()
  {
    // Script match creators are handled a little differently during match candidate count creation
    // than regular match creators.  This test specifically checks that the match creators
    // used by the visitor are the correct ones that were specified in the configuration.

    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    QStringList matchCreators;
    matchCreators.append("hoot::ScriptMatchCreator,PoiGeneric.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)21, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      boost::any_cast<QMap<QString, long>>(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL(
      (long)21, matchCandidateCountsByMatchCreator["hoot::ScriptMatchCreator,PoiGeneric.js"]);
  }

  void runMultipleScriptMatchCreatorTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    QStringList matchCreators;
    matchCreators.append("hoot::ScriptMatchCreator,LinearWaterway.js");
    matchCreators.append("hoot::ScriptMatchCreator,PoiGeneric.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)21, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      boost::any_cast<QMap<QString, long>>(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    LOG_VARD(matchCandidateCountsByMatchCreator);
    CPPUNIT_ASSERT_EQUAL(
      (long)21, matchCandidateCountsByMatchCreator["hoot::ScriptMatchCreator,PoiGeneric.js"]);
  }

  void runDualPoiScriptMatchCreatorTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    QStringList matchCreators;
    matchCreators.append("hoot::ScriptMatchCreator,PoiGeneric.js");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);

    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)21, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      boost::any_cast<QMap<QString, long>>(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL(
      (long)21, matchCandidateCountsByMatchCreator["hoot::ScriptMatchCreator,PoiGeneric.js"]);
  }

  void runFilteredPoiMatchCreatorTest()
  {
    OsmMapPtr map(new OsmMap());
    NodePtr node1(new Node(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0));
    NodePtr node2(new Node(Status::Unknown2, 2, geos::geom::Coordinate(0.0, 0.0), 15.0));

    node1->getTags().clear();
    node1->getTags().set("poi", "yes");
    map->addNode(node1);

    node2->getTags().clear();
    node2->getTags().set("poi", "yes");
    map->addNode(node2);

    MapProjector::projectToPlanar(map);

    QStringList matchCreators;
    matchCreators.append("hoot::ScriptMatchCreator,PoiGeneric.js");
    boost::shared_ptr<MatchCandidateCountVisitor> uut;
    const QString poiTagFilter = "{ \"must\": [ { \"tag\": \"poi=yes\" } ] }";

    MatchFactory::getInstance().reset();
    MatchFactory::_setTagFilter(poiTagFilter);
    MatchFactory::_setMatchCreators(matchCreators);
    uut.reset(new MatchCandidateCountVisitor(MatchFactory::getInstance().getCreators()));
    map->visitRo(*uut);
    CPPUNIT_ASSERT_EQUAL((int)2, (int)uut->getStat());

    const QString restaurantTagFilter = "{ \"must\": [ { \"tag\": \"amenity=restaurant\" } ] }";

    MatchFactory::getInstance().reset();
    MatchFactory::_setTagFilter(restaurantTagFilter);
    MatchFactory::_setMatchCreators(matchCreators);
    uut.reset(new MatchCandidateCountVisitor(MatchFactory::getInstance().getCreators()));
    node1->getTags().set("amenity", "restaurant");
    map->visitRo(*uut);
    CPPUNIT_ASSERT_EQUAL((int)1, (int)uut->getStat());

    MatchFactory::getInstance().reset();
    MatchFactory::_setTagFilter(restaurantTagFilter);
    MatchFactory::_setMatchCreators(matchCreators);
    uut.reset(new MatchCandidateCountVisitor(MatchFactory::getInstance().getCreators()));
    node2->getTags().set("amenity", "restaurant");
    map->visitRo(*uut);
    CPPUNIT_ASSERT_EQUAL((int)2, (int)uut->getStat());
  }

  void runFilteredMultipleMatchCreatorTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    QStringList matchCreators;
    matchCreators.append("hoot::BuildingMatchCreator");
    matchCreators.append("hoot::ScriptMatchCreator,PoiGeneric.js");
    boost::shared_ptr<MatchCandidateCountVisitor> uut;

    MatchFactory::getInstance().reset();
    MatchFactory::_setTagFilter("");
    MatchFactory::_setMatchCreators(matchCreators);
    uut.reset(new MatchCandidateCountVisitor(MatchFactory::getInstance().getCreators()));
    map->visitRo(*uut);
    CPPUNIT_ASSERT_EQUAL((int)39, (int)uut->getStat());

    MatchFactory::getInstance().reset();
    MatchFactory::_setTagFilter("{ \"must\": [ { \"tag\": \"building=yes\" } ] }");
    MatchFactory::_setMatchCreators(matchCreators);
    uut.reset(new MatchCandidateCountVisitor(MatchFactory::getInstance().getCreators()));
    map->visitRo(*uut);
    CPPUNIT_ASSERT_EQUAL((int)17, (int)uut->getStat());

    MatchFactory::getInstance().reset();
    MatchFactory::_setTagFilter("{ \"must\": [ { \"tag\": \"poi=yes\" } ] }");
    MatchFactory::_setMatchCreators(matchCreators);
    uut.reset(new MatchCandidateCountVisitor(MatchFactory::getInstance().getCreators()));
    map->visitRo(*uut);
    CPPUNIT_ASSERT_EQUAL((int)21, (int)uut->getStat());

    MatchFactory::getInstance().reset();
    MatchFactory::_setTagFilter("{ \"must\": [ { \"tag\": \"name=Starbucks\" } ] }");
    MatchFactory::_setMatchCreators(matchCreators);
    uut.reset(new MatchCandidateCountVisitor(MatchFactory::getInstance().getCreators()));
    map->visitRo(*uut);
    CPPUNIT_ASSERT_EQUAL((int)12, (int)uut->getStat());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchCandidateCountVisitorTest, "quick");

}
