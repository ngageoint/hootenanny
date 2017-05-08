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

#include "ServicesDbTestUtils.h"

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

// Hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/io/OsmApiDbReader.h>
#include <hoot/core/util/DbUtils.h>

// Qt
#include <QFile>
#include <QStringList>

// Tgs
#include <tgs/StreamUtils.h>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

void ServicesDbTestUtils::compareRecords(QString sql, QString expected, QVariant v1,
  QVariant v2)
{
  HootApiDb db;
  db.open(ServicesDbTestUtils::getDbModifyUrl());
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

void ServicesDbTestUtils::execOsmApiDbSqlTestScript(const QString scriptName)
{
  //example: osmapidb://hoot:hoottest@localhost:5432/osmapi_test
  QString dbUrlString = getOsmApiDbUrl().toString();
  QStringList dbUrlParts = dbUrlString.split("/");
  QString dbName = dbUrlParts[dbUrlParts.size()-1];
  QStringList userParts = dbUrlParts[dbUrlParts.size()-2].split(":");
  QString dbUser = userParts[0];
  QString dbPassword = userParts[1].split("@")[0];
  QString dbHost = userParts[1].split("@")[1];
  QString dbPort = userParts[2];
  const QString auth = "-h "+dbHost+" -p "+dbPort+" -U "+dbUser;

  QString cmd =
    "export PGPASSWORD=" + dbPassword + "; export PGUSER=" + dbUser + "; export PGDATABASE=" +
    dbName + "; psql " + auth + " -v ON_ERROR_STOP=1 -f ${HOOT_HOME}/test-files/servicesdb/" +
    scriptName;
  if (Log::getInstance().getLevel() > Log::Debug)
  {
    cmd += " > /dev/null 2>&1";
  }
  LOG_VARD(cmd);
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Failed postgres command.  Exiting test.");
  }
}

QUrl ServicesDbTestUtils::getDbModifyUrl()
{
  // read the DB values from the DB config file.
  Settings s = _readDbConfig();
  QUrl result;
  result.setScheme("hootapidb");
  result.setHost(s.get("DB_HOST").toString());
  result.setPort(s.get("DB_PORT").toInt());
  result.setUserName(s.get("DB_USER").toString());
  result.setPassword(s.get("DB_PASSWORD").toString());
  result.setPath("/" + s.get("DB_NAME").toString() + "/testMap");
  return result;
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
  Settings s = _readDbConfig();
  QUrl result;
  result.setScheme("osmapidb");
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
}

int ServicesDbTestUtils::findIndex(const QList<QString>& keys, const QString& key)
{
  for(int i=0;i<keys.size();i++)
  {
    if(keys[i]==key) return i;
  }

  // didn't find a match so return -1
  return -1;
}

Settings ServicesDbTestUtils::_readDbConfig()
{
  Settings result;
  //  Read in the default values
  QString defaults = ConfPath::getHootHome() + "/conf/database/DatabaseConfigDefault.sh";
  _readDbConfig(result, defaults);
  //  Read in the local values if the file exists
  QString local = ConfPath::getHootHome() + "/conf/database/DatabaseConfigLocal.sh";
  if (QFile::exists(local))
  {
    _readDbConfig(result, local);
  }
  return result;
}

void ServicesDbTestUtils::_readDbConfig(Settings& settings, QString config_path)
{
  QFile fp(config_path);
  if (fp.open(QIODevice::ReadOnly) == false)
  {
    throw HootException("Error opening: " + fp.fileName());
  }
  QString s = QString::fromUtf8(fp.readAll());

  QStringList sl = s.split('\n', QString::SkipEmptyParts);

  foreach (QString s, sl)
  {
    QString key = s.section("=", 0, 0).remove("export ").trimmed();
    QString value = s.section("=", 1).trimmed();
    if (!key.startsWith("#") && key.length() > 0)
    {
      settings.set(key, value);
    }
  }
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
  boost::shared_ptr<OsmApiDb> osmApiDb = dynamic_pointer_cast<OsmApiDb>(reader._getDatabase());
  CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Node));
  CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Way));
  CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ElementType::Relation));
  CPPUNIT_ASSERT_EQUAL((long)1, osmApiDb->getNextId(ApiDb::getChangesetsTableName()));

  reader.close();
}

boost::shared_ptr<Node> ServicesDbTestUtils::_createNode(double x, double y, OsmMapPtr map)
{
  boost::shared_ptr<Node> n(new Node(Status::Unknown1, map->createNextNodeId(), x, y, 10.0));
  map->addNode(n);
  return n;
}

boost::shared_ptr<OsmMap> ServicesDbTestUtils::createTestMap1()
{
  boost::shared_ptr<OsmMap> map(new OsmMap());

  boost::shared_ptr<Node> n1 = _createNode(-77.0, 38.0, map);
  n1->setTag("building", "yes");
  n1->setTag("name", "n1");

  boost::shared_ptr<Way> w1(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w1->setTag("area", "yes");
  w1->setTag("building", "yes");
  w1->setTag("name", "w1");
  w1->addNode(_createNode(-77.1, 38.0, map)->getId());
  w1->addNode(_createNode(-77.2, 38.0, map)->getId());
  w1->addNode(_createNode(-77.2, 38.1, map)->getId());
  w1->addNode(w1->getNodeId(0));
  map->addWay(w1);

  boost::shared_ptr<Way> w2(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w2->setTag("highway", "track");
  w2->setTag("name", "w2");
  w2->addNode(_createNode(-77.3, 38.0, map)->getId());
  w2->addNode(_createNode(-77.3, 38.1, map)->getId());
  map->addWay(w2);

  boost::shared_ptr<Way> w3(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w3->setTag("highway", "road");
  w3->setTag("name", "w3");
  w3->addNode(_createNode(-77.4, 38.0, map)->getId());
  w3->addNode(_createNode(-77.4, 38.1, map)->getId());
  map->addWay(w3);

  boost::shared_ptr<Way> w4(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w4->addNode(_createNode(-77.5, 38.0, map)->getId());
  w4->addNode(_createNode(-77.7, 38.0, map)->getId());
  w4->addNode(_createNode(-77.6, 38.1, map)->getId());
  w4->addNode(w4->getNodeId(0));
  map->addWay(w4);

  boost::shared_ptr<Way> w5(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
  w5->addNode(_createNode(-77.55, 38.01, map)->getId());
  w5->addNode(_createNode(-77.65, 38.01, map)->getId());
  w5->addNode(_createNode(-77.6, 38.05, map)->getId());
  w5->addNode(w5->getNodeId(0));
  map->addWay(w5);

  boost::shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15.0, "multipolygon"));
  r1->setTag("building", "yes");
  r1->setTag("name", "r1");
  r1->addElement("outer", w4->getElementId());
  r1->addElement("inner", w5->getElementId());
  map->addRelation(r1);

  return map;
}

}

