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
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/MedianNodeVisitor.h>

namespace hoot
{

class MedianNodeVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MedianNodeVisitorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MedianNodeVisitorTest()
    : HootTestFixture("test-files/",
                      UNUSED_PATH)
  {
    setResetType(ResetAll);
  }

  void runTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "ToyTestA.osm", map);
    MapProjector::projectToPlanar(map);

    MedianNodeVisitor uut;
    map->visitRo(uut);
    NodePtr n = map->getNode(uut.calculateMedianNode()->getElementId());
    MapProjector::projectToWgs84(map);
    HOOT_STR_EQUALS("-104.89970698747904 38.854167001890765", n->toCoordinate().toString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MedianNodeVisitorTest, "quick");

}
