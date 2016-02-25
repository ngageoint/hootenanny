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
#include <hoot/core/io/ServicesDb.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
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
  ServicesDb db;
  db.open(ServicesDbTestUtils::getDbModifyUrl());
  QString result = db._execToString(sql, v1, v2);
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
  // don't use the default settings b/c they've been cleared for unit testing.
  Settings s;
  s.loadDefaults();
  return QUrl(ConfigOptions(s).getServicesDbTestUrl());
}

QUrl ServicesDbTestUtils::getDbReadUrl(const long mapId)
{
  //insert url example: postgresql://hoot:hoottest@localhost:5432/hoot/testMap
  QString dbModifyUrl = getDbModifyUrl().toString();
  QStringList modifyUrlParts = dbModifyUrl.split("/");
  //read url example: postgresql://hoot:hoottest@localhost:5432/hoot/1
  assert(mapId > 0);
  QString dbReadUrl =
    dbModifyUrl.remove("/" + modifyUrlParts[modifyUrlParts.size() - 1]) + "/" +
    QString::number(mapId);

  QUrl url(dbReadUrl);
  return url;
}

QUrl ServicesDbTestUtils::getOsmApiDbUrl()
{
  Settings s = conf();
  return QUrl(ConfigOptions(s).getServicesDbTestUrlOsmapi());
}

void ServicesDbTestUtils::deleteUser(QString email)
{
  ServicesDb database;
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

}

