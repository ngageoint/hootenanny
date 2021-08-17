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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/manipulators/WayMergeManipulation.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

// TGS
#include <tgs/StreamUtils.h>

using namespace Tgs;
using namespace std;

namespace hoot
{

class WayMergeManipulationTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayMergeManipulationTest);
  CPPUNIT_TEST(individualManipulationsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  const QString inputPath = "test-files/manipulators/";
  const QString outputPath = "test-output/manipulators/";

  WayMergeManipulationTest()
  {
    setResetType(ResetBasic);
    TestUtils::mkpath(outputPath);
  }

  void individualManipulationsTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.read(inputPath + "WayMergeManipulation.osm", map);

    MapProjector::projectToOrthographic(map);

    long left = FindWaysVisitor::findWaysByTag(map, "note", "3")[0];
    long right = FindWaysVisitor::findWaysByTag(map, "note", "4")[0];

    map->getWay(left)->setStatus(Status::Unknown1);
    map->getWay(right)->setStatus(Status::Unknown2);

    WayMergeManipulation uut(left, right, map, 10.0);
    set<ElementId> ignored1, ignored2;
    OsmMapPtr after(new OsmMap(map));
    uut.applyManipulation(after, ignored1, ignored2);

    MapProjector::projectToWgs84(after);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(after, outputPath + "WayMergeManipulation.osm");

    HOOT_FILE_EQUALS(inputPath + "WayMergeManipulationOutput.osm",
                     outputPath + "WayMergeManipulation.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayMergeManipulationTest, "quick");

}
