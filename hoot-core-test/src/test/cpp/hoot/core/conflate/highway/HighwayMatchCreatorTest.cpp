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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

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
  CPPUNIT_TEST_SUITE_END();

public:

  HighwayMatchCreatorTest()
    : HootTestFixture("test-files/",
                      UNUSED_PATH)
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
          ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "name", "Panera Bread")[0]), map));

    map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyTestA.osm", map);
    MapProjector::projectToPlanar(map);

    CPPUNIT_ASSERT(
      uut.isMatchCandidate(
        map->getWay(ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1")[0]), map));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HighwayMatchCreatorTest, "quick");

}
