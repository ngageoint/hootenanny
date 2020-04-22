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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DateTimeUtils.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDateTime>
#include <QRegExp>

namespace hoot
{

QString DateTimeUtils::toTimeString(quint64 timestamp)
{
  // convert time in seconds since epoch into timestamp string
  QDateTime dt;
  dt.setTimeSpec(Qt::UTC);
  dt.setMSecsSinceEpoch(timestamp*1000);
  return dt.toString("yyyy-MM-ddThh:mm:ssZ");
}

quint64 DateTimeUtils::fromTimeString(QString timestamp)
{
  //2016-05-04T22:07:19Z
  QRegExp timestampRegex("\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}Z*");
  if (!timestampRegex.exactMatch(timestamp))
  {
    throw IllegalArgumentException("Invalid timestamp string: " + timestamp);
  }

  struct tm t;
  strptime(timestamp.toStdString().c_str(), "%Y-%m-%dT%H:%M:%SZ", &t);

  // calc time in seconds since epoch
  return (quint64)(t.tm_sec + t.tm_min*60 + t.tm_hour*3600 + t.tm_yday*86400 +
    (t.tm_year-70)*31536000 + ((t.tm_year-69)/4)*86400 -
    ((t.tm_year-1)/100)*86400 + ((t.tm_year+299)/400)*86400);
}

QString DateTimeUtils::currentTimeAsString()
{
  return QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ssZ");
}

}
