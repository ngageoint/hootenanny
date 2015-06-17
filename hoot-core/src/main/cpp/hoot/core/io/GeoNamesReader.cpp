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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "GeoNamesReader.h"

// hoot
#include <hoot/core/MapReprojector.h>

// qt
#include <QFile>

namespace hoot
{

GeoNamesReader::GeoNamesReader()
{
  _circularError = 15.0;
}

void GeoNamesReader::read(QString path, shared_ptr<OsmMap> map)
{
  MapReprojector::reprojectToWgs84(map);

  QFile fp(path);

  if (fp.open(QFile::ReadOnly) == false)
  {
    throw HootException("Error opening file for reading: " + path);
  }

  QStringList l;
  l << "geonameid";
  l << "name";
  l << "asciiname";
  l << "alternatenames";
  l << "latitude";
  l << "longitude";
  l << "feature_class";
  l << "feature_code";
  l << "country_code";
  l << "cc2";
  l << "admin1_code";
  l << "admin2_code";
  l << "admin3_code";
  l << "admin4_code";
  l << "population";
  l << "elevation";
  l << "dem";
  l << "timezone";
  l << "modification_date";

  QList<int> convertColumns;
  convertColumns << 1 << 3 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 18;
  const int LATITUDE = 4;
  const int LONGITUDE = 5;

  while (!fp.atEnd())
  {
    QByteArray lineBytes = fp.readLine();
    QString line = QString::fromUtf8(lineBytes.constData());
    QStringList fields = line.split('\t');

    bool ok;
    double x = fields[LONGITUDE].toDouble(&ok);
    if (ok == false)
    {
      throw HootException(QString("Error parsing longitude (%1): %2").arg(fields[LONGITUDE]).
        arg(line));
    }
    double y = fields[LATITUDE].toDouble(&ok);
    if (ok == false)
    {
      throw HootException(QString("Error parsing latitude (%1): %2").arg(fields[LATITUDE]).
        arg(line));
    }

    shared_ptr<Node> n(new Node(_status, map->createNextNodeId(), x, y, _circularError));

    if (l.size() != fields.size())
    {
      throw HootException(QString("Expected %1 fields but found %2 fields. %3").arg(l.size()).
        arg(fields.size()).arg(line));
    }

    for (int i = 0; i < l.size(); i++)
    {
      int j = i; //convertColumns[i];
      n->getTags()[l[j]] = _saveMemory(fields[j]);
    }

    map->addNode(n);
  }
}

const QString& GeoNamesReader::_saveMemory(const QString& s)
{
  if (!_strings.contains(s))
  {
    _strings[s] = s;
  }
  return _strings[s];
}

}
