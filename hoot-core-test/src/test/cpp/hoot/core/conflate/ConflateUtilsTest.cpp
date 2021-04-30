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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/conflate/ConflateUtils.h>
#include <hoot/core/conflate/highway/HighwayMatchCreator.h>
#include <hoot/core/conflate/highway/HighwayMergerCreator.h>
#include <hoot/core/io/OsmMapReaderFactory.h>

namespace hoot
{

class ConflateUtilsTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(ConflateUtilsTest);
    CPPUNIT_TEST(runWriteUnconflatableTest);
    CPPUNIT_TEST(runWriteDiffTest);
    CPPUNIT_TEST_SUITE_END();

public:

  ConflateUtilsTest() :
  HootTestFixture(
    "test-files/conflate/ConflateUtilsTest/", "test-output/conflate/ConflateUtilsTest/")
  {
    setResetType(ResetAll);
  }

  void runWriteUnconflatableTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm", true, Status::Unknown1);
    LOG_VART(map->size());

    // There should be nothing that can't be conflated in this map, therefore no map file will be
    // output.
    CPPUNIT_ASSERT_EQUAL(
      0,
      ConflateUtils::writeNonConflatable(
        map, _outputPath + "runWriteUnconflatableTest-1-out.osm", true));

    // Now, let's add a single node that can't be conflated if generic conflators are ignored.
    const long nodeId = map->createNextNodeId();
    LOG_VART(nodeId);
    NodePtr unconflatableNode =
      std::make_shared<Node>(Status::Unknown1, nodeId, geos::geom::Coordinate(0.0, 0.0), 15.0);
    unconflatableNode->setTag("blah", "bleh");
    map->addNode(unconflatableNode);
    LOG_VART(map->size());

    const int mapSize =
      ConflateUtils::writeNonConflatable(
        map, _outputPath + "runWriteUnconflatableTest-2-out.osm", true);
    LOG_VART(mapSize);
    HOOT_FILE_EQUALS(
      _inputPath + "runWriteUnconflatableTest-2-out.osm",
      _outputPath + "runWriteUnconflatableTest-2-out.osm");
  }

  void runWriteDiffTest()
  {
    QString out = _outputPath + "runWriteDiffTest-out.osm";
    ConflateUtils::writeDiff(
      "test-files/ToyTestB.osm", "test-files/ToyTestA.osm",
      geos::geom::Envelope(-104.9007, -104.8994, 38.8541, 38.8552), out);
    HOOT_FILE_EQUALS(_inputPath + "runWriteDiffTest-out.osm", out);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateUtilsTest, "quick");

}
