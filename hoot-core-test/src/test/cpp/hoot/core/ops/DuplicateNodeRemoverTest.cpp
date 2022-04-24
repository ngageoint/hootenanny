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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>
#include <hoot/core/util/Progress.h>

namespace hoot
{

class DuplicateNodeRemoverTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(DuplicateNodeRemoverTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST(runMetadataTest);
    CPPUNIT_TEST(runInvalidDistanceTest);
    CPPUNIT_TEST(runIgnoreStatusTest);
    CPPUNIT_TEST_SUITE_END();

public:

  DuplicateNodeRemoverTest() : HootTestFixture("test-files/", UNUSED_PATH)
  {
    setResetType(ResetEnvironment);
  }

  void runBasicTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    IoUtils::loadMap(map, _inputPath + "jakarta_raya_coastline.shp", true);
    MapProjector::projectToOrthographic(map);
    CPPUNIT_ASSERT_EQUAL(604, (int)map->getNodes().size());

    // Merge all nodes within a meter.
    DuplicateNodeRemover::removeNodes(map, 1.0);

    CPPUNIT_ASSERT_EQUAL(601, (int)map->getNodes().size());
  }

  void runMetadataTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    // Nodes are within the proximity threshold and have no tags...so they're dupes.
    /*NodePtr node1 =*/ TestUtils::createNode(map);
    /*NodePtr node2 =*/ TestUtils::createNode(map);
    MapProjector::projectToOrthographic(map);

    DuplicateNodeRemover::removeNodes(map, 1.0);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getNodes().size());

    // uuid is a metadata tag and should be ignored, so no removal here
    map->clear();
    NodePtr node3 = TestUtils::createNode(map);
    node3->getTags().set("uuid", "{12449bc4-c059-4270-8d72-134fcf54291d}");
    NodePtr node4 = TestUtils::createNode(map);
    node4->getTags().set("uuid", "{bfbf2946-4342-444c-9926-1477c7bcce05}");
    MapProjector::projectToOrthographic(map);

    DuplicateNodeRemover::removeNodes(map, 1.0);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getNodes().size());

    // hoot:* tags are metadata tags and should be ignored, so no removal here
    map->clear();
    NodePtr node5 = TestUtils::createNode(map);
    node5->getTags().set(MetadataTags::HootId(), "1");
    NodePtr node6 = TestUtils::createNode(map);
    node6->getTags().set(MetadataTags::HootId(), "2");
    MapProjector::projectToOrthographic(map);

    DuplicateNodeRemover::removeNodes(map, 1.0);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getNodes().size());

    // The nodes are inside of the proximity threshold but have different names, so aren't
    // duplicates.
    map->clear();
    NodePtr node7 = TestUtils::createNode(map);
    node7->getTags().set("name", "node7");
    NodePtr node8 = TestUtils::createNode(map);
    node8->getTags().set("name", "node8");
    MapProjector::projectToOrthographic(map);

    DuplicateNodeRemover::removeNodes(map, 1.0);

    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
  }

  void runInvalidDistanceTest()
  {
    conf().set(ConfigOptions::getDuplicateNodeRemoverDistanceThresholdKey(), -1.0);

    QString exceptionMsg;
    try
    {
      DuplicateNodeRemover uut(-1.0);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS("Nearby node merging distance must be greater than zero. Distance specified: -1", exceptionMsg);
  }

  void runIgnoreStatusTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();

    // Statuses are different, so won't be considered dupes unless we explicitly ignore status.
    /*NodePtr node1 =*/ TestUtils::createNode(map, "", Status::Unknown1);
    /*NodePtr node2 =*/ TestUtils::createNode(map, "", Status::Unknown2);
    MapProjector::projectToOrthographic(map);

    DuplicateNodeRemover::removeNodes(map, 1.0, true);

    CPPUNIT_ASSERT_EQUAL(1, (int)map->getNodes().size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DuplicateNodeRemoverTest, "quick");

}
