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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/scoring/MapComparator.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>

using namespace hoot;

// Tgs
#include <tgs/Statistics/Random.h>

namespace hoot
{

class MapComparatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MapComparatorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MapComparatorTest() : HootTestFixture("test-files/", UNUSED_PATH)
  {
  }

  void runTest()
  {
    OsmXmlReader reader;
    reader.setAddSourceDateTime(true);

    OsmMap::resetCounters();
    OsmMapPtr map1 = std::make_shared<OsmMap>();
    reader.read(_inputPath + "ToyTestA.osm", map1);

    OsmMap::resetCounters();
    OsmMapPtr map2 = std::make_shared<OsmMap>();
    reader.read(_inputPath + "ToyTestA.osm", map2);

    MapComparator uut;

    // Make sure the maps are the same
    bool match = uut.isMatch(map1, map2);
    CPPUNIT_ASSERT(match);

    // Now turn on datetime checking...
    uut.setUseDateTime();

    // Change dates...
    SetTagValueVisitor vtor(MetadataTags::SourceDateTime(), "1989-12-13T12:34:56Z");
    map2->visitRw(vtor);

    // Make sure it fails now!
    {
      DisableLog dl; // Temporarily disable log
      (void) dl;     // Avoid unused var warning
      match = uut.isMatch(map1, map2);
    }
    CPPUNIT_ASSERT(!match);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapComparatorTest, "quick");

}

