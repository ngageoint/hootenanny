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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "GeoNamesReader.h"

// hoot
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, GeoNamesReader)

GeoNamesReader::GeoNamesReader()
  : _maxSaveMemoryStrings(ConfigOptions().getGeonamesReaderStringCacheSize()),
    _defaultCircularError(ConfigOptions().getCircularErrorDefaultValue()),
    _useDataSourceIds(false)
{
}

void GeoNamesReader::close()
{
  _fp.close();
}

std::shared_ptr<OGRSpatialReference> GeoNamesReader::getProjection() const
{
  if (!_wgs84)
    _wgs84 = MapProjector::createWgs84Projection();
  return _wgs84;
}

bool GeoNamesReader::hasMoreElements()
{
  return !_fp.atEnd();
}

bool GeoNamesReader::isSupported(const QString& url) const
{
  QString path = QDir().absoluteFilePath(url);
  QFile f(path);
  bool result = path.endsWith(".geonames", Qt::CaseInsensitive) && f.exists();
  return result;
}

void GeoNamesReader::open(const QString& url)
{
  OsmMapReader::open(url);
  _fp.close();
  _fp.setFileName(QDir().absoluteFilePath(url));
  _elementsRead = 0;

  if (_fp.open(QFile::ReadOnly) == false)
    throw HootException("Error opening file for reading: " + url);

  // Now figure out what type of Geonames file we have
  QString line = QString::fromUtf8(_fp.readLine().constData());
  QStringList fields;
  fields.append(line.split('\t'));

  if (fields.size() == 36)
  {
    _GEONAMESID = 1;  // UFI Field
    _LATITUDE = 3;
    _LONGITUDE = 4;

    // Geonames.nga.mil column names
    // https://geonames.nga.mil/gns/html/gis_countryfiles.html
    _columns << getStringLocation("region_font_code"); // RC
    _columns << getStringLocation("ufi");
    _columns << getStringLocation("uni");
    _columns << getStringLocation("latitude");
    _columns << getStringLocation("longitude");
    _columns << getStringLocation("dms_latitude"); // DMS_LAT
    _columns << getStringLocation("dms_longitude"); // DMS_LONG
    _columns << getStringLocation("mgrs");
    _columns << getStringLocation("jog");
    _columns << getStringLocation("feature_class"); // FC
    _columns << getStringLocation("feature_code"); // DSG
    _columns << getStringLocation("populated_place_code"); // PC
    _columns << getStringLocation("country_code"); // CC1
    _columns << getStringLocation("admin1_code"); // ADM1
    _columns << getStringLocation("population"); // POP - not populated
    _columns << getStringLocation("elevation"); // ELEV - not populated
    _columns << getStringLocation("secondary_country_code"); // CC2
    _columns << getStringLocation("name_type"); // NT
    _columns << getStringLocation("language_code"); // LC
    _columns << getStringLocation("short_name"); // SHORT_FORM
    _columns << getStringLocation("generic_name"); // GENERIC
    _columns << getStringLocation("sort_name_reading_order"); // SORT_NAME_RO
    _columns << getStringLocation("full_name_reading_order"); // FULL_NAME_RO
    _columns << getStringLocation("full_name_reading_order_no_diacritics"); // FULL_NAME_ND_RO
    _columns << getStringLocation("sort_name_reversed_generic"); // SORT_NAME_RG
    _columns << getStringLocation("full_name_reversed_generic"); // FULL_NAME_RG
    _columns << getStringLocation("full_name_reversed_generic_no_diacritics"); // FULL_NAME_ND_RO
    _columns << getStringLocation("note"); // NOTE
    _columns << getStringLocation("modification_date"); // MODIFY_DATE
    _columns << getStringLocation("display_scale"); // DISPLAY
    _columns << getStringLocation("name_rank"); // NAME_RANK
    _columns << getStringLocation("name_link"); // NAME_LINK
    _columns << getStringLocation("transliteration_code"); // TRANSL_CODE
    _columns << getStringLocation("name_modification_date"); // NM_MODIFY_DATE
    _columns << getStringLocation("effective_date"); // F_EFCTV_DDT
    _columns << getStringLocation("termination_date"); // F_TERM_DT
  }
  else
  {
    // Reset the text file since we do not have a header line with Geonames.Org
    _fp.seek(0);

    _GEONAMESID = 0;
    _LATITUDE = 4;
    _LONGITUDE = 5;

    // Geonames.org column names
    _columns << getStringLocation("geonameid");
    _columns << getStringLocation("name");
    _columns << getStringLocation("asciiname");
    _columns << getStringLocation("alternatenames");
    _columns << getStringLocation("latitude");
    _columns << getStringLocation("longitude");
    _columns << getStringLocation("feature_class");
    _columns << getStringLocation("feature_code");
    _columns << getStringLocation("country_code");
    _columns << getStringLocation("cc2");
    _columns << getStringLocation("admin1_code");
    _columns << getStringLocation("admin2_code");
    _columns << getStringLocation("admin3_code");
    _columns << getStringLocation("admin4_code");
    _columns << getStringLocation("population");
    _columns << getStringLocation("elevation");
    _columns << getStringLocation("dem");
    _columns << getStringLocation("timezone");
    _columns << getStringLocation("modification_date");
  }

}

ElementPtr GeoNamesReader::readNextElement()
{
  QString line = QString::fromUtf8(_fp.readLine().constData());
  LOG_VART(line);
  QStringList fields;
  fields.append(line.split('\t'));
  LOG_VART(fields);

  bool ok;
  double x = fields[_LONGITUDE].toDouble(&ok);
  LOG_VART(x);
  if (ok == false)
    throw HootException(QString("Error parsing longitude (%1): %2").arg(fields[_LONGITUDE], line));

  double y = fields[_LATITUDE].toDouble(&ok);
  if (ok == false)
    throw HootException(QString("Error parsing latitude (%1): %2").arg(fields[_LATITUDE], line));

  long id;
  if (_useDataSourceIds)
  {
    id = fields[_GEONAMESID].toLong(&ok);
    if (!ok)
      throw HootException("Error parsing geonames ID: " + line);
  }
  else
    id = _partialMap->createNextNodeId();

  NodePtr n(Node::newSp(_status, id, x, y, _defaultCircularError));

  if (_columns.size() != fields.size())
    throw HootException(QString("Expected %1 fields but found %2 fields. %3").arg(_columns.size()).arg(fields.size()).arg(line));

  for (int i = 0; i < _columns.size(); i++)
  {
    int j = i; //convertColumns[i];
    QString val = fields[j].trimmed();
    if (!val.isEmpty())
      n->getTags()[_columns[j]] = getStringLocation(val);
  }

  LOG_VART(n);
  return n;
}

}
