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
 * @copyright Copyright (C) 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SERVICESDBTESTUTILS_H
#define SERVICESDBTESTUTILS_H

// Qt
#include <QHash>
#include <QUrl>
#include <QVariant>

// Hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/HootNetworkCookieJar.h>

#ifdef HOOT_HAVE_SERVICES

namespace hoot
{

/**
 * @brief The ServicesDbTestUtils class contains general utilities that can be shared between the
 * services database related unit tests
 */
class ServicesDbTestUtils
{

public:

  /**
   * @brief compareRecords Runs a specified query and compare the results against the expected
   * results.
   */
  static void compareRecords(QString sql, QString expected, QString testName = nullptr,
                             QVariant v1 = QVariant(), QVariant v2 = QVariant());

  /**
   * @brief getDbModifyUrl retrieves a URL used to modify services database data.
   */
  static QUrl getDbModifyUrl(const QString& mapName = "testMap");

  /**
   * @brief getDbReadUrl retrieves a URL used to read services database data.
   */
  static QUrl getDbReadUrl(const long mapId);

  static QUrl getDbReadUrl(const long mapId, const long elemId, const QString& elemType);

  /**
   * @brief getOsmApiDbUrl retrieves URL for OSM API database.
   * @return URL for OSM API database
   */
  static QUrl getOsmApiDbUrl();

  /**
   * @brief deleteUser deletes the user with the specified email from the services database.
   */
  static void deleteUser(QString email);

  /**
   * @brief findIndex Finds a match in the test key list and returns the index.
   */
  static int findIndex(const QList<QString>& keys, const QString& key);

  static void deleteDataFromOsmApiTestDatabase();

  static void verifyTestDatabaseEmpty();

  static std::shared_ptr<OsmMap> createTestMap1();

  static OsmMapPtr createServiceTestMap();

  static long insertTestUser(const QString& userName, const QString& email, const QString& sessionId,
                             const QString& accessToken, const QString& accessTokenSecret, bool isAdmin = false);

  static bool deleteUserByUserName(const QString& userName);

  static std::shared_ptr<HootNetworkCookieJar> getTestSessionCookie(const QString& sessionId,
                                                                    const QString& url);

private:

  static std::shared_ptr<Node> _createNode(double x, double y, OsmMapPtr map);
};

}

#endif // HOOT_HAVE_SERVICES

#endif // SERVICESDBTESTUTILS_H
