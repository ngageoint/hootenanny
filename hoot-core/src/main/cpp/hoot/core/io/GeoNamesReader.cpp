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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "GeoNamesReader.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>

// qt
#include <QDir>
#include <QFile>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, GeoNamesReader)

GeoNamesReader::GeoNamesReader()
{
  _circularError = 15.0;
  _useDataSourceIds = false;

  _columns << "geonameid";
  _columns << "name";
  _columns << "asciiname";
  _columns << "alternatenames";
  _columns << "latitude";
  _columns << "longitude";
  _columns << "feature_class";
  _columns << "feature_code";
  _columns << "country_code";
  _columns << "cc2";
  _columns << "admin1_code";
  _columns << "admin2_code";
  _columns << "admin3_code";
  _columns << "admin4_code";
  _columns << "population";
  _columns << "elevation";
  _columns << "dem";
  _columns << "timezone";
  _columns << "modification_date";

  _GEONAMESID = 0;
  _convertColumns << 1 << 3 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 18;
  _LATITUDE = 4;
  _LONGITUDE = 5;
}

void GeoNamesReader::close()
{
  _fp.close();
}

boost::shared_ptr<OGRSpatialReference> GeoNamesReader::getProjection() const
{
  if (!_wgs84)
  {
    _wgs84 = MapProjector::getInstance().createWgs84Projection();
  }

  return _wgs84;
}

bool GeoNamesReader::hasMoreElements()
{
  return !_fp.atEnd();
}

bool GeoNamesReader::isSupported(QString url)
{
  url = QDir().absoluteFilePath(url);
  QFile f(url);

  bool result = url.toLower().endsWith(".geonames") && f.exists();
  return result;
}

void GeoNamesReader::open(QString url)
{
  _fp.close();
  _fp.setFileName(QDir().absoluteFilePath(url));
  _elementsRead = 0;

  if (_fp.open(QFile::ReadOnly) == false)
  {
    throw HootException("Error opening file for reading: " + url);
  }
}

ElementPtr GeoNamesReader::readNextElement()
{
  QByteArray lineBytes = _fp.readLine();
  QString line = QString::fromUtf8(lineBytes.constData());
  QStringList fields = line.split('\t');

  bool ok;
  double x = fields[_LONGITUDE].toDouble(&ok);
  if (ok == false)
  {
    throw HootException(QString("Error parsing longitude (%1): %2").arg(fields[_LONGITUDE]).
      arg(line));
  }
  double y = fields[_LATITUDE].toDouble(&ok);
  if (ok == false)
  {
    throw HootException(QString("Error parsing latitude (%1): %2").arg(fields[_LATITUDE]).
      arg(line));
  }

  long id;
  if (_useDataSourceIds)
  {
    bool ok;
    id = fields[_GEONAMESID].toLong(&ok);
    if (!ok)
    {
      throw HootException("Error parsing geonames ID: " + line);
    }
  }
  else
  {
    id = _partialMap->createNextNodeId();
  }

  boost::shared_ptr<Node> n(new Node(_status, id, x, y, _circularError));

  if (_columns.size() != fields.size())
  {
    throw HootException(QString("Expected %1 fields but found %2 fields. %3").arg(_columns.size()).
      arg(fields.size()).arg(line));
  }

  for (int i = 0; i < _columns.size(); i++)
  {
    int j = i; //convertColumns[i];
    n->getTags()[_columns[j]] = _saveMemory(fields[j]);
  }

  return n;
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
