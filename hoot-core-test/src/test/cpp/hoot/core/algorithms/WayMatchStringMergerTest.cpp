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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "../TestUtils.h"

// hoot
#include <hoot/core/algorithms/WayMatchStringMerger.h>
#include <hoot/core/algorithms/WayMatchStringSplitter.h>
#include <hoot/core/algorithms/linearreference/WayString.h>
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/visitors/FindNodesVisitor.h>

// Qt
#include <QDir>

namespace hoot
{

class WayMatchStringMergerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(WayMatchStringMergerTest);
  CPPUNIT_TEST(runMergeNodeTest);
  CPPUNIT_TEST(runMergeTagsTest);
  CPPUNIT_TEST(runSplitTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WayStringPtr createWayString1(OsmMapPtr map)
  {
    WayPtr w1a = getWay(map, "1a");
    WayPtr w1b = getWay(map, "1b");

    // Create matching WayStrings
    WayStringPtr str1(new WayString());
    str1->append(WaySubline(WayLocation(map, w1a, 0.0), WayLocation::createAtEndOfWay(map, w1a)));
    str1->append(WaySubline(WayLocation(map, w1b, 0.0), WayLocation::createAtEndOfWay(map, w1b)));

    return str1;
  }

  WayStringPtr createWayString2(OsmMapPtr map)
  {
    WayPtr w2a = getWay(map, "2a");
    WayPtr w2b = getWay(map, "2b");
    WayPtr w2c = getWay(map, "2c");

    WayStringPtr str2(new WayString());
    str2->append(WaySubline(WayLocation(map, w2a, 0.0), WayLocation::createAtEndOfWay(map, w2a)));
    // these segments are reversed
    str2->append(WaySubline(WayLocation::createAtEndOfWay(map, w2b), WayLocation(map, w2b, 0.0)));
    str2->append(WaySubline(WayLocation::createAtEndOfWay(map, w2c), WayLocation(map, w2c, 0.0)));

    return str2;
  }

  OsmMapPtr createTestMap(QString path = "test-files/algorithms/WayMatchStringMergerTest.osm")
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::getInstance().read(map, path);

    MapProjector::projectToPlanar(map);

    return map;
  }

  NodePtr getNode(OsmMapPtr map, QString note)
  {
    vector<long> nids = FindNodesVisitor::findNodesByTag(map, "note", note);
    if (nids.size() != 1)
    {
      throw HootException(QString("Expected to find 1 node, but found %1 - %2").arg(nids.size()).
        arg(note));
    }
    return map->getNode(nids[0]);
  }

  WayPtr getWay(OsmMapPtr map, QString note)
  {
    vector<long> vids = FindWaysVisitor::findWaysByTag(map, "note", note);
    if (vids.size() != 1)
    {
      throw HootException(QString("Expected to find 1 way, but found %1 - %2").arg(vids.size()).
        arg(note));
    }
    return map->getWay(vids[0]);
  }

  void runMergeNodeTest()
  {
    TestUtils::resetEnvironment();
    OsmMapPtr map = createTestMap("test-files/algorithms/WayMatchStringMergerTestMergeNode.osm");

    WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(createWayString1(map),
      createWayString2(map)));

    vector< pair<ElementId, ElementId> > replaced;
    WayMatchStringMerger uut(map, mapping, replaced);

    QDir().mkdir("test-output/algorithms/");

    WayMatchStringSplitter().applySplits(map, replaced, uut.getAllSublineMappings());
    uut.updateSublineMapping();

    uut.setTagMerger(TagMergerFactory::getInstance().getDefaultPtr());

    uut.mergeIntersection(getNode(map, "n0")->getElementId());
    uut.mergeNode(getNode(map, "n1")->getElementId());
    uut.mergeNode(getNode(map, "n2")->getElementId());
    uut.mergeIntersection(getNode(map, "n3")->getElementId());

    MapProjector::projectToWgs84(map);
    shared_ptr<OsmWriter> writer(new OsmWriter());
    writer->setIncludeHootInfo(true);
    writer->write(map,
      "test-output/algorithms/WayMatchStringMergerTestMergeNode.osm");

    HOOT_FILE_EQUALS("test-files/algorithms/WayMatchStringMergerTestMergeNodeExpected.osm",
      "test-output/algorithms/WayMatchStringMergerTestMergeNode.osm");
  }

  void runMergeTagsTest()
  {
    TestUtils::resetEnvironment();
    OsmMapPtr map = createTestMap();

    WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(createWayString1(map),
      createWayString2(map)));

    vector< pair<ElementId, ElementId> > replaced;
    WayMatchStringMerger uut(map, mapping, replaced);

    QDir().mkdir("test-output/algorithms/");

    WayMatchStringSplitter().applySplits(map, replaced, uut.getAllSublineMappings());
    uut.updateSublineMapping();

    uut.setTagMerger(TagMergerFactory::getInstance().getDefaultPtr());
    uut.mergeTags();
    uut.setKeeperStatus(Status::Conflated);

    MapProjector::projectToWgs84(map);
    shared_ptr<OsmWriter> writer(new OsmWriter());
    writer->setIncludeHootInfo(true);
    writer->write(map,
      "test-output/algorithms/WayMatchStringMergerTestMergeTags.osm");

    HOOT_FILE_EQUALS("test-files/algorithms/WayMatchStringMergerTestMergeTagsExpected.osm",
      "test-output/algorithms/WayMatchStringMergerTestMergeTags.osm");
  }

  void runSplitTest()
  {
    TestUtils::resetEnvironment();
    OsmMapPtr map = createTestMap();

    WayMatchStringMappingPtr mapping(new NaiveWayMatchStringMapping(createWayString1(map),
      createWayString2(map)));

    vector< pair<ElementId, ElementId> > replaced;
    WayMatchStringMerger uut(map, mapping, replaced);
    WayMatchStringSplitter().applySplits(map, replaced, uut.getAllSublineMappings());

    QDir().mkdir("test-output/algorithms/");

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::getInstance().write(map,
      "test-output/algorithms/WayMatchStringMergerTestSplit.osm");

    HOOT_FILE_EQUALS("test-files/algorithms/WayMatchStringMergerTestSplitExpected.osm",
      "test-output/algorithms/WayMatchStringMergerTestSplit.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayMatchStringMergerTest, "quick");

}
