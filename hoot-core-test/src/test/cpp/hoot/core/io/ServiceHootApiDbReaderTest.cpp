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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/io/HootApiDbWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/RemoveAttributeVisitor.h>
#include <hoot/core/elements/ElementAttributeType.h>

// Qt
#include <QDir>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"


namespace hoot
{

class ServiceHootApiDbReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ServiceHootApiDbReaderTest);
  CPPUNIT_TEST(runCalculateBoundsTest);
  CPPUNIT_TEST(runElementIdTest);
  CPPUNIT_TEST(runUrlMissingMapIdTest);
  CPPUNIT_TEST(runUrlInvalidMapIdTest);
  CPPUNIT_TEST(runReadTest);
  CPPUNIT_TEST(runPartialReadTest);
  CPPUNIT_TEST(runFactoryReadTest);
  CPPUNIT_TEST(runReadWithElemTest);
  CPPUNIT_TEST(runReadByBoundsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceHootApiDbReaderTest@hoottestcpp.org"; }

  long mapId;

  void setUp()
  {
    mapId = -1;
    ServicesDbTestUtils::deleteUser(userEmail());
    HootApiDb database;

    database.open(ServicesDbTestUtils::getDbModifyUrl());
    database.getOrCreateUser(userEmail(), "ServiceHootApiDbReaderTest");
    database.close();
  }

  void tearDown()
  {
    ServicesDbTestUtils::deleteUser(userEmail());

    if (mapId != -1)
    {
      HootApiDb database;
      database.open(ServicesDbTestUtils::getDbModifyUrl());
      database.deleteMap(mapId);
      database.close();
    }
  }

  long populateMap()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());

    boost::shared_ptr<Node> n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    map->addNode(n1);
    boost::shared_ptr<Node> n2(new Node(Status::Unknown2, 2, 0.1, 0.0, 11.0));
    n2->setTag("noteb", "n2b");
    map->addNode(n2);
    boost::shared_ptr<Node> n3(new Node(Status::Conflated, 3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3");
    map->addNode(n3);
    boost::shared_ptr<Node> n4(new Node(Status::Conflated, 4, 0.3, 0.0, 13.0));
    n4->setTag("note", "n4");
    map->addNode(n4);
    boost::shared_ptr<Node> n5(new Node(Status::Invalid, 5, 0.4, 0.0, 14.0));
    map->addNode(n5);

    boost::shared_ptr<Way> w1(new Way(Status::Unknown1, 1, 15.0));
    w1->addNode(1);
    w1->addNode(2);
    w1->setTag("noteb", "w1b");
    map->addWay(w1);
    boost::shared_ptr<Way> w2(new Way(Status::Unknown2, 2, 16.0));
    w2->addNode(2);
    w2->addNode(3);
    w2->setTag("note", "w2");
    map->addWay(w2);
    boost::shared_ptr<Way> w3(new Way(Status::Unknown2, 3, 17.0));
    w3->addNode(2);
    map->addWay(w3);

    boost::shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 18.1, "collection"));
    r1->addElement("n1", n1->getElementId());
    r1->addElement("w1", w1->getElementId());
    r1->setTag("note", "r1");
    map->addRelation(r1);
    boost::shared_ptr<Relation> r2(new Relation(Status::Unknown1, 2, -1.0));
    r2->addElement("n2", n2->getElementId());
    map->addRelation(r2);

    HootApiDbWriter writer;
    writer.setUserEmail(userEmail());
    writer.setRemap(false);
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    writer.write(map);
    writer.close();
    return writer.getMapId();
  }

  long insertDataForBoundTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/io/ServiceHootApiDbReaderTest/runReadByBoundsTestInput.osm", false,
      Status::Unknown1);

    HootApiDbWriter writer;
    writer.setUserEmail(userEmail());
    writer.setRemap(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl().toString());
    writer.write(map);
    writer.close();
    return writer.getMapId();
  }

  template<typename T>
  vector<long> getKeys(T begin, T end)
  {
    vector<long> result;
    for (;begin != end; ++begin)
    {
      result.push_back(begin->first);
    }
    return result;
  }

  void runCalculateBoundsTest()
  {
    mapId = populateMap();

    HootApiDbReader reader;
    QString url = ServicesDbTestUtils::getDbReadUrl(mapId).toString();
    reader.open(url);
    HOOT_STR_EQUALS("Env[0:0.4,0:0]", reader.calculateEnvelope().toString());
  }

  void runElementIdTest()
  {
    mapId = populateMap();

    HootApiDbReader reader;
    // make sure all the element ids start with -1
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setUseDataSourceIds(false);
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId).toString());
    reader.read(map);

    HOOT_STR_EQUALS("[5]{-5, -4, -3, -2, -1}",
      getKeys(map->getNodes().begin(), map->getNodes().end()));
    HOOT_STR_EQUALS("[2]{-2, -1}",
      getKeys(map->getRelations().begin(), map->getRelations().end()));
    HOOT_STR_EQUALS("[3]{-3, -2, -1}", getKeys(map->getWays().begin(), map->getWays().end()));

    HOOT_STR_EQUALS("[1]{-2}", map->getWay(-3)->getNodeIds());
    HOOT_STR_EQUALS("[2]{-2, -3}", map->getWay(-2)->getNodeIds());
    HOOT_STR_EQUALS("[2]{-1, -2}", map->getWay(-1)->getNodeIds());
    HOOT_STR_EQUALS("[1]{Entry: role: n2, eid: Node:-2}", map->getRelation(-2)->getMembers());
    HOOT_STR_EQUALS("[2]{Entry: role: n1, eid: Node:-1, Entry: role: w1, eid: Way:-1}",
      map->getRelation(-1)->getMembers());
  }

  void runUrlMissingMapIdTest()
  {
    // temporarily disable logging to avoid isValid warning
    DisableLog dl;

    HootApiDbReader reader;
    QString exceptionMsg("");
    try
    {
      reader.open(
        ServicesDbTestUtils::getDbReadUrl(mapId).toString().replace("/" + QString::number(mapId), ""));
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }

    //I would rather this return: "URL does not contain valid map ID." from
    //HootApiDbReader::open
    CPPUNIT_ASSERT(exceptionMsg.contains("An unsupported URL was passed in"));
  }

  void runUrlInvalidMapIdTest()
  {
    HootApiDbReader reader;
    QString exceptionMsg("");
    const long invalidMapId = mapId + 1;
    try
    {
      reader.open(
        ServicesDbTestUtils::getDbReadUrl(mapId).toString().replace(
          "/" + QString::number(mapId), "/" + QString::number(invalidMapId)));
    }
    catch (HootException e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("No map exists with ID: " + QString::number(invalidMapId)).toStdString(), exceptionMsg.toStdString());
  }

  void verifyFullReadOutput(boost::shared_ptr<OsmMap> map)
  {
    //nodes

    CPPUNIT_ASSERT_EQUAL(5, (int)map->getNodes().size());

    boost::shared_ptr<Node> node = map->getNode(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(10.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("1", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.1, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(11.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n2b", node->getTags().get("noteb"));
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("2", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(3);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.2, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n3", node->getTags().get("note"));
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("3", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(4);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)4, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.3, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(13.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n4", node->getTags().get("note"));
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("4", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(5);
    CPPUNIT_ASSERT_EQUAL(Status::Invalid, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)5, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.4, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(14.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("5", node->getTags().get(MetadataTags::HootId()));

    //ways

    CPPUNIT_ASSERT_EQUAL(3, (int)map->getWays().size());

    boost::shared_ptr<Way> way = map->getWay(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(1));
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(2, way->getTags().size());
    HOOT_STR_EQUALS("w1b", way->getTags().get("noteb"));
    CPPUNIT_ASSERT_EQUAL((long)1, way->getVersion());
    CPPUNIT_ASSERT(way->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("1", way->getTags().get(MetadataTags::HootId()));

    way = map->getWay(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, way->getId());
    CPPUNIT_ASSERT_EQUAL(16.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT(way->hasNode(3));
    CPPUNIT_ASSERT_EQUAL(2, way->getTags().size());
    HOOT_STR_EQUALS("w2", way->getTags().get("note"));
    CPPUNIT_ASSERT_EQUAL((long)1, way->getVersion());
    CPPUNIT_ASSERT(way->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("2", way->getTags().get(MetadataTags::HootId()));

    way = map->getWay(3);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, way->getId());
    CPPUNIT_ASSERT_EQUAL(17.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getVersion());
    CPPUNIT_ASSERT(way->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("3", way->getTags().get(MetadataTags::HootId()));

    //relations

    CPPUNIT_ASSERT_EQUAL(2, (int)map->getRelations().size());

    boost::shared_ptr<Relation> relation = map->getRelation(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    CPPUNIT_ASSERT_EQUAL(18.1, relation->getCircularError());
    HOOT_STR_EQUALS("collection", relation->getType());
    vector<RelationData::Entry> relationMembers = relation->getMembers();
    CPPUNIT_ASSERT_EQUAL(size_t(2), relationMembers.size());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(1)));
    CPPUNIT_ASSERT(relation->contains(ElementId::way(1)));
    RelationData::Entry member = relationMembers.at(0);
    HOOT_STR_EQUALS("n1", member.role);
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    member = relationMembers.at(1);
    HOOT_STR_EQUALS("w1", member.role);
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(3, relation->getTags().size());
    HOOT_STR_EQUALS("r1", relation->getTags().get("note"));
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getVersion());
    CPPUNIT_ASSERT(relation->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("1", relation->getTags().get(MetadataTags::HootId()));

    relation = map->getRelation(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, relation->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
    HOOT_STR_EQUALS("", relation->getType());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(2)));
    CPPUNIT_ASSERT_EQUAL(size_t(1), relation->getMembers().size());
    member = relation->getMembers().at(0);
    HOOT_STR_EQUALS("n2", member.role);
    CPPUNIT_ASSERT_EQUAL((long)2, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(1, relation->getTags().size());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getVersion());
    CPPUNIT_ASSERT(relation->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("2", relation->getTags().get(MetadataTags::HootId()));
  }

  void verifySingleReadOutput(boost::shared_ptr<OsmMap> map)
  {
    //nodes

    CPPUNIT_ASSERT_EQUAL(5, (int)map->getNodes().size());

    boost::shared_ptr<Node> node = map->getNode(3);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.2, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n3", node->getTags().get("note"));
    HOOT_STR_EQUALS("3", node->getTags().get(MetadataTags::HootId()));
  }

  void runReadTest()
  {
    mapId = populateMap();

    HootApiDbReader reader;
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId).toString());
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();
  }

  void runReadWithElemTest()
  {
    mapId = populateMap();

    HootApiDbReader reader;
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId,3,"node").toString());
    reader.read(map);
    verifySingleReadOutput(map);
    reader.close();
  }

  void runFactoryReadTest()
  {
    mapId = populateMap();

    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMapReaderFactory::read(map, ServicesDbTestUtils::getDbReadUrl(mapId).toString());
    verifyFullReadOutput(map);
  }

  void runPartialReadTest()
  {
    mapId = populateMap();

    HootApiDbReader reader;
    const int chunkSize = 3;
    reader.setMaxElementsPerMap(chunkSize);
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId).toString());
    reader.initializePartial();

    int ctr = 0;
    boost::shared_ptr<OsmMap> map(new OsmMap());

    //3 nodes

    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);

    CPPUNIT_ASSERT_EQUAL(
      chunkSize,
      (int)(map->getNodes().size() + map->getWays().size() + map->getRelations().size()));
    CPPUNIT_ASSERT_EQUAL(3, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    boost::shared_ptr<Node> node = map->getNode(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(10.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    HOOT_STR_EQUALS("1", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.1, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(11.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n2b", node->getTags().get("noteb"));
    HOOT_STR_EQUALS("2", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(3);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.2, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n3", node->getTags().get("note"));
    HOOT_STR_EQUALS("3", node->getTags().get(MetadataTags::HootId()));

    ctr++;

    //2 nodes, 1 way

    map.reset(new OsmMap());
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(
      chunkSize,
      (int)(map->getNodes().size() + map->getWays().size() + map->getRelations().size()));
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    node = map->getNode(4);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)4, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.3, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(13.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n4", node->getTags().get("note"));
    HOOT_STR_EQUALS("4", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(5);
    CPPUNIT_ASSERT_EQUAL(Status::Invalid, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)5, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.4, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(14.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    HOOT_STR_EQUALS("5", node->getTags().get(MetadataTags::HootId()));

    boost::shared_ptr<Way> way = map->getWay(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(1));
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(2, way->getTags().size());
    HOOT_STR_EQUALS("w1b", way->getTags().get("noteb"));
    HOOT_STR_EQUALS("1", way->getTags().get(MetadataTags::HootId()));

    ctr++;

    //2 ways, 1 relation

    map.reset(new OsmMap());
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(
      chunkSize,
      (int)(map->getNodes().size() + map->getWays().size() + map->getRelations().size()));
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());

    way = map->getWay(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, way->getId());
    CPPUNIT_ASSERT_EQUAL(16.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT(way->hasNode(3));
    CPPUNIT_ASSERT_EQUAL(2, way->getTags().size());
    HOOT_STR_EQUALS("w2", way->getTags().get("note"));
    HOOT_STR_EQUALS("2", way->getTags().get(MetadataTags::HootId()));

    way = map->getWay(3);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, way->getId());
    CPPUNIT_ASSERT_EQUAL(17.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    HOOT_STR_EQUALS("3", way->getTags().get(MetadataTags::HootId()));

    boost::shared_ptr<Relation> relation = map->getRelation(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    CPPUNIT_ASSERT_EQUAL(18.1, relation->getCircularError());
    HOOT_STR_EQUALS("collection", relation->getType());
    CPPUNIT_ASSERT_EQUAL(size_t(2), relation->getMembers().size());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(1)));
    CPPUNIT_ASSERT(relation->contains(ElementId::way(1)));
    RelationData::Entry member = relation->getMembers().at(0);
    HOOT_STR_EQUALS("n1", member.role);
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    member = relation->getMembers().at(1);
    HOOT_STR_EQUALS("w1", member.role);
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(3, relation->getTags().size());
    HOOT_STR_EQUALS("r1", relation->getTags().get("note"));
    HOOT_STR_EQUALS("1", relation->getTags().get(MetadataTags::HootId()));

    ctr++;

    //1 relation

    map.reset(new OsmMap());
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(
      1,
      (int)(map->getNodes().size() + map->getWays().size() + map->getRelations().size()));
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());

    relation = map->getRelation(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, relation->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
    HOOT_STR_EQUALS("", relation->getType());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(2)));
    CPPUNIT_ASSERT_EQUAL(size_t(1), relation->getMembers().size());
    member = relation->getMembers().at(0);
    HOOT_STR_EQUALS("n2", member.role);
    CPPUNIT_ASSERT_EQUAL((long)2, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(1, relation->getTags().size());
    HOOT_STR_EQUALS("2", relation->getTags().get(MetadataTags::HootId()));

    ctr++;

    CPPUNIT_ASSERT(!reader.hasMoreElements());
    reader.finalizePartial();

    CPPUNIT_ASSERT_EQUAL(4, ctr);
  }

  void runReadByBoundsTest()
  {
    mapId = insertDataForBoundTest();

    HootApiDbReader reader;
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId).toString());

    reader.setBoundingBox(
      "-78.02265434416296,38.90089748801109,-77.9224564416296,39.00085678801109");
    reader.read(map);

    //quick check to see if the element counts are off...consult the test output for more detail

    //See explanations for these assertions in ServiceOsmApiDbReaderTest::runReadByBoundsTest
    //(exact same input data)
    CPPUNIT_ASSERT_EQUAL(6, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(5, (int)map->getRelations().size());

    //We need to drop to set all the element changeset tags here to empty, which will cause them
    //to be dropped from the file output.  If they aren't dropped, they will increment with each
    //test and cause the output comparison to fail.
    QList<ElementAttributeType> types;
    types.append(ElementAttributeType(ElementAttributeType::Changeset));
    types.append(ElementAttributeType(ElementAttributeType::Timestamp));
    RemoveAttributeVisitor attrVis(types);
    map->visitRw(attrVis);

    MapProjector::projectToWgs84(map);

    QDir().mkpath("test-output/io/ServiceHootApiDbReaderTest");
    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(
      map, "test-output/io/ServiceHootApiDbReaderTest/runReadByBoundsTestOutput.osm");
    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/ServiceHootApiDbReaderTest/runReadByBoundsTestOutput.osm"),
      TestUtils::readFile("test-output/io/ServiceHootApiDbReaderTest/runReadByBoundsTestOutput.osm"));

    //just want to make sure I can read against the same data twice in a row w/o crashing and also
    //make sure I don't get the same result again for a different bounds
    reader.setBoundingBox("-1,-1,1,1");
    map.reset(new OsmMap());
    reader.read(map);

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    reader.close();
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbReaderTest, "slow");

}
