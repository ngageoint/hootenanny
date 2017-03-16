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
#ifndef SERVICESDBTESTUTILS_H
#define SERVICESDBTESTUTILS_H

// Qt
#include <QHash>
#include <QUrl>
#include <QVariant>

// Hoot
#include <hoot/core/util/Settings.h>

namespace hoot
{

/**
General utilities that can be shared between the services database related unit tests
*/
class ServicesDbTestUtils
{

public:

  /**
   * Run a specified query and compare the results against the expected results.
   */
  static void compareRecords(QString sql, QString expected, QVariant v1 = QVariant(),
    QVariant v2 = QVariant());

  /**
   * Retrieves a URL used to modify services database data
   */
  static QUrl getDbModifyUrl();

  /**
   * Retrieves a URL used to read services database data
   */
  static QUrl getDbReadUrl(const long mapId);

  static QUrl getDbReadUrl(const long mapId, const long elemId, const QString& elemType);

  /**
   * Retrieves URL for OSM API database
   *
   * @return URL for OSM API database
   */
  static QUrl getOsmApiDbUrl();

  /**
   * Delete the user with the specified email from the services database
   */
  static void deleteUser(QString email);

  /**
   * Find a match in the test key list and return the index
   */
  static int findIndex(const QList<QString>& keys, const QString& key);

  static void execOsmApiDbSqlTestScript(const QString scriptName);

  static void deleteDataFromOsmApiTestDatabase();

private:

  static Settings _readDbConfig();
  static void _readDbConfig(Settings& settings, QString config_path);
};

}

#endif // SERVICESDBTESTUTILS_H
