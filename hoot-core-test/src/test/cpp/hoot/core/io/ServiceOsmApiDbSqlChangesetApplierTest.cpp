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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/io/OsmApiDbSqlChangesetApplier.h>
#include <hoot/core/io/ServicesDbTestUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/geometry/GeometryUtils.h>

// Qt
#include <QDateTime>

using namespace geos::geom;

namespace hoot
{

class ServiceOsmApiDbSqlChangesetApplierTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ServiceOsmApiDbSqlChangesetApplierTest);
  CPPUNIT_TEST(runSqlChangesetWriteTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "ServiceOsmApiDbSqlChangesetApplierTest@hoottestcpp.org"; }

  const QString _scriptDir = "test-files/servicesdb/";

  long mapId;

  void setUp() override
  {
    HootTestFixture::setUp();
    deleteUser(userEmail());
  }

  void tearDown() override
  {
    deleteUser(userEmail());

    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    database.close();
    HootTestFixture::tearDown();
  }

  void deleteUser(QString email)
  {
    OsmApiDb db;
    db.open(ServicesDbTestUtils::getOsmApiDbUrl());

    long userId = db.getUserId(email, false);
    if (userId != -1)
    {
      db.transaction();
      db.deleteUser(userId);
      db.commit();
    }
  }

  void insertChangeset(const QDateTime& createdAt, const QString& boundsStr)
  {
    OsmApiDb db;
    db.open(ServicesDbTestUtils::getOsmApiDbUrl());

    const Envelope bounds = GeometryUtils::envelopeFromString(boundsStr);
    LOG_VARD(boundsStr);
    LOG_VARD(createdAt.toString(OsmApiDb::TIME_FORMAT));
    const QString sql =
      QString("INSERT INTO %1 (id, user_id, created_at, closed_at, min_lat, max_lat, min_lon, max_lon) ")
        .arg(ApiDb::getChangesetsTableName()) +
      QString("VALUES (1, 1, '%1', '%2', %3, %4, %5, %6)")
        .arg(createdAt.toString(OsmApiDb::TIME_FORMAT))
        .arg(createdAt.toString(OsmApiDb::TIME_FORMAT))
        .arg(OsmApiDb::toOsmApiDbCoord(bounds.getMinY()))
        .arg(OsmApiDb::toOsmApiDbCoord(bounds.getMaxY()))
        .arg(OsmApiDb::toOsmApiDbCoord(bounds.getMinX()))
        .arg(OsmApiDb::toOsmApiDbCoord(bounds.getMaxX()));
    QSqlQuery q(db.getDB());
    LOG_VARD(sql);
    if (q.exec(sql) == false)
    {
      throw HootException(QString("Error executing query: %1 (%2)").arg(q.lastError().text()).arg(sql));
    }
    LOG_VARD(q.numRowsAffected());
  }

  //This may get pushed up into OsmApiDb later.
  QDateTime now()
  {
    return QDateTime::currentDateTime().toUTC();
  }

  void runSqlChangesetWriteTest()
  {
    OsmApiDb database;
    database.open(ServicesDbTestUtils::getOsmApiDbUrl());
    database.deleteData();
    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "users.sql");
    ApiDb::execSqlFile(
      ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "changesets.sql");

    std::shared_ptr<QSqlQuery> nodesItr = database.selectElements(ElementType::Node);
    assert(nodesItr->isActive());
    CPPUNIT_ASSERT_EQUAL(0, nodesItr->size());

    ApiDb::execSqlFile(ServicesDbTestUtils::getOsmApiDbUrl().toString(), _scriptDir + "nodes.sql");

    nodesItr = database.selectElements(ElementType::Node);
    assert(nodesItr->isActive());
    int nodesCountBefore = 0;
    long existingChangesetId = -1;
    while (nodesItr->next())
    {
      existingChangesetId = nodesItr->value(3).toLongLong();
      LOG_VARD(existingChangesetId)
      nodesCountBefore++;
    }
    nodesCountBefore--;
    LOG_VARD(nodesCountBefore);

    const long nextNodeId = database.getNextId(ApiDb::getCurrentNodesTableName());
    LOG_VARD(nextNodeId);

    const long nextChangesetId = existingChangesetId + 1;
    QString sql =
      QString("INSERT INTO %1 (id, user_id, created_at, closed_at) VALUES (%2, 1, now(), now());\n")
        .arg(ApiDb::getChangesetsTableName())
        .arg(nextChangesetId);
    sql +=
      QString("INSERT INTO %1 (id, latitude, longitude, changeset_id, visible, \"timestamp\", tile,  version) VALUES (%2, 0, 0, %3, true, now(), 3221225472, 1);")
        .arg(ApiDb::getCurrentNodesTableName())
        .arg(nextNodeId)
        .arg(nextChangesetId);

    OsmApiDbSqlChangesetApplier(ServicesDbTestUtils::getOsmApiDbUrl()).write(sql);

    nodesItr = database.selectElements(ElementType::Node);
    assert(nodesItr->isActive());
    int nodesCountAfter = 0;
    while (nodesItr->next())
    {
      nodesCountAfter++;
    }
    nodesCountAfter--;
    LOG_VARD(nodesCountAfter);

    CPPUNIT_ASSERT(nodesCountAfter == (nodesCountBefore + 1));
  }
};

#ifdef HOOT_HAVE_SERVICES
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbSqlChangesetApplierTest, "slow");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServiceOsmApiDbSqlChangesetApplierTest, "serial");
#endif  // HOOT_HAVE_SERVICES

}
