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
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/OsmMap.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"


namespace hoot
{

class HootApiDbReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(HootApiDbReaderTest);

  CPPUNIT_TEST(runCalculateBoundsTest);
  CPPUNIT_TEST(runElementIdTest);
  CPPUNIT_TEST(runUrlMissingMapIdTest);
  CPPUNIT_TEST(runUrlInvalidMapIdTest);
  CPPUNIT_TEST(runReadTest);
  CPPUNIT_TEST(runPartialReadTest);
  CPPUNIT_TEST(runFactoryReadTest);
  CPPUNIT_TEST(runReadWithElemTest);

  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "HootApiDbReaderTest@hoottestcpp.org"; }

  long mapId;

  void setUp()
  {
    mapId = -1;
    ServicesDbTestUtils::deleteUser(userEmail());
    HootApiDb database;

    database.open(ServicesDbTestUtils::getDbModifyUrl());
    database.getOrCreateUser(userEmail(), "HootApiDbReaderTest");
    database.close();

    //inserting a map before all of these tests isn't actually necessary (url tests) and
    //is probably slowing the test run down a little more than necessary
    mapId = populateMap();
  }

  void tearDown()
  {
    // HootApi DB
    ServicesDbTestUtils::deleteUser(userEmail());

    HootApiDb database;
    database.open(ServicesDbTestUtils::getDbModifyUrl());
    database.deleteMap(mapId);
    database.close();
  }

  long populateMap()
  {
    shared_ptr<OsmMap> map(new OsmMap());

    shared_ptr<Node> n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
    map->addNode(n1);
    shared_ptr<Node> n2(new Node(Status::Unknown2, 2, 0.1, 0.0, 11.0));
    n2->setTag("noteb", "n2b");
    map->addNode(n2);
    shared_ptr<Node> n3(new Node(Status::Conflated, 3, 0.2, 0.0, 12.0));
    n3->setTag("note", "n3");
    map->addNode(n3);
    shared_ptr<Node> n4(new Node(Status::Conflated, 4, 0.3, 0.0, 13.0));
    n4->setTag("note", "n4");
    map->addNode(n4);
    shared_ptr<Node> n5(new Node(Status::Invalid, 5, 0.4, 0.0, 14.0));
    map->addNode(n5);

    shared_ptr<Way> w1(new Way(Status::Unknown1, 1, 15.0));
    w1->addNode(1);
    w1->addNode(2);
    w1->setTag("noteb", "w1b");
    map->addWay(w1);
    shared_ptr<Way> w2(new Way(Status::Unknown2, 2, 16.0));
    w2->addNode(2);
    w2->addNode(3);
    w2->setTag("note", "w2");
    map->addWay(w2);
    shared_ptr<Way> w3(new Way(Status::Unknown2, 3, 17.0));
    w3->addNode(2);
    map->addWay(w3);

    shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 18.1, "collection"));
    r1->addElement("n1", n1->getElementId());
    r1->addElement("w1", w1->getElementId());
    r1->setTag("note", "r1");
    map->addRelation(r1);
    shared_ptr<Relation> r2(new Relation(Status::Unknown1, 2, -1.0));
    r2->addElement("n2", n2->getElementId());
    map->addRelation(r2);

    HootApiDbWriter writer;
    writer.setUserEmail(userEmail());
    writer.setRemap(false);
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
    HootApiDbReader reader;
    QString url = ServicesDbTestUtils::getDbReadUrl(mapId).toString();
    reader.open(url);
    HOOT_STR_EQUALS("Env[0:0.4,0:0]", reader.calculateEnvelope().toString());
  }

  void runElementIdTest()
  {
    HootApiDbReader reader;
    // make sure all the element ids start with -1
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    reader.setUseDataSourceIds(false);
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId).toString());
    reader.read(map);

    HOOT_STR_EQUALS("[5]{-5, -4, -3, -2, -1}",
      getKeys(map->getNodeMap().begin(), map->getNodeMap().end()));
    HOOT_STR_EQUALS("[2]{-2, -1}",
      getKeys(map->getRelationMap().begin(), map->getRelationMap().end()));
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

  void verifyFullReadOutput(shared_ptr<OsmMap> map)
  {
    //nodes

    CPPUNIT_ASSERT_EQUAL(5, (int)map->getNodeMap().size());

    shared_ptr<Node> node = map->getNode(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(10.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(0, node->getTags().size());
    QString tagValue = node->getTags().get("hoot:status");
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(10.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    node = map->getNode(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.1, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(11.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    tagValue = node->getTags().get("noteb");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("n2b").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(11.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    node = map->getNode(3);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.2, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    tagValue = node->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("n3").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    node = map->getNode(4);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)4, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.3, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(13.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    tagValue = node->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("n4").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(13.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    node = map->getNode(5);
    CPPUNIT_ASSERT_EQUAL(Status::Invalid, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)5, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.4, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(14.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(0, node->getTags().size());
    CPPUNIT_ASSERT_EQUAL(Status::Invalid, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(14.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    //ways

    CPPUNIT_ASSERT_EQUAL(3, (int)map->getWays().size());

    shared_ptr<Way> way = map->getWay(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(1));
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    tagValue = way->getTags().get("noteb");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("w1b").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getVersion());
    CPPUNIT_ASSERT(way->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    way = map->getWay(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, way->getId());
    CPPUNIT_ASSERT_EQUAL(16.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT(way->hasNode(3));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    tagValue = way->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("w2").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(16.0, way->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getVersion());
    CPPUNIT_ASSERT(way->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    way = map->getWay(3);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, way->getId());
    CPPUNIT_ASSERT_EQUAL(17.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(0, way->getTags().size());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(17.0, way->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getVersion());
    CPPUNIT_ASSERT(way->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    //relations

    CPPUNIT_ASSERT_EQUAL(2, (int)map->getRelationMap().size());

    shared_ptr<Relation> relation = map->getRelation(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    CPPUNIT_ASSERT_EQUAL(18.1, relation->getCircularError());
    CPPUNIT_ASSERT_EQUAL(QString("collection").toStdString(), relation->getType().toStdString());
    vector<RelationData::Entry> relationMembers = relation->getMembers();
    CPPUNIT_ASSERT_EQUAL(size_t(2), relationMembers.size());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(1)));
    CPPUNIT_ASSERT(relation->contains(ElementId::way(1)));
    RelationData::Entry member = relationMembers.at(0);
    CPPUNIT_ASSERT_EQUAL(QString("n1").toStdString(), member.role.toStdString());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    member = relationMembers.at(1);
    CPPUNIT_ASSERT_EQUAL(QString("w1").toStdString(), member.role.toStdString());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(1, relation->getTags().size());
    tagValue = relation->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("r1").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(18.1, relation->getCircularError());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getVersion());
    CPPUNIT_ASSERT(relation->getTimestamp() != ElementData::TIMESTAMP_EMPTY);

    relation = map->getRelation(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, relation->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
    CPPUNIT_ASSERT_EQUAL(QString("").toStdString(), relation->getType().toStdString());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(2)));
    CPPUNIT_ASSERT_EQUAL(size_t(1), relation->getMembers().size());
    member = relation->getMembers().at(0);
    CPPUNIT_ASSERT_EQUAL(QString("n2").toStdString(), member.role.toStdString());
    CPPUNIT_ASSERT_EQUAL((long)2, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(0, relation->getTags().size());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getVersion());
    CPPUNIT_ASSERT(relation->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
  }

  void verifySingleReadOutput(shared_ptr<OsmMap> map)
  {
    //nodes

    CPPUNIT_ASSERT_EQUAL(5, (int)map->getNodeMap().size());

    shared_ptr<Node> node = map->getNode(3);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.2, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    QString tagValue = node->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("n3").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
  }

  void runReadTest()
  {
    HootApiDbReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId).toString());
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();
  }

  void runReadWithElemTest()
  {
    HootApiDbReader reader;
    shared_ptr<OsmMap> map(new OsmMap());
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId,3,"node").toString());
    reader.read(map);
    verifySingleReadOutput(map);
    reader.close();
  }

  void runFactoryReadTest()
  {
    shared_ptr<OsmMap> map(new OsmMap());
    OsmMapReaderFactory::read(map, ServicesDbTestUtils::getDbReadUrl(mapId).toString());
    verifyFullReadOutput(map);
  }

  void runPartialReadTest()
  {
    HootApiDbReader reader;
    const int chunkSize = 3;
    reader.setMaxElementsPerMap(chunkSize);
    reader.open(ServicesDbTestUtils::getDbReadUrl(mapId).toString());
    reader.initializePartial();

    int ctr = 0;
    shared_ptr<OsmMap> map(new OsmMap());

    //3 nodes

    QString tagValue;
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);

    CPPUNIT_ASSERT_EQUAL(
      chunkSize,
      (int)(map->getNodeMap().size() + map->getWays().size() + map->getRelationMap().size()));
    CPPUNIT_ASSERT_EQUAL(3, (int)map->getNodeMap().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelationMap().size());

    shared_ptr<Node> node = map->getNode(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(10.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(0, node->getTags().size());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(10.0, node->getCircularError());

    node = map->getNode(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.1, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(11.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    CPPUNIT_ASSERT_EQUAL(QString("n2b").toStdString(), node->getTags().get("noteb").toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(11.0, node->getCircularError());

    node = map->getNode(3);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.2, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    tagValue = node->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("n3").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());

    ctr++;

    //2 nodes, 1 way

    map.reset(new OsmMap());
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(
      chunkSize,
      (int)(map->getNodeMap().size() + map->getWays().size() + map->getRelationMap().size()));
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodeMap().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelationMap().size());

    node = map->getNode(4);
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)4, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.3, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(13.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    tagValue = node->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("n4").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(13.0, node->getCircularError());

    node = map->getNode(5);
    CPPUNIT_ASSERT_EQUAL(Status::Invalid, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)5, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.4, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(14.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(0, node->getTags().size());
    CPPUNIT_ASSERT_EQUAL(Status::Invalid, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(14.0, node->getCircularError());

    shared_ptr<Way> way = map->getWay(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(1));
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    tagValue = way->getTags().get("noteb");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("w1b").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());

    ctr++;

    //2 ways, 1 relation

    map.reset(new OsmMap());
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(
      chunkSize,
      (int)(map->getNodeMap().size() + map->getWays().size() + map->getRelationMap().size()));
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodeMap().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelationMap().size());

    way = map->getWay(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, way->getId());
    CPPUNIT_ASSERT_EQUAL(16.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT(way->hasNode(3));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    tagValue = way->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("w2").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(16.0, way->getCircularError());

    way = map->getWay(3);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, way->getId());
    CPPUNIT_ASSERT_EQUAL(17.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(0, way->getTags().size());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(17.0, way->getCircularError());

    shared_ptr<Relation> relation = map->getRelation(1);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    CPPUNIT_ASSERT_EQUAL(18.1, relation->getCircularError());
    CPPUNIT_ASSERT_EQUAL(QString("collection").toStdString(), relation->getType().toStdString());
    CPPUNIT_ASSERT_EQUAL(size_t(2), relation->getMembers().size());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(1)));
    CPPUNIT_ASSERT(relation->contains(ElementId::way(1)));
    RelationData::Entry member = relation->getMembers().at(0);
    CPPUNIT_ASSERT_EQUAL(QString("n1").toStdString(), member.role.toStdString());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    member = relation->getMembers().at(1);
    CPPUNIT_ASSERT_EQUAL(QString("w1").toStdString(), member.role.toStdString());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(1, relation->getTags().size());
    tagValue = relation->getTags().get("note");
    CPPUNIT_ASSERT(!tagValue.isEmpty());
    CPPUNIT_ASSERT_EQUAL(QString("r1").toStdString(), tagValue.toStdString());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL(18.1, relation->getCircularError());

    ctr++;

    //1 relation

    map.reset(new OsmMap());
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(
      1,
      (int)(map->getNodeMap().size() + map->getWays().size() + map->getRelationMap().size()));
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodeMap().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelationMap().size());

    relation = map->getRelation(2);
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, relation->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
    CPPUNIT_ASSERT_EQUAL(QString("").toStdString(), relation->getType().toStdString());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(2)));
    CPPUNIT_ASSERT_EQUAL(size_t(1), relation->getMembers().size());
    member = relation->getMembers().at(0);
    CPPUNIT_ASSERT_EQUAL(QString("n2").toStdString(), member.role.toStdString());
    CPPUNIT_ASSERT_EQUAL((long)2, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(0, relation->getTags().size());
    CPPUNIT_ASSERT_EQUAL(Status::Unknown1, relation->getStatus().getEnum());

    ctr++;

    CPPUNIT_ASSERT(!reader.hasMoreElements());
    reader.finalizePartial();

    CPPUNIT_ASSERT_EQUAL(4, ctr);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HootApiDbReaderTest, "slow");

}
