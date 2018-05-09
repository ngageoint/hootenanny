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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/visitors/MatchCandidateCountVisitor.h>
#include <hoot/core/TestUtils.h>

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

namespace hoot
{

class MatchCandidateCountVisitorRndTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MatchCandidateCountVisitorRndTest);
  CPPUNIT_TEST(runMatchCandidateCountTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }

  void runMatchCandidateCountTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/conflate/unified/AllDataTypesA.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/conflate/unified/AllDataTypesB.osm", map);
    MapProjector::projectToPlanar(map);

    QStringList matchCreators;

    matchCreators.clear();
    matchCreators.append("hoot::PoiPolygonMatchCreator");
    MatchFactory::getInstance().reset();
    MatchFactory::_setMatchCreators(matchCreators);
    MatchCandidateCountVisitor uut(MatchFactory::getInstance().getCreators());
    map->visitRo(uut);
    CPPUNIT_ASSERT_EQUAL((int)36, (int)uut.getStat());
    QMap<QString, long> matchCandidateCountsByMatchCreator =
      any_cast<QMap<QString, long> >(uut.getData());
    CPPUNIT_ASSERT_EQUAL(1, matchCandidateCountsByMatchCreator.size());
    CPPUNIT_ASSERT_EQUAL((long)36, matchCandidateCountsByMatchCreator["hoot::PoiPolygonMatchCreator"]);
    CPPUNIT_ASSERT_EQUAL(
      (long)0,
      matchCandidateCountsByMatchCreator["hoot::hoot::ScriptMatchCreator,LineStringGenericTest.js"]);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchCandidateCountVisitorRndTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchCandidateCountVisitorRndTest, "current");

}
