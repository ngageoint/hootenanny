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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/index/KnnWayIterator.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>
#include <hoot/core/io/OsmJsonReader.h>

// Qt
#include <QTime>

// TGS
#include <tgs/RStarTree/KnnIterator.h>
#include <tgs/RStarTree/RStarTreePrinter.h>
#include <tgs/Statistics/Random.h>
using namespace Tgs;

using namespace geos::geom;
using namespace std;

namespace hoot
{

class OsmMapTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmMapTest);
  CPPUNIT_TEST(runCopyTest);
  CPPUNIT_TEST(runFindWayNeighbors);
  CPPUNIT_TEST(runNnTest);
  CPPUNIT_TEST(runAppendTest);
  CPPUNIT_TEST(runAppendDuplicateNodeTest);
  CPPUNIT_TEST(runAppendDuplicateWayTest);
  CPPUNIT_TEST(runAppendDuplicateRelationTest);
  CPPUNIT_TEST(runAppendDifferentNodeSameIdTest);
  CPPUNIT_TEST(runAppendDifferentWaySameIdTest);
  CPPUNIT_TEST(runAppendDifferentRelationSameIdTest);
  CPPUNIT_TEST(runAppendDifferentNodeSameIdSkipDupesTest);
  CPPUNIT_TEST(runAppendDifferentWaySameIdSkipDupesTest);
  CPPUNIT_TEST(runAppendDifferentRelationSameIdSkipDupesTest);
  CPPUNIT_TEST(runAppendSameMapTest);
  CPPUNIT_TEST(runAppendDifferentCoordinateSystemsTest);
  CPPUNIT_TEST(runRemoveTest);
  CPPUNIT_TEST(runReplaceListTest1);
  CPPUNIT_TEST(runReplaceListTest2);
  CPPUNIT_TEST(runReplaceListTest3);
  CPPUNIT_TEST(runReplaceNodeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmMapTest() :
  HootTestFixture("test-files/elements/OsmMapTest/", "test-output/elements/OsmMapTest/")
  {
    setResetType(ResetBasic);
  }

  void _checkKnnWayIterator(OsmMapPtr map)
  {
    std::shared_ptr<const HilbertRTree> tree = map->getIndex().getWayTree();

    ElementToGeometryConverter ec(map);
    const WayMap& ways = map->getWays();
    for (WayMap::const_iterator itw = ways.begin(); itw != ways.end(); ++itw)
    {
      const WayPtr& w = itw->second;
      std::shared_ptr<LineString> ls = ElementToGeometryConverter(map).convertToLineString(w);
      KnnWayIterator it(*map, w, tree.get(), map->getIndex().getTreeIdToWidMap());

      int count = 0;
      double lastDistance = 0.0;
      while (it.hasNext())
      {
        double d = ec.convertToLineString(it.getWay())->distance(ls.get());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(d, it.getDistance(), 1e-5);
        CPPUNIT_ASSERT(it.getDistance() >= lastDistance);
        lastDistance = it.getDistance();
        count++;
      }

      CPPUNIT_ASSERT_EQUAL((int)ways.size(), count);
    }
  }

  void changeMapForCopyTest(OsmMapPtr map)
  {
    map->getNode(-1669793)->setX(0);
    map->getNode(-1669793)->getTags()["foo"] = "bar";

    map->getWay(-1669801)->addNode(-1669723);

    map->getRelation(-1)->addElement(MetadataTags::RoleOuter(), ElementId::way(-1669797));
  }

  OsmMapPtr createMapForCopyTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);

    RelationPtr r(new Relation(Status::Unknown1, -1, 10, MetadataTags::RelationMultiPolygon()));
    r->addElement(MetadataTags::RoleOuter(), ElementId::way(-1669799));
    map->addRelation(r);

    return map;
  }

  /**
   * OsmMap does some fanciness to do copy on write rather than always copying. Handy, but it was
   * a bit buggy. This tests for those bugs.
   */
  void runCopyTest()
  {
    OsmMapPtr map = createMapForCopyTest();
    OsmMapPtr copy = OsmMapPtr(new OsmMap(map));

    QString nodePreChange = copy->getNode(-1669793)->toString();
    QString wayPreChange = copy->getWay(-1669801)->toString();
    QString relationPreChange = copy->getRelation(-1)->toString();

    // change the original
    changeMapForCopyTest(map);

    HOOT_STR_EQUALS(nodePreChange, copy->getNode(-1669793)->toString());
    HOOT_STR_EQUALS(wayPreChange, copy->getWay(-1669801)->toString());
    HOOT_STR_EQUALS(relationPreChange, copy->getRelation(-1)->toString());

    // now change the copy
    map = createMapForCopyTest();
    copy = OsmMapPtr(new OsmMap(map));
    changeMapForCopyTest(copy);

    HOOT_STR_EQUALS(nodePreChange, map->getNode(-1669793)->toString());
    HOOT_STR_EQUALS(wayPreChange, map->getWay(-1669801)->toString());
    HOOT_STR_EQUALS(relationPreChange, map->getRelation(-1)->toString());
  }

  void runAppendTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    mapA->append(mapB);

    MapProjector::projectToWgs84(mapA);

    OsmXmlWriter writer;
    writer.write(mapA, _outputPath + "OsmMapAppendTest.osm");
    HOOT_FILE_EQUALS(_inputPath + "OsmMapAppendTest.osm", _outputPath + "OsmMapAppendTest.osm");
  }

  void runAppendDuplicateNodeTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    const size_t sizeMapAPlusMapBBefore = mapA->getElementCount() + mapB->getElementCount();

    // Since the element is an exact duplicate with the same ID, the map will skip appending it.
    ElementPtr duplicateNode(mapA->getNode(-1669765)->clone());
    mapB->addElement(duplicateNode);

    mapA->append(mapB);
    CPPUNIT_ASSERT_EQUAL(1, mapA->numNodesSkippedForAppending());
    CPPUNIT_ASSERT_EQUAL(sizeMapAPlusMapBBefore, mapA->getElementCount());
  }

  void runAppendDuplicateWayTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    const size_t sizeMapAPlusMapBBefore = mapA->getElementCount() + mapB->getElementCount();

    // Since the element is an exact duplicate with the same ID, the map will skip appending it.
    ElementPtr duplicateWay(mapA->getWay(-1669801)->clone());
    mapB->addElement(duplicateWay);

    mapA->append(mapB);
    CPPUNIT_ASSERT_EQUAL(1, mapA->numWaysSkippedForAppending());
    CPPUNIT_ASSERT_EQUAL(sizeMapAPlusMapBBefore, mapA->getElementCount());
  }

  void runAppendDuplicateRelationTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    RelationPtr relation(new Relation(Status::Unknown1, -1, 15.0));
    relation->addElement(
      "", mapA->getWay(ElementIdsVisitor::findElementsByTag(mapA, ElementType::Way, "note", "1")[0]));
    mapA->addRelation(relation);

    const size_t sizeMapAPlusMapBBefore = mapA->getElementCount() + mapB->getElementCount();

    ElementPtr duplicatedRelation(mapA->getRelation(relation->getId())->clone());
    mapB->addElement(duplicatedRelation);

    mapA->append(mapB);
    CPPUNIT_ASSERT_EQUAL(1, mapA->numRelationsSkippedForAppending());
    CPPUNIT_ASSERT_EQUAL(sizeMapAPlusMapBBefore, mapA->getElementCount());
  }

  void runAppendDifferentNodeSameIdTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    // Since the element is not an exact duplicate and has the same ID, no appending will occur
    // and an error will be thrown.
    ElementPtr nearDuplicateNode(mapA->getNode(-1669765)->clone());
    nearDuplicateNode->getTags().set("runAppendDifferentNodeSameIdTest", true);
    mapB->addElement(nearDuplicateNode);

    QString exceptionMsg("");
    try
    {
      mapA->append(mapB);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Map already contains"));
  }

  void runAppendDifferentWaySameIdTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    // Since the element is not an exact duplicate and has the same ID, no appending will occur
    // and an error will be thrown.
    ElementPtr nearDuplicateWay(mapA->getWay(-1669801)->clone());
    nearDuplicateWay->getTags().set("runAppendDifferentWaySameIdTest", true);
    mapB->addElement(nearDuplicateWay);

    QString exceptionMsg("");
    try
    {
      mapA->append(mapB);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Map already contains"));
  }

  void runAppendDifferentRelationSameIdTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    RelationPtr relation(new Relation(Status::Unknown1, -1, 15.0));
    relation->addElement(
      "", mapA->getWay(ElementIdsVisitor::findElementsByTag(mapA, ElementType::Way, "note", "1")[0]));
    mapA->addRelation(relation);

    // Since the element is not an exact duplicate and has the same ID, no appending will occur
    // and an error will be thrown.
    ElementPtr nearDuplicateRelation(mapA->getRelation(relation->getId())->clone());
    nearDuplicateRelation->getTags().set("runAppendDifferentRelationSameIdTest", true);
    mapB->addElement(nearDuplicateRelation);

    QString exceptionMsg("");
    try
    {
      mapA->append(mapB);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Map already contains"));
  }

  void runAppendDifferentNodeSameIdSkipDupesTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    const size_t sizeMapAPlusMapBBefore = mapA->getElementCount() + mapB->getElementCount();

    // Since the element is not an exact duplicate, has the same ID, and the throwOutDupes option is
    // enabled, the element will be skipped for appending and no error will be thrown.
    ElementPtr nearDuplicateNode(mapA->getNode(-1669765)->clone());
    nearDuplicateNode->getTags().set("runAppendDifferentWaySameIdTest", true);
    mapB->addElement(nearDuplicateNode);

    mapA->append(mapB, true);
    CPPUNIT_ASSERT_EQUAL(1, mapA->numNodesSkippedForAppending());
    CPPUNIT_ASSERT_EQUAL(sizeMapAPlusMapBBefore, mapA->getElementCount());
  }

  void runAppendDifferentWaySameIdSkipDupesTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    const size_t sizeMapAPlusMapBBefore = mapA->getElementCount() + mapB->getElementCount();

    // Since the element is not an exact duplicate, has the same ID, and the throwOutDupes option is
    // enabled, the element will be skipped for appending and no error will be thrown.
    ElementPtr nearDuplicateWay(mapA->getWay(-1669801)->clone());
    nearDuplicateWay->getTags().set("runAppendDifferentWaySameIdTest", true);
    mapB->addElement(nearDuplicateWay);

    mapA->append(mapB, true);
    CPPUNIT_ASSERT_EQUAL(1, mapA->numWaysSkippedForAppending());
    CPPUNIT_ASSERT_EQUAL(sizeMapAPlusMapBBefore, mapA->getElementCount());
  }

  void runAppendDifferentRelationSameIdSkipDupesTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    RelationPtr relation(new Relation(Status::Unknown1, -1, 15.0));
    relation->addElement(
      "", mapA->getWay(ElementIdsVisitor::findElementsByTag(mapA, ElementType::Way, "note", "1")[0]));
    mapA->addRelation(relation);

    const size_t sizeMapAPlusMapBBefore = mapA->getElementCount() + mapB->getElementCount();

    // Since the element is not an exact duplicate, has the same ID, and the throwOutDupes option is
    // enabled, the element will be skipped for appending and no error will be thrown.
    ElementPtr nearDuplicateRelation(mapA->getRelation(relation->getId())->clone());
    nearDuplicateRelation->getTags().set("runAppendDifferentRelationSameIdTest", true);
    mapB->addElement(nearDuplicateRelation);

    mapA->append(mapB, true);
    CPPUNIT_ASSERT_EQUAL(1, mapA->numRelationsSkippedForAppending());
    CPPUNIT_ASSERT_EQUAL(sizeMapAPlusMapBBefore, mapA->getElementCount());
  }

  void runAppendSameMapTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    QString exceptionMsg = "<wrong>";
    try
    {
      mapA->append(mapA);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS(exceptionMsg, "Can't append to the same map.");
  }

  void runAppendDifferentCoordinateSystemsTest()
  {
    OsmXmlReader reader;
    reader.setUseDataSourceIds(true);

    reader.setDefaultStatus(Status::Unknown1);
    OsmMapPtr mapA(new OsmMap());
    reader.read("test-files/ToyTestA.osm", mapA);

    reader.setDefaultStatus(Status::Unknown2);
    OsmMapPtr mapB(new OsmMap());
    reader.read("test-files/ToyTestB.osm", mapB);

    MapProjector::projectToPlanar(mapB);

    QString exceptionMsg;
    try
    {
      mapA->append(mapB);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Incompatible maps"));
  }

  void runFindWayNeighbors()
  {
    OsmXmlReader reader;

    QTime t;
    t.start();

    LOG_INFO("Reading file...");

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);

    LOG_INFO("Finished reading file. " << t.elapsed() << "ms");
    LOG_INFO("Map size: " << map->getWays().size());

    t.restart();

    MapProjector::projectToOrthographic(map);
    const WayMap& ways = map ->getWays();

    LOG_INFO("Finished reprojecting. " << t.elapsed() << "ms");

    t.restart();

    // build the tree before the benchmark.
    map->getIndex().getWayTree();
    if (Log::getInstance().isDebugEnabled())
    {
      RStarTreePrinter::print(map->getIndex().getWayTree());
    }

    LOG_INFO("Index build elapsed: " << t.elapsed() << "ms");

    t.restart();
    int i = 0;
    for (WayMap::const_iterator itw = ways.begin(); itw != ways.end() && i < 20; ++itw)
    {
      const WayPtr& w = itw->second;
      std::vector<long> wids = map->getIndex().findWayNeighbors(w, 30.0);
      i++;
    }

    LOG_INFO("Time per way (indexed): " << (double)t.elapsed() / (double)i << "ms");

    t.restart();
    i = 0;
    for (WayMap::const_iterator itw = ways.begin(); itw != ways.end() && i < 20; ++itw)
    {
      WayPtr w = itw->second;
      std::vector<long> wids = map->getIndex().findWayNeighborsBruteForce(w, 30.0);
      i++;
    }

    LOG_INFO("Time per way (brute force): " << (double)t.elapsed() / (double)i << "ms");

  }

  void runNnTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);

    MapProjector::projectToOrthographic(map);

    std::shared_ptr<const HilbertRTree> tree = map->getIndex().getWayTree();

    for (int i = 0; i < 10; i++)
    {
      KnnIterator it(tree.get(),
                     Tgs::Random::instance()->generateInt() % 200 - 100,
                     Tgs::Random::instance()->generateInt() % 200 - 100);

      double lastDistance = 0.0;
      while (it.hasNext())
      {
        CPPUNIT_ASSERT(it.getDistance() >= lastDistance);
        lastDistance = it.getDistance();
      }
    }

    _checkKnnWayIterator(map);
  }

  void runRemoveTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);

    MapProjector::projectToOrthographic(map);

    // force it to build the tree before we start removing nodes.
    map->getIndex().getWayTree();

    RemoveWayByEid::removeWay(
      map, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "0")[0]);
    _checkKnnWayIterator(map);

    RemoveWayByEid::removeWay(
      map, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "1")[0]);
    _checkKnnWayIterator(map);

    RemoveWayByEid::removeWay(
      map, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "2")[0]);
    _checkKnnWayIterator(map);

    RemoveWayByEid::removeWay(
      map, ElementIdsVisitor::findElementsByTag(map, ElementType::Way, "note", "3")[0]);
    _checkKnnWayIterator(map);
  }

  /**
   * Test to verify ways get replaced in a relation properly and the replaced way (w1) is removed.
   */
  void runReplaceListTest1()
  {
    OsmMapPtr map(new OsmMap());
    RelationPtr r1(new Relation(Status::Unknown1, 1, 15));
    WayPtr w1(new Way(Status::Unknown1, 1, 15));
    WayPtr w2(new Way(Status::Unknown1, 2, 15));
    WayPtr w3(new Way(Status::Unknown1, 3, 15));
    map->addElement(r1);
    map->addElement(w1);
    map->addElement(w2);
    map->addElement(w3);

    r1->addElement("foo", w2->getElementId());
    r1->addElement("bar", w1->getElementId());
    r1->addElement("lucky", w1->getElementId());

    QList<ElementPtr> newWays;
    newWays.append(w2);
    newWays.append(w3);

    map->replace(w1, newWays);

    const QString testFileName = "runReplaceListTest1.json";
    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  /**
   * Verify a way is replaced by nodes.
   */
  void runReplaceListTest2()
  {
    OsmMapPtr map(new OsmMap());
    WayPtr w1(new Way(Status::Unknown1, 1, 15));
    NodePtr n1(new Node(Status::Unknown1, 1, 0, 0, 15));
    NodePtr n2(new Node(Status::Unknown1, 2, 0, 0, 15));
    NodePtr n3(new Node(Status::Unknown1, 3, 0, 0, 15));
    map->addElement(w1);
    map->addElement(n1);

    w1->addNode(1);

    QList<ElementPtr> newNodes;
    newNodes.append(n2);
    newNodes.append(n3);

    map->replace(w1, newNodes);

    const QString testFileName = "runReplaceListTest2.json";
    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.open(_outputPath + testFileName);
    MapProjector::projectToWgs84(map);
    writer.write(map);
    writer.close();
    HOOT_FILE_EQUALS(_inputPath + testFileName, _outputPath + testFileName);
  }

  /**
   * Verify a way is replaced by nodes.
   */
  void runReplaceListTest3()
  {
    OsmMapPtr map(new OsmMap());
    WayPtr w1(new Way(Status::Unknown1, 1, 15));
    WayPtr w2(new Way(Status::Unknown1, 2, 15));
    NodePtr n1(new Node(Status::Unknown1, 1, 0, 0, 15));
    NodePtr n2(new Node(Status::Unknown1, 2, 0, 0, 15));
    NodePtr n3(new Node(Status::Unknown1, 3, 0, 0, 15));
    map->addElement(w1);
    map->addElement(n1);

    w1->addNode(1);

    QList<ElementPtr> badNodes;
    badNodes.append(n2);
    badNodes.append(n3);

    // we can't replace a node that is part of a way with multiple nodes.
    CPPUNIT_ASSERT_THROW(map->replace(n1, badNodes), HootException);

    QList<ElementPtr> badWay;
    badWay.append(w2);
    badWay.append(n2);

    // we can't replace a node that is part of a way with a way.
    CPPUNIT_ASSERT_THROW(map->replace(n1, badWay), HootException);

    QList<ElementPtr> newNode;
    newNode.append(n2);

    map->replace(n1, newNode);

    OsmJsonWriter writer;
    writer.setIncludeCompatibilityTags(false);
    const QString actual = writer.toString(map);
    HOOT_STR_EQUALS("{\"version\": 0.6,\"generator\": \"Hootenanny\",\"elements\": [\n"
      "{\"type\":\"way\",\"id\":1,\"nodes\":[2],\"tags\":{\"" + MetadataTags::ErrorCircular() + "\":\"15\"}}]\n"
      "}\n",
      actual);
    CPPUNIT_ASSERT(OsmJsonReader().isValidJson(actual));
  }

  void runReplaceNodeTest()
  {
    OsmXmlReader reader;

    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/ToyTestA.osm", map);

    // Sample data does not have any relations, have to add some with nodes in them
    RelationPtr relations[5] =
    {
      RelationPtr(new Relation(Status::Unknown1, 100, 3.0, "relationtype0")),
      RelationPtr(new Relation(Status::Unknown1, 101, 4.1, "relationtype1")),
      RelationPtr(new Relation(Status::Unknown1, 102, 5.2, "relationtype2")),
      RelationPtr(new Relation(Status::Unknown1, 103, 6.3, "relationtype3")),
      RelationPtr(new Relation(Status::Unknown1, 104, 7.4, "relationtype4")),
    };

    // Add relations to the map
    for (int i = 0; i < 5; i++)
    {
      relations[i]->addElement("correlated_streetlight", ElementId::node(-10 - i));
      relations[i]->addElement("correlated_streetlight", ElementId::node(-11 - i));
      LOG_TRACE(relations[i]->toString());
      map->addRelation(relations[i]);
    }

    WayMap waysBeforeReplacement = map->getWays();
    LOG_TRACE("Ways before replacement:");
    for (WayMap::const_iterator iterator = waysBeforeReplacement.begin();
         iterator != waysBeforeReplacement.end(); ++iterator)
    {
      LOG_VART(iterator->second->getNodeIds());
    }

    // Replace selected nodes
    for (int i = -2; i > -22; i -= 2)
    {
      LOG_TRACE("Replacing node: " << i << " with: " << (-10 + i) << "...");
      map->replaceNode(i, -10 + i);
    }

    // Original data had nodes -1 through -36.  Make sure that even-numbered nodes -2 through
    //  -20 are gone

    const NodeMap nodes = map->getNodes();
    CPPUNIT_ASSERT_EQUAL(26, (int)nodes.size());
    for (NodeMap::const_iterator nodeIter = nodes.begin(); nodeIter != nodes.end(); ++nodeIter)
    {
      const ConstNodePtr n = nodeIter->second;
      LOG_TRACE("Node: " << n->getId());
      CPPUNIT_ASSERT((n->getId() >= -36) && (n->getId() <= -1));

      // If it's in the range where nodes were replaced, make sure only ones left are odd IDs
      if (n->getId() >= -21)
      {
        LOG_TRACE("Even test on negative number: " << (n->getId() % 2) );
        CPPUNIT_ASSERT((n->getId() % 2) == -1);
      }
    }

    // Make sure replacement did correct thing with ways
    WayMap ways = map->getWays();
    CPPUNIT_ASSERT(4 == ways.size());

    int i = 1;
    for (WayMap::const_iterator iterator = ways.begin(); iterator != ways.end(); ++iterator)
    {
      WayPtr way = iterator->second;
      LOG_TRACE(way->toString());
      std::vector<long> nodeIds = way->getNodeIds();
      CPPUNIT_ASSERT((-5 + i) == way->getId());
      if (i == 1)
      {
        const size_t correctSize = 4;
        CPPUNIT_ASSERT(nodeIds.size() == correctSize);
        long correctIds[correctSize] = { -35, -34, -33, -30 };
        std::vector<long> correctIdVector(correctIds, correctIds + sizeof(correctIds) / sizeof(long));
        CPPUNIT_ASSERT(correctIdVector == nodeIds);
      }
      else if (i == 2)
      {
        const size_t correctSize = 30;
        CPPUNIT_ASSERT( nodeIds.size() == correctSize );
        // really there shouldn't be duplicated way nodes in the way; see comments in
        // Way::replaceNode
        long correctIds[correctSize] = { -3, -24, -5, -26, -7, -32, -31, -30, -36, -29, -28, -27, -26,
                                         -25, -24, -23, -22, -21, -30, -19, -28, -17, -26, -15, -24,
                                         -13, -22, -11, -30, -9 };
        std::vector<long> correctIdVector(correctIds, correctIds + sizeof(correctIds) / sizeof(long));
        CPPUNIT_ASSERT(correctIdVector == nodeIds);
      }
      else if (i == 3)
      {
        const size_t correctSize = 3;
        CPPUNIT_ASSERT(nodeIds.size() == correctSize);
        long correctIds[correctSize] = { -33, -28, -7 };
        std::vector<long> correctIdVector(correctIds, correctIds + sizeof(correctIds) / sizeof(long));
        CPPUNIT_ASSERT(correctIdVector == nodeIds);
      }
      else if (i == 4)
      {
        const size_t correctSize = 3;
        CPPUNIT_ASSERT(nodeIds.size() == correctSize);
        long correctIds[correctSize] = { -32, -22, -1 };
        std::vector<long> correctIdVector(correctIds, correctIds + sizeof(correctIds) / sizeof(long));
        CPPUNIT_ASSERT(correctIdVector == nodeIds);
      }

      i++;
    }

    // Make sure relations were updated properly
    RelationMap checkRelations = map->getRelations();
    i = 0;
    CPPUNIT_ASSERT(5 == checkRelations.size());

    for ( RelationMap::const_iterator iterator = checkRelations.begin();
          iterator != checkRelations.end(); ++iterator )
    {
      RelationPtr myRelation = iterator->second;
      //LOG_DEBUG(myRelation->toString());
      CPPUNIT_ASSERT(myRelation->getId() == 100 + i);
      //LOG_DEBUG(myRelation->getType());
      QString checkRelationship("relationtype" + QString::number(i) );
      //LOG_DEBUG("Checktype: " << checkRelationship);
      CPPUNIT_ASSERT(myRelation->getType() == checkRelationship);
      std::vector<RelationData::Entry> entries = myRelation->getMembers();
      CPPUNIT_ASSERT(entries.size() == 2);\

      switch (i)
      {
      case 0:
        CPPUNIT_ASSERT(entries[0].getElementId() == ElementId::node(-30));
        CPPUNIT_ASSERT(entries[1].getElementId() == ElementId::node(-11));
        break;

      case 1:
        CPPUNIT_ASSERT(entries[0].getElementId() == ElementId::node(-11));
        CPPUNIT_ASSERT(entries[1].getElementId() == ElementId::node(-22));
        break;

      case 2:
        CPPUNIT_ASSERT(entries[0].getElementId() == ElementId::node(-22));
        CPPUNIT_ASSERT(entries[1].getElementId() == ElementId::node(-13));
        break;

      case 3:
        CPPUNIT_ASSERT(entries[0].getElementId() == ElementId::node(-13));
        CPPUNIT_ASSERT(entries[1].getElementId() == ElementId::node(-24));
        break;

      case 4:
        CPPUNIT_ASSERT(entries[0].getElementId() == ElementId::node(-24));
        CPPUNIT_ASSERT(entries[1].getElementId() == ElementId::node(-15));
        break;
      }

      i++;
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmMapTest, "quick");

}
