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
 * @copyright Copyright (C) 2013, 2014, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/ElementAttributeType.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/io/HootApiDbWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/visitors/RemoveAttributesVisitor.h>

using namespace std;

namespace hoot
{

class ServiceHootApiDbReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceHootApiDbReaderTest);
  CPPUNIT_TEST(runCalculateBoundsTest);
  CPPUNIT_TEST(runElementIdTest);
  CPPUNIT_TEST(runUrlInvalidMapIdTest);
  CPPUNIT_TEST(runReadTest);
  CPPUNIT_TEST(runPartialReadTest);
  CPPUNIT_TEST(runFactoryReadTest);
  CPPUNIT_TEST(runReadWithElemTest);
  CPPUNIT_TEST(runReadByBoundsTest);
  CPPUNIT_TEST(runAccessPublicMapWithoutEmailTest);
  CPPUNIT_TEST(runAccessPrivateMapWithoutEmailTest);
  CPPUNIT_TEST(runInvalidUserTest);
  CPPUNIT_TEST(runMultipleMapsSameNameDifferentUsersPrivateTest);
  CPPUNIT_TEST(runMultipleMapsSameNameDifferentUsersPublicTest);
  ////CPPUNIT_TEST(runMultipleMapsSameNameNoUserPublicTest);
  CPPUNIT_TEST(readByBoundsLeaveConnectedOobWaysTest);
  CPPUNIT_TEST(runAccessPrivateMapWithAdminUserTest);
  CPPUNIT_TEST_SUITE_END();

