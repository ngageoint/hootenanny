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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ServicesDbTestUtils.h"

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/OsmApiDbReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/StreamUtils.h>

#ifdef HOOT_HAVE_SERVICES

using namespace std;
using namespace Tgs;

namespace hoot
{

void ServicesDbTestUtils::compareRecords(QString sql, QString expected, QString testName, QVariant v1, QVariant v2)
{
  HootApiDb db;
  QUrl url = (testName == NULL) ? ServicesDbTestUtils::getDbModifyUrl() : ServicesDbTestUtils::getDbModifyUrl(testName);
  db.open(url);
  QString result = db.execToString(sql, v1, v2);
  if (expected == "")
  {
    QStringList rows = result.split("\n");
    for (int i = 0; i < rows.size(); ++i)
    {
      cout << "\"" << rows[i];
      if (i == rows.size() - 1)
      {
        cout << "\"" << endl;
      }
      else
      {
        cout << "\\n\"" << endl;
      }
    }
  }
  else
  {
    if (expected != result)
    {
      LOG_INFO(TestUtils::toQuotedString(result));
    }
    CPPUNIT_ASSERT_EQUAL(expected.toStdString(), result.toStdString());
  }
}

void ServicesDbTestUtils::deleteDataFromOsmApiTestDatabase()
{
  OsmApiDb database;
  database.open(getOsmApiDbUrl().toString());
  database.deleteData();
  database.close();
}

QUrl ServicesDbTestUtils::getDbModifyUrl(const QString& mapName)
{
  return QUrl(HootApiDb::getBaseUrl().toString() + "/" + mapName);
}

QUrl ServicesDbTestUtils::getDbReadUrl(const long mapId)
{
  //insert url example: hootapidb://hoot:hoottest@localhost:5432/hoot/testMap
  QString dbModifyUrl = getDbModifyUrl().toString();
  QStringList modifyUrlParts = dbModifyUrl.split("/");
  //read url example: hootapidb://hoot:hoottest@localhost:5432/hoot/1
  QString dbReadUrl =
    dbModifyUrl.remove("/" + modifyUrlParts[modifyUrlParts.size() - 1]) + "/" +
    QString::number(mapId);

  QUrl url(dbReadUrl);
  return url;
}

QUrl ServicesDbTestUtils::getDbReadUrl(const long mapId, const long elemId, const QString& elemType)
{
  //insert url example: hootapidb://hoot:hoottest@localhost:5432/hoot/testMap
  QString dbModifyUrl = getDbModifyUrl().toString();
  QStringList modifyUrlParts = dbModifyUrl.split("/");
  //read url example: hootapidb://hoot:hoottest@localhost:5432/hoot/1
  QString dbReadUrl =
    dbModifyUrl.remove("/" + modifyUrlParts[modifyUrlParts.size() - 1]) + "/" +
      QString::number(mapId) + "?osm-element-id=" + QString::number((elemId)) +
      "&osm-element-type=" + elemType;

  QUrl url(dbReadUrl);
  return url;
}

QUrl ServicesDbTestUtils::getOsmApiDbUrl()
{
  // read the DB values from the DB config file.
  Settings s = ApiDb::readDbConfig();
  QUrl result;
  result.setScheme(MetadataTags::OsmApiDbScheme());
  result.setHost(s.get("DB_HOST_OSMAPI").toString());
  result.setPort(s.get("DB_PORT_OSMAPI").toInt());
  result.setUserName(s.get("DB_USER_OSMAPI").toString());
  result.setPassword(s.get("DB_PASSWORD_OSMAPI").toString());
  result.setPath("/" + s.get("DB_NAME_OSMAPI").toString());
  return result;
}

void ServicesDbTestUtils::deleteUser(QString email)
{
  HootApiDb database;
  database.open(getDbModifyUrl());

  long userId = database.getUserId(email, false);
  if (userId != -1)
  {
    database.deleteUser(userId);
  }
  database.close();
}

int ServicesDbTestUtils::findIndex(const QList<QString>& keys, const QString& key)
{
  for (int i = 0; i < keys.size(); i++)
  {
    if (keys[i] == key) return i;
  }

  // didn't find a match so return -1
  return -1;
}

void ServicesDbTestUtils::verifyTestDatabaseEmpty()
{
  OsmApiDbReader reader;
  OsmMapPtr map(new OsmMap());
  reader.open(ServicesDbTestUtils::getOsmApiDbUrl().toString());
  reader.read(map);

  //verify current elements
  CPPUNIT_ASSERT_EQUAL((size_t)0, map->getNodes().size());
  CPPUNIT_ASSERT_EQUAL((size_t)0, map->getWays().size());
  CPPUNIT_ASSERT_EQUAL((size_t)0, map->getRelations().size());

  //verify historical element table sizes
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodesTableName()));
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getNodeTagsTableName()));
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWaysTableName()));
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayTagsTableName()));
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getWayNodesTableName()));
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationsTableName()));
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationTagsTableName()));
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getRelationMembersTableName()));

  //verify changeset table size
  CPPUNIT_ASSERT_EQUAL(
    (long)0,
    DbUtils::getRowCount(reader._getDatabase()->getDB(), ApiDb::getChangesetsTableName()));

  //verify sequences
  std::shared_ptr<OsmApiDb> osmApiDb = std::dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
  CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Node));
  CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Way));
  CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Relation));
  CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

  reader.close();
}

std::shared_ptr<Node> ServicesDbTestUtils::_createNode(double x, double y, OsmMapPtr map)
{
  std::shared_ptr<Node> n(new Node(Status::Unknown1, map->createNextNodeId(), x, y, 10.0));
  map->addNode(n);
  return n;
}

