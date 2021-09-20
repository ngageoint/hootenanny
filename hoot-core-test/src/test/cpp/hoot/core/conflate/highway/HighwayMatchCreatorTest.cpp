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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/highway/HighwayMatchCreator.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

namespace hoot
{

class HighwayMatchCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(HighwayMatchCreatorTest);
  CPPUNIT_TEST(runIsCandidateTest);
  CPPUNIT_TEST(runBadMedianOptionsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  HighwayMatchCreatorTest() : HootTestFixture("test-files/", UNUSED_PATH)
  {
  }

  void runIsCandidateTest()
  {
    HighwayMatchCreator uut;

    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();

    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyBuildingsTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(
      !uut.isMatchCandidate(
        map->getWay(
          ElementIdsVisitor::findElementsByTag(
            map, ElementType::Way, "name", "Panera Bread")[0]), map));

    map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(
      uut.isMatchCandidate(
        map->getWay(
          ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1")[0]), map));
  }

  void runBadMedianOptionsTest()
  {
    HighwayMatchCreator uut;
    QString exceptionMsg;

    // No error here, despite empty option values, since median matching is turned off.
    uut.setRunMedianMatching(false, QStringList(), QStringList());

    // No error here since validation only errors on empty option values.
    uut.setRunMedianMatching(true, QStringList("blah"), QStringList("blah"));

    try
    {
      // Error here since one of the option values list is empty.
      uut.setRunMedianMatching(true, QStringList(), QStringList("blah"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No road median identifying tags specified in"));

    try
    {
      // Error here since one of the option values list is empty.
      uut.setRunMedianMatching(true, QStringList("blah"), QStringList());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No road median transfer tag keys specified in"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwayMatchCreatorTest, "quick");

}
