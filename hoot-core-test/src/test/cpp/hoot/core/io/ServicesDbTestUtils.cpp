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
  assert(mapId > 0);
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
  assert(mapId > 0);
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
  result.setHost(s.get("DB_HOST").toString());
  result.setPort(s.get("DB_PORT").toInt());
  result.setUserName(s.get("DB_USER").toString());
  result.setPassword(s.get("DB_PASSWORD").toString());
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
  QFile fp(ConfPath::getHootHome() + "/conf/DatabaseConfig.sh");
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
      result.set(key, value);
    }
  }

  return result;
}

}