public:

  QString userEmail(const QString& test_name)
  { return QString("%1.ServiceHootApiDbReaderTest@hoottestcpp.org").arg(test_name); }

  ServiceHootApiDbReaderTest()
    : HootTestFixture("test-files/io/ServiceHootApiDbReaderTest/",
                      "test-output/io/ServiceHootApiDbReaderTest/")
  {
    setResetType(ResetEnvironment);
  }

  void setUpTest(const QString& testName, bool adminUser = false)
  {
    _mapId = -1;
    _testName = testName;
    ServicesDbTestUtils::deleteUser(userEmail(testName));
    HootApiDb database;

    database.open(ServicesDbTestUtils::getDbModifyUrl());
    database.getOrCreateUser(userEmail(testName), QString("%1.ServiceHootApiDbReaderTest").arg(testName), adminUser);
    database.close();
  }

  void tearDown() override
  {
    //  Delete the current map
    if (_mapId != -1)
    {
      HootApiDb database;
      database.open(ServicesDbTestUtils::getDbModifyUrl());

      database._deleteFolderMapMappingsByMapId(_mapId);
      database._deleteAllFolders(database.getFolderIdsAssociatedWithMap(_mapId));

      database.deleteMap(_mapId);

      database.close();
    }
    //  Delete the current user afterwards
    ServicesDbTestUtils::deleteUser(userEmail(_testName));
    HootTestFixture::tearDown();
  }

  long populateMap(const bool putInFolder = false, const bool folderIsPublic = false)
  {
    LOG_DEBUG("Populating test map...");

    OsmMapPtr map = ServicesDbTestUtils::createServiceTestMap();

    HootApiDbWriter writer;
    writer.setUserEmail(userEmail(_testName));
    writer.setRemap(false);
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(map);
    writer.close();

    if (putInFolder)
    {
      LOG_DEBUG("Adding test data folder...");
      HootApiDb db;
      db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
      db.insertFolderMapMapping(
        writer.getMapId(),
        db.insertFolder(_testName, -1, db.getUserId(userEmail(_testName), true), folderIsPublic));
      db.close();
    }

    return writer.getMapId();
  }

  long insertDataForBoundTest()
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map, _inputPath + "runReadByBoundsTestInput.osm", false, Status::Unknown1);

    HootApiDbWriter writer;
    writer.setUserEmail(userEmail(_testName));
    writer.setRemap(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
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
    setUpTest("runCalculateBoundsTest");
    _mapId = populateMap();

    HootApiDbReader reader;
    reader.setUserEmail(userEmail(_testName));
    QString url = ServicesDbTestUtils::getDbReadUrl(_mapId).toString();
    reader.open(url);
    HOOT_STR_EQUALS("Env[0:0.4,0:0]", reader.calculateEnvelope().toString());
  }

  void runElementIdTest()
  {
    setUpTest("runElementIdTest");
    _mapId = populateMap();

    HootApiDbReader reader;
    reader.setUserEmail(userEmail(_testName));
    // make sure all the element ids start with -1
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setUseDataSourceIds(false);
    reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId).toString());
    reader.read(map);

    HOOT_STR_EQUALS("[5]{-5, -4, -3, -2, -1}",
      getKeys(map->getNodes().begin(), map->getNodes().end()));
    HOOT_STR_EQUALS("[2]{-2, -1}",
      getKeys(map->getRelations().begin(), map->getRelations().end()));
    HOOT_STR_EQUALS("[3]{-3, -2, -1}", getKeys(map->getWays().begin(), map->getWays().end()));

    HOOT_STR_EQUALS("[1]{-2}", map->getWay(-3)->getNodeIds());
    HOOT_STR_EQUALS("[2]{-2, -3}", map->getWay(-2)->getNodeIds());
    HOOT_STR_EQUALS("[2]{-1, -2}", map->getWay(-1)->getNodeIds());
    HOOT_STR_EQUALS("[1]{Entry: role: n2, eid: Node(-2)}", map->getRelation(-2)->getMembers());
    HOOT_STR_EQUALS("[2]{Entry: role: n1, eid: Node(-1), Entry: role: w1, eid: Way(-1)}",
      map->getRelation(-1)->getMembers());
  }

  void runUrlInvalidMapIdTest()
  {
    setUpTest("runUrlInvalidMapIdTest");
    HootApiDbReader reader;
    reader.setUserEmail(userEmail(_testName));
    QString exceptionMsg("");
    const long invalidMapId = _mapId + 1;
    try
    {
      reader.open(
        ServicesDbTestUtils::getDbReadUrl(_mapId).toString().replace(
          "/" + QString::number(_mapId), "/" + QString::number(invalidMapId)));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("No map exists with requested ID: " +
      QString::number(invalidMapId)).toStdString(), exceptionMsg.toStdString());
  }

  void verifyFullReadOutput(OsmMapPtr map)
  {
    //nodes

    CPPUNIT_ASSERT_EQUAL(5, (int)map->getNodes().size());

    NodePtr node = map->getNode(1);
    HOOT_STR_EQUALS(Status::Unknown1, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(10.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getVersion());
    CPPUNIT_ASSERT(node->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("1", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(2);
    HOOT_STR_EQUALS(Status::Unknown2, node->getStatus().getEnum());
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
    HOOT_STR_EQUALS(Status::Conflated, node->getStatus().getEnum());
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
    HOOT_STR_EQUALS(Status::Conflated, node->getStatus().getEnum());
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
    HOOT_STR_EQUALS(Status::Invalid, node->getStatus().getEnum());
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

    WayPtr way = map->getWay(1);
    HOOT_STR_EQUALS(Status::Unknown1, way->getStatus().getEnum());
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
    HOOT_STR_EQUALS(Status::Unknown2, way->getStatus().getEnum());
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
    HOOT_STR_EQUALS(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, way->getId());
    CPPUNIT_ASSERT_EQUAL(17.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getVersion());
    CPPUNIT_ASSERT(way->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("3", way->getTags().get(MetadataTags::HootId()));

    //relations

    CPPUNIT_ASSERT_EQUAL(2, (int)map->getRelations().size());

    RelationPtr relation = map->getRelation(1);
    HOOT_STR_EQUALS(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    CPPUNIT_ASSERT_EQUAL(18.1, relation->getCircularError());
    HOOT_STR_EQUALS(MetadataTags::RelationCollection(), relation->getType());
    vector<RelationData::Entry> relationMembers = relation->getMembers();
    CPPUNIT_ASSERT_EQUAL(size_t(2), relationMembers.size());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(1)));
    CPPUNIT_ASSERT(relation->contains(ElementId::way(1)));
    RelationData::Entry member = relationMembers.at(0);
    HOOT_STR_EQUALS("n1", member.getRole());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    member = relationMembers.at(1);
    HOOT_STR_EQUALS("w1", member.getRole());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(2, relation->getTags().size());
    HOOT_STR_EQUALS("r1", relation->getTags().get("note"));
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getVersion());
    CPPUNIT_ASSERT(relation->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("1", relation->getTags().get(MetadataTags::HootId()));

    relation = map->getRelation(2);
    HOOT_STR_EQUALS(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, relation->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
    HOOT_STR_EQUALS("", relation->getType());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(2)));
    CPPUNIT_ASSERT_EQUAL(size_t(1), relation->getMembers().size());
    member = relation->getMembers().at(0);
    HOOT_STR_EQUALS("n2", member.getRole());
    CPPUNIT_ASSERT_EQUAL((long)2, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(1, relation->getTags().size());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getVersion());
    CPPUNIT_ASSERT(relation->getTimestamp() != ElementData::TIMESTAMP_EMPTY);
    HOOT_STR_EQUALS("2", relation->getTags().get(MetadataTags::HootId()));
  }

  void verifySingleReadOutput(OsmMapPtr map)
  {
    //nodes

    CPPUNIT_ASSERT_EQUAL(5, (int)map->getNodes().size());

    NodePtr node = map->getNode(3);
    HOOT_STR_EQUALS(Status::Conflated, node->getStatus().getEnum());
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
    setUpTest("runReadTest");
    _mapId = populateMap();

    HootApiDbReader reader;
    reader.setUserEmail(userEmail(_testName));
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId).toString());
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();
  }

  void runReadWithElemTest()
  {
    setUpTest("runReadWithElemTest");
    _mapId = populateMap();

    HootApiDbReader reader;
    reader.setUserEmail(userEmail(_testName));
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId, 3, "node").toString());
    reader.read(map);
    verifySingleReadOutput(map);
    reader.close();
  }

  void runFactoryReadTest()
  {
    setUpTest("runFactoryReadTest");
    _mapId = populateMap();

    OsmMapPtr map = std::make_shared<OsmMap>();
    conf().set(ConfigOptions::getApiDbEmailKey(), userEmail(_testName));
    OsmMapReaderFactory::read(map, ServicesDbTestUtils::getDbReadUrl(_mapId).toString());
    verifyFullReadOutput(map);
  }

  void runPartialReadTest()
  {
    setUpTest("runPartialReadTest");
    _mapId = populateMap();

    HootApiDbReader reader;
    reader.setUserEmail(userEmail(_testName));
    const int chunkSize = 3;
    reader.setMaxElementsPerMap(chunkSize);
    reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId).toString());
    reader.initializePartial();

    int ctr = 0;
    OsmMapPtr map = std::make_shared<OsmMap>();

    //3 nodes

    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);

    CPPUNIT_ASSERT_EQUAL(3, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    NodePtr node = map->getNode(1);
    HOOT_STR_EQUALS(Status::Unknown1, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(10.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    HOOT_STR_EQUALS("1", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(2);
    HOOT_STR_EQUALS(Status::Unknown2, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.1, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(11.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n2b", node->getTags().get("noteb"));
    HOOT_STR_EQUALS("2", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(3);
    HOOT_STR_EQUALS(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.2, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(12.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n3", node->getTags().get("note"));
    HOOT_STR_EQUALS("3", node->getTags().get(MetadataTags::HootId()));

    ctr++;

    //2 nodes, 1 way

    map = std::make_shared<OsmMap>();
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    node = map->getNode(4);
    HOOT_STR_EQUALS(Status::Conflated, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)4, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.3, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(13.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(2, node->getTags().size());
    HOOT_STR_EQUALS("n4", node->getTags().get("note"));
    HOOT_STR_EQUALS("4", node->getTags().get(MetadataTags::HootId()));

    node = map->getNode(5);
    HOOT_STR_EQUALS(Status::Invalid, node->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)5, node->getId());
    CPPUNIT_ASSERT_EQUAL(0.4, node->getX());
    CPPUNIT_ASSERT_EQUAL(0.0, node->getY());
    CPPUNIT_ASSERT_EQUAL(14.0, node->getCircularError());
    CPPUNIT_ASSERT_EQUAL(1, node->getTags().size());
    HOOT_STR_EQUALS("5", node->getTags().get(MetadataTags::HootId()));

    WayPtr way = map->getWay(1);
    HOOT_STR_EQUALS(Status::Unknown1, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, way->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(1));
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(2, way->getTags().size());
    HOOT_STR_EQUALS("w1b", way->getTags().get("noteb"));
    HOOT_STR_EQUALS("1", way->getTags().get(MetadataTags::HootId()));

    ctr++;

    //2 ways, 1 relation

    map = std::make_shared<OsmMap>();
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());

    way = map->getWay(2);
    HOOT_STR_EQUALS(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, way->getId());
    CPPUNIT_ASSERT_EQUAL(16.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT(way->hasNode(3));
    CPPUNIT_ASSERT_EQUAL(2, way->getTags().size());
    HOOT_STR_EQUALS("w2", way->getTags().get("note"));
    HOOT_STR_EQUALS("2", way->getTags().get(MetadataTags::HootId()));

    way = map->getWay(3);
    HOOT_STR_EQUALS(Status::Unknown2, way->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)3, way->getId());
    CPPUNIT_ASSERT_EQUAL(17.0, way->getCircularError());
    CPPUNIT_ASSERT(way->hasNode(2));
    CPPUNIT_ASSERT_EQUAL(1, way->getTags().size());
    HOOT_STR_EQUALS("3", way->getTags().get(MetadataTags::HootId()));

    RelationPtr relation = map->getRelation(1);
    HOOT_STR_EQUALS(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)1, relation->getId());
    CPPUNIT_ASSERT_EQUAL(18.1, relation->getCircularError());
    HOOT_STR_EQUALS(MetadataTags::RelationCollection(), relation->getType());
    CPPUNIT_ASSERT_EQUAL(size_t(2), relation->getMembers().size());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(1)));
    CPPUNIT_ASSERT(relation->contains(ElementId::way(1)));
    RelationData::Entry member = relation->getMembers().at(0);
    HOOT_STR_EQUALS("n1", member.getRole());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    member = relation->getMembers().at(1);
    HOOT_STR_EQUALS("w1", member.getRole());
    CPPUNIT_ASSERT_EQUAL((long)1, member.getElementId().getId());
    CPPUNIT_ASSERT_EQUAL(2, relation->getTags().size());
    HOOT_STR_EQUALS("r1", relation->getTags().get("note"));
    HOOT_STR_EQUALS("1", relation->getTags().get(MetadataTags::HootId()));

    ctr++;

    //1 relation

    map = std::make_shared<OsmMap>();
    CPPUNIT_ASSERT(reader.hasMoreElements());
    reader.readPartial(map);
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(1, (int)map->getRelations().size());

    relation = map->getRelation(2);
    HOOT_STR_EQUALS(Status::Unknown1, relation->getStatus().getEnum());
    CPPUNIT_ASSERT_EQUAL((long)2, relation->getId());
    CPPUNIT_ASSERT_EQUAL(15.0, relation->getCircularError());
    HOOT_STR_EQUALS("", relation->getType());
    CPPUNIT_ASSERT(relation->contains(ElementId::node(2)));
    CPPUNIT_ASSERT_EQUAL(size_t(1), relation->getMembers().size());
    member = relation->getMembers().at(0);
    HOOT_STR_EQUALS("n2", member.getRole());
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
    setUpTest("runReadByBoundsTest");
    _mapId = insertDataForBoundTest();

    HootApiDbReader reader;
    reader.setUserEmail(userEmail(_testName));
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId).toString());

    // See related note in ServiceOsmApiDbReaderTest::runReadByBoundsTest.

    reader.setBoundingBox("-88.1,28.89,-88.0,28.91");
    reader.read(map);

    //quick check to see if the element counts are off...consult the test output for more detail

    //See explanations for these assertions in ServiceOsmApiDbReaderTest::runReadByBoundsTest
    //(exact same input data)
    CPPUNIT_ASSERT_EQUAL(5, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(2, (int)map->getRelations().size());

    //We need to set all the element changeset tags here to empty, which will cause them
    //to be dropped from the file output.  If they aren't dropped, they will increment with each
    //test and cause the output comparison to fail.
    QList<ElementAttributeType> types;
    types.append(ElementAttributeType(ElementAttributeType::Changeset));
    types.append(ElementAttributeType(ElementAttributeType::Timestamp));
    RemoveAttributesVisitor attrVis(types);
    map->visitRw(attrVis);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "runReadByBoundsTestOutput.osm");
    HOOT_FILE_EQUALS( _inputPath + "runReadByBoundsTestOutput.osm",
                     _outputPath + "runReadByBoundsTestOutput.osm");

    //just want to make sure I can read against the same data twice in a row w/o crashing and also
    //make sure I don't get the same result again for a different bounds
    reader.setBoundingBox("-1,-1,1,1");
    map = std::make_shared<OsmMap>();
    reader.read(map);

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    reader.close();
  }

  void runAccessPublicMapWithoutEmailTest()
  {
    setUpTest("runAccessPublicMapWithoutEmailTest");
    _mapId = populateMap(true, true);

    // Even though we have no configured user, the map is public, so we should be able to read it.

    HootApiDbReader reader;
    reader.setUserEmail("");
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId).toString());
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();
  }

  void runAccessPrivateMapWithoutEmailTest()
  {
    setUpTest("runAccessPrivateMapWithoutEmailTest");
    // write a private map
    _mapId = populateMap(true, false);

    // configure no user with the reader
    HootApiDbReader reader;
    reader.setUserEmail("");

    // The reader should not be able to read the private map without a configured user.
    QString exceptionMsg("");
    try
    {
      reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId).toString());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
      reader.close();
    }
    LOG_VARD(exceptionMsg);
    CPPUNIT_ASSERT(exceptionMsg.contains("not available for public"));
  }

  void runInvalidUserTest()
  {
    setUpTest("runInvalidUserTest");
    // write a private map
    _mapId = populateMap(true, false);

    // create a second user
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const QString differentUserEmail = "blah@blah.com";
    const long differentUserId = db.insertUser(differentUserEmail, differentUserEmail);
    db.close();

    // set the reader up with the different user
    HootApiDbReader reader;
    reader.setUserEmail(differentUserEmail);

    // The second user should not be able to access the other user's private map.
    QString exceptionMsg("");
    try
    {
      reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId).toString());
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();

      reader.close();

      // delete the second user
      db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
      db.deleteUser(differentUserId);
      db.close();
    }
    LOG_VARD(exceptionMsg);
    CPPUNIT_ASSERT(exceptionMsg.contains("access to map with ID"));  
  }

  void runMultipleMapsSameNameDifferentUsersPrivateTest()
  {
    setUpTest("runMultipleMapsSameNameDifferentUsersPrivateTest");
    // create a private map
    LOG_DEBUG("Creating first map...");
    _mapId = populateMap();
    LOG_VARD(_mapId);

    // create a user different than the one who wrote the original map
    LOG_DEBUG("Creating second user...");
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const QString differentUserEmail = "runMultipleMapsSameNameDifferentUsersPrivateTest2";
    const long differentUserId = db.insertUser(differentUserEmail, differentUserEmail);
    LOG_VARD(differentUserId);
    db.close();

    // write a private map for the second user with the same name as the original map
    LOG_DEBUG("Writing second map...");
    HootApiDbWriter writer;
    writer.setUserEmail(differentUserEmail);
    writer.setRemap(false);
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(ServicesDbTestUtils::createServiceTestMap());
    writer.close();
    const long secondMapId = writer.getMapId();
    LOG_VARD(secondMapId);

    HootApiDbReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    QString url = ServicesDbTestUtils::getDbReadUrl(_mapId).toString();
    url = url.replace("/" + QString::number(_mapId), "/" + _testName);
    LOG_VARD(url);

    // Configure the reader for the original user, and we should be able to read out the map.
    // There are two maps with the same name, but they are owned by different users.
    LOG_DEBUG("Reading first map with first user...");
    reader.setUserEmail(userEmail(_testName));
    reader.open(url);
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();

    // Configure the reader for the second user, and we should be able to read out the map as well.
    LOG_DEBUG("Reading second map with second user...");
    map = std::make_shared<OsmMap>();
    reader.setUserEmail(differentUserEmail);
    reader.open(url);
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();

    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    // clean up the second map
    LOG_DEBUG("Deleting second map...");
    db.deleteMap(db.getMapIdByName(_testName));

    LOG_DEBUG("Deleting second user...");
    db.deleteUser(differentUserId);
    db.close();
  }

  void runMultipleMapsSameNameDifferentUsersPublicTest()
  {
    setUpTest("runMultipleMapsSameNameDifferentUsersPublicTest");
    // create a public map
    LOG_DEBUG("Creating original map...");
    _mapId = populateMap(true, true);
    LOG_VARD(_mapId);

    // create a user different than the one who wrote the original map
    LOG_DEBUG("Creating second user...");
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const QString differentUserEmail = "runMultipleMapsSameNameDifferentUsersPublicTest2";
    const long differentUserId = db.insertUser(differentUserEmail, differentUserEmail);
    LOG_VARD(differentUserId);
    db.close();

    // write a private map for the different user with the same name as the original map
    LOG_DEBUG("Writing second map...");
    HootApiDbWriter writer;
    writer.setUserEmail(differentUserEmail);
    writer.setRemap(false);
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(ServicesDbTestUtils::createServiceTestMap());
    writer.close();
    const long secondMapId = writer.getMapId();
    LOG_VARD(secondMapId);

    LOG_DEBUG("Reading first map with first user...");
    HootApiDbReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    QString url = ServicesDbTestUtils::getDbReadUrl(_mapId).toString();
    url = url.replace("/" + QString::number(_mapId), "/" + _testName);
    // Configure the reader for the original user, and we should be able to read out the second map.
    // There are two maps with the same name, but they are owned by different users.
    reader.setUserEmail(userEmail(_testName));
    reader.open(url);
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();

    // Configure the reader for the second user, and we should be able to read out the map as well.
    LOG_DEBUG("Reading second map with second user...");
    map = std::make_shared<OsmMap>();
    reader.setUserEmail(differentUserEmail);
    reader.open(url);
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();

    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    // clean up the second map
    db.deleteMap(db.getMapIdByName(_testName));

    db.deleteUser(differentUserId);
    db.close();
  }

  void runMultipleMapsSameNameNoUserPublicTest()
  {
    setUpTest("runMultipleMapsSameNameNoUserPublicTest");
    // create a public map
    LOG_DEBUG("Creating original map...");
    _mapId = populateMap(true, true);
    LOG_VARD(_mapId);
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    const long firstUserId = db.getUserId(userEmail(_testName), true);
    LOG_VARD(firstUserId);

    // create a user different than the one who wrote the original map
    LOG_DEBUG("Creating second user...");
    const QString differentUserEmail = "runMultipleMapsSameNameDifferentUsersTest2";
    const long differentUserId = db.insertUser(differentUserEmail, differentUserEmail);
    LOG_VARD(differentUserId);
    db.close();

    // write a public map for the different user with the same name as the original map
    LOG_DEBUG("Writing second map...");
    HootApiDbWriter writer;
    writer.setUserEmail(differentUserEmail);
    writer.setRemap(false);
    writer.setIncludeDebug(true);
    writer.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    writer.write(ServicesDbTestUtils::createServiceTestMap());
    writer.close();
    const long secondMapId = writer.getMapId();
    LOG_VARD(secondMapId);
    db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
    db.insertFolderMapMapping(
      secondMapId,
      db.insertFolder(_testName, -1, db.getUserId(differentUserEmail, true), true));
    db.close();

    // Configure the reader for no user
    HootApiDbReader reader;
    QString url = ServicesDbTestUtils::getDbReadUrl(_mapId).toString();
    url = url.replace("/" + QString::number(_mapId), "/" + _testName);
    reader.setUserEmail("");

    // Try to read the map out anonymously from its public folder.  This will cause an error, as
    // its ambiguous to hoot as to which map to return.  A map ID would need to be used in this
    // situation instead.
    QString exceptionMsg("");
    LOG_DEBUG("Reading map for no configured user...");
    try
    {
      reader.open(url);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();

      reader.close();

      // clean up the second map and user
      db.open(ServicesDbTestUtils::getDbModifyUrl(_testName).toString());
      db.deleteMap(db.getMapIdByName(_testName));
      db.deleteUser(differentUserId);
      db.close();
    }
    LOG_VARD(exceptionMsg);
    CPPUNIT_ASSERT(exceptionMsg.contains("already has map with name"));
  }

  void readByBoundsLeaveConnectedOobWaysTest()
  {
    // This will leave any ways in the output which are outside of the bounds but are directly
    // connected to ways which cross the bounds.

    setUpTest("readByBoundsLeaveConnectedOobWaysTest");
    _mapId = insertDataForBoundTest();

    HootApiDbReader reader;
    reader.setUserEmail(userEmail(_testName));
    reader.setKeepImmediatelyConnectedWaysOutsideBounds(true);
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.open(ServicesDbTestUtils::getDbReadUrl(_mapId).toString());

    // See related note in ServiceOsmApiDbReaderTest::runReadByBoundsTest.

    reader.setBoundingBox("-88.1,28.89,-88.0,28.91");
    reader.read(map);

    //quick check to see if the element counts are off...consult the test output for more detail

    //See explanations for these assertions in ServiceOsmApiDbReaderTest::runReadByBoundsTest
    //(exact same input data)
    //OsmMapWriterFactory::write(
      //map, _outputPath + "/readByBoundsLeaveConnectedOobWaysTest.osm", false, true);
    CPPUNIT_ASSERT_EQUAL(6, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(3, (int)map->getRelations().size());

    //We need to set all the element changeset tags here to empty, which will cause them
    //to be dropped from the file output.  If they aren't dropped, they will increment with each
    //test and cause the output comparison to fail.
    QList<ElementAttributeType> types;
    types.append(ElementAttributeType(ElementAttributeType::Changeset));
    types.append(ElementAttributeType(ElementAttributeType::Timestamp));
    RemoveAttributesVisitor attrVis(types);
    map->visitRw(attrVis);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(map, _outputPath + "readByBoundsLeaveConnectedOobWaysTestOutput.osm");
    HOOT_FILE_EQUALS(
      _inputPath + "readByBoundsLeaveConnectedOobWaysTestOutput.osm",
      _outputPath + "readByBoundsLeaveConnectedOobWaysTestOutput.osm");

    //just want to make sure I can read against the same data twice in a row w/o crashing and also
    //make sure I don't get the same result again for a different bounds
    reader.setBoundingBox("-1,-1,1,1");
    map = std::make_shared<OsmMap>();
    reader.read(map);

    CPPUNIT_ASSERT_EQUAL(0, (int)map->getNodes().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getWays().size());
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getRelations().size());

    reader.close();
  }

  void runAccessPrivateMapWithAdminUserTest()
  {
    //  First test has a private map
    QString testMap1 = "runAccessPrivateMapWithAdminUserTest1";
    setUpTest(testMap1, false);
    LOG_DEBUG("Creating original map...");
    LOG_VARD(_mapId);
    //  Don't make a folder for the map and keep the map private for this test
    _mapId = populateMap(false, false);

    // Create an admin user
    QString testMap2 = "runAccessPrivateMapWithAdminUserTest2";
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl(testMap2).toString());
    const long adminUser = db.getOrCreateUser(userEmail(testMap2), testMap2, true);
    LOG_VARD(adminUser);

    //  Attempt to read the original map with the admin user
    LOG_DEBUG("Reading map with second user...");
    HootApiDbReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    QString url = ServicesDbTestUtils::getDbReadUrl(_mapId).toString();
    reader.setUserEmail(userEmail(testMap2));
    reader.open(url);
    reader.read(map);
    verifyFullReadOutput(map);
    reader.close();

    db.open(ServicesDbTestUtils::getDbModifyUrl(testMap1).toString());
    db.deleteUser(adminUser);
    db.close();
  }

private:

  long _mapId;
  QString _testName;
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceHootApiDbReaderTest, "slow");
#endif  // HOOT_HAVE_SERVICES

}