std::shared_ptr<OsmMap> ServicesDbTestUtils::createTestMap1()
{
  std::shared_ptr<OsmMap> map(new OsmMap());

  std::shared_ptr<Node> n1 = _createNode(-77.0, 38.0, map);
  n1->setTag("building", "yes");
  //put a space in this tag value, since hstore dies on those if they are not esacaped properly
  n1->setTag("name", "n1 - n2");

  std::shared_ptr<Way> w1(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w1->setTag("area", "yes");
  w1->setTag("building", "yes");
  w1->setTag("name", "w1");
  w1->addNode(_createNode(-77.1, 38.0, map)->getId());
  w1->addNode(_createNode(-77.2, 38.0, map)->getId());
  w1->addNode(_createNode(-77.2, 38.1, map)->getId());
  w1->addNode(w1->getNodeId(0));
  map->addWay(w1);

  std::shared_ptr<Way> w2(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w2->setTag("highway", "track");
  w2->setTag("name", "w2");
  w2->addNode(_createNode(-77.3, 38.0, map)->getId());
  w2->addNode(_createNode(-77.3, 38.1, map)->getId());
  map->addWay(w2);

  std::shared_ptr<Way> w3(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w3->setTag("highway", "road");
  w3->setTag("name", "w3");
  w3->addNode(_createNode(-77.4, 38.0, map)->getId());
  w3->addNode(_createNode(-77.4, 38.1, map)->getId());
  map->addWay(w3);

  std::shared_ptr<Way> w4(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w4->addNode(_createNode(-77.5, 38.0, map)->getId());
  w4->addNode(_createNode(-77.7, 38.0, map)->getId());
  w4->addNode(_createNode(-77.6, 38.1, map)->getId());
  w4->addNode(w4->getNodeId(0));
  map->addWay(w4);

  std::shared_ptr<Way> w5(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w5->addNode(_createNode(-77.55, 38.01, map)->getId());
  w5->addNode(_createNode(-77.65, 38.01, map)->getId());
  w5->addNode(_createNode(-77.6, 38.05, map)->getId());
  w5->addNode(w5->getNodeId(0));
  map->addWay(w5);

  std::shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15.0, "multipolygon"));
  r1->setTag("building", "yes");
  r1->setTag("name", "r1");
  r1->addElement("outer", w4->getElementId());
  r1->addElement("inner", w5->getElementId());
  map->addRelation(r1);

  return map;
}

OsmMapPtr ServicesDbTestUtils::createServiceTestMap()
{
  OsmMapPtr map(new OsmMap());

  NodePtr n1(new Node(Status::Unknown1, 1, 0.0, 0.0, 10.0));
  map->addNode(n1);
  NodePtr n2(new Node(Status::Unknown2, 2, 0.1, 0.0, 11.0));
  n2->setTag("noteb", "n2b");
  map->addNode(n2);
  NodePtr n3(new Node(Status::Conflated, 3, 0.2, 0.0, 12.0));
  n3->setTag("note", "n3");
  map->addNode(n3);
  NodePtr n4(new Node(Status::Conflated, 4, 0.3, 0.0, 13.0));
  n4->setTag("note", "n4");
  map->addNode(n4);
  NodePtr n5(new Node(Status::Invalid, 5, 0.4, 0.0, 14.0));
  map->addNode(n5);

  WayPtr w1(new Way(Status::Unknown1, 1, 15.0));
  w1->addNode(1);
  w1->addNode(2);
  w1->setTag("noteb", "w1b");
  map->addWay(w1);
  WayPtr w2(new Way(Status::Unknown2, 2, 16.0));
  w2->addNode(2);
  w2->addNode(3);
  w2->setTag("note", "w2");
  map->addWay(w2);
  WayPtr w3(new Way(Status::Unknown2, 3, 17.0));
  w3->addNode(2);
  map->addWay(w3);

  RelationPtr r1(new Relation(Status::Unknown1, 1, 18.1, MetadataTags::RelationCollection()));
  r1->addElement("n1", n1->getElementId());
  r1->addElement("w1", w1->getElementId());
  r1->setTag("note", "r1");
  map->addRelation(r1);
  RelationPtr r2(new Relation(Status::Unknown1, 2, -1.0));
  r2->addElement("n2", n2->getElementId());
  map->addRelation(r2);

  return map;
}

long ServicesDbTestUtils::insertTestUser(const QString& userName, const QString& email,
                                         const QString& sessionId, const QString& accessToken,
                                         const QString& accessTokenSecret)
{
  HootApiDb db;
  db.open(QUrl(HootApiDb::getBaseUrl().toString() + "/blah"));
  const long userId = db.insertUser(email, userName);
  db.insertUserSession(userId, sessionId);
  db.updateUserAccessTokens(userId, accessToken, accessTokenSecret);
  db.close();
  return userId;
}

bool ServicesDbTestUtils::deleteUserByUserName(const QString& userName)
{
  HootApiDb db;
  db.open(getDbModifyUrl());
  const long userId = db.getUserIdByName(userName);
  if (userId == -1)
  {
    return false;
  }
  db.deleteUser(userId);
  db.close();
  return true;
}

std::shared_ptr<HootNetworkCookieJar> ServicesDbTestUtils::getTestSessionCookie(const QString& sessionId, const QString& url)
{
  std::shared_ptr<HootNetworkCookieJar> cookieJar(new HootNetworkCookieJar());
  QList<QNetworkCookie> cookies;
  QNetworkCookie sessionCookie(QString("SESSION").toUtf8(), sessionId.toUtf8());
  cookies.append(sessionCookie);
  cookieJar->setCookiesFromUrl(cookies, url);
  return cookieJar;
}

}

#endif  // HOOT_HAVE_SERVICES
