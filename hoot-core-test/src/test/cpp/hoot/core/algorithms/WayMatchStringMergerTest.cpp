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
 * @copyright Copyright (C) 2013, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/WayMatchStringMerger.h>
#include <hoot/core/algorithms/WayMatchStringSplitter.h>
#include <hoot/core/algorithms/linearreference/WayString.h>
#include <hoot/core/algorithms/linearreference/NaiveWayMatchStringMapping.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

using namespace std;

namespace hoot
{

class WayMatchStringMergerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(WayMatchStringMergerTest);
  CPPUNIT_TEST(runMergeNodeTest);
  CPPUNIT_TEST(runMergeTagsTest);
  CPPUNIT_TEST(runSplitTest);
  CPPUNIT_TEST_SUITE_END();

public:

  WayMatchStringMergerTest()
    : HootTestFixture("test-files/algorithms/WayMatchStringMergerTest/",
                      "test-output/algorithms/WayMatchStringMergerTest/")
  {
    setResetType(ResetBasic);
  }

  WayStringPtr createWayString1(OsmMapPtr map)
  {
    WayPtr w1a = getWay(map, "1a");
    WayPtr w1b = getWay(map, "1b");

    // Create matching WayStrings
    WayStringPtr str1 = std::make_shared<WayString>();
    str1->append(WaySubline(WayLocation(map, w1a, 0.0), WayLocation::createAtEndOfWay(map, w1a)));
    str1->append(WaySubline(WayLocation(map, w1b, 0.0), WayLocation::createAtEndOfWay(map, w1b)));

    return str1;
  }

  WayStringPtr createWayString2(OsmMapPtr map)
  {
    WayPtr w2a = getWay(map, "2a");
    WayPtr w2b = getWay(map, "2b");
    WayPtr w2c = getWay(map, "2c");

    WayStringPtr str2 = std::make_shared<WayString>();
    str2->append(WaySubline(WayLocation(map, w2a, 0.0), WayLocation::createAtEndOfWay(map, w2a)));
    // these segments are reversed
    str2->append(WaySubline(WayLocation::createAtEndOfWay(map, w2b), WayLocation(map, w2b, 0.0)));
    str2->append(WaySubline(WayLocation::createAtEndOfWay(map, w2c), WayLocation(map, w2c, 0.0)));

    return str2;
  }

  OsmMapPtr createTestMap(QString filename)
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, _inputPath + filename);

    MapProjector::projectToPlanar(map);

    return map;
  }

  NodePtr getNode(OsmMapPtr map, QString note)
  {
    vector<long> nids = ElementIdsVisitor::findElementsByTag(map, ElementType::Node, "note", note);
    if (nids.size() != 1)
    {
      throw HootException(QString("Expected to find 1 node, but found %1 - %2").arg(nids.size()).
        arg(note));
    }
    return map->getNode(nids[0]);
  }

  WayPtr getWay(OsmMapPtr map, QString note)
  {
    vector<long> vids = ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", note);
    if (vids.size() != 1)
    {
      throw HootException(QString("Expected to find 1 way, but found %1 - %2").arg(vids.size()).
        arg(note));
    }
    return map->getWay(vids[0]);
  }

  void runMergeNodeTest()
  {
    OsmMapPtr map = createTestMap("WayMatchStringMergerTestMergeNode.osm");

    WayMatchStringMappingPtr mapping =
      std::make_shared<NaiveWayMatchStringMapping>(createWayString1(map), createWayString2(map));

    vector<pair<ElementId, ElementId>> replaced;
    WayMatchStringMerger uut(map, mapping, replaced);

    WayMatchStringSplitter().applySplits(map, replaced, uut.getAllSublineMappings());
    uut.updateSublineMapping();

    uut.setTagMerger(TagMergerFactory::getInstance().getDefaultPtr());

    uut.mergeIntersection(getNode(map, "n0")->getElementId());
    uut.mergeNode(getNode(map, "n1")->getElementId());
    uut.mergeNode(getNode(map, "n2")->getElementId());
    uut.mergeIntersection(getNode(map, "n3")->getElementId());

    MapProjector::projectToWgs84(map);
    std::shared_ptr<OsmXmlWriter> writer = std::make_shared<OsmXmlWriter>();
    writer->setIncludeHootInfo(true);
    writer->write(map, _outputPath + "WayMatchStringMergerTestMergeNode.osm");

    HOOT_FILE_EQUALS(_inputPath + "WayMatchStringMergerTestMergeNodeExpected.osm",
                     _outputPath + "WayMatchStringMergerTestMergeNode.osm");
  }

  void runMergeTagsTest()
  {
    OsmMapPtr map = createTestMap("WayMatchStringMergerTest.osm");

    WayMatchStringMappingPtr mapping =
      std::make_shared<NaiveWayMatchStringMapping>(createWayString1(map), createWayString2(map));

    vector<pair<ElementId, ElementId>> replaced;
    WayMatchStringMerger uut(map, mapping, replaced);

    WayMatchStringSplitter().applySplits(map, replaced, uut.getAllSublineMappings());
    uut.updateSublineMapping();

    uut.setTagMerger(TagMergerFactory::getInstance().getDefaultPtr());
    uut.mergeTags();
    uut.setKeeperStatus(Status::Conflated);

    MapProjector::projectToWgs84(map);
    std::shared_ptr<OsmXmlWriter> writer = std::make_shared<OsmXmlWriter>();
    writer->setIncludeHootInfo(true);
    writer->write(map, _outputPath + "WayMatchStringMergerTestMergeTags.osm");

    HOOT_FILE_EQUALS(_inputPath + "WayMatchStringMergerTestMergeTagsExpected.osm",
                    _outputPath + "WayMatchStringMergerTestMergeTags.osm");
  }

  void runSplitTest()
  {
    OsmMapPtr map = createTestMap("WayMatchStringMergerTest.osm");

    WayMatchStringMappingPtr mapping =
      std::make_shared<NaiveWayMatchStringMapping>(
        createWayString1(map), createWayString2(map));

    vector<pair<ElementId, ElementId>> replaced;
    WayMatchStringMerger uut(map, mapping, replaced);
    WayMatchStringSplitter().applySplits(map, replaced, uut.getAllSublineMappings());

    MapProjector::projectToWgs84(map);
    OsmMapWriterFactory::write(map, _outputPath + "WayMatchStringMergerTestSplit.osm");

    HOOT_FILE_EQUALS(_inputPath + "WayMatchStringMergerTestSplitExpected.osm",
                    _outputPath + "WayMatchStringMergerTestSplit.osm");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WayMatchStringMergerTest, "quick");

}
