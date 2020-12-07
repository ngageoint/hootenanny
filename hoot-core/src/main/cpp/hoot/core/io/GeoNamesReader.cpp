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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "GeoNamesReader.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>

// qt
#include <QDir>
#include <QFile>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, GeoNamesReader)

GeoNamesReader::GeoNamesReader() :
_maxSaveMemoryStrings(ConfigOptions().getGeonamesReaderStringCacheSize()),
_defaultCircularError(ConfigOptions().getCircularErrorDefaultValue()),
_useDataSourceIds(false)
{
  // Geonames.org column names
//  _columns << "geonameid";
//  _columns << "name";
//  _columns << "asciiname";
//  _columns << "alternatenames";
//  _columns << "latitude";
//  _columns << "longitude";
//  _columns << "feature_class";
//  _columns << "feature_code";
//  _columns << "country_code";
//  _columns << "cc2";
//  _columns << "admin1_code";
//  _columns << "admin2_code";
//  _columns << "admin3_code";
//  _columns << "admin4_code";
//  _columns << "population";
//  _columns << "elevation";
//  _columns << "dem";
//  _columns << "timezone";
//  _columns << "modification_date";

  // Commenting this out for a while.  I'm not sure what it was supposed to do
//  _convertColumns << 1 << 3 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 18;

//  // Geonames.nga.mil column names
//  // https://geonames.nga.mil/gns/html/gis_countryfiles.html
//  _mil_columns << "region_font_code"; // RC
//  _mil_columns << "ufi";
//  _mil_columns << "uni";
//  _mil_columns << "latitude";
//  _mil_columns << "longitude";
//  _mil_columns << "dms_latitude"; // DMS_LAT
//  _mil_columns << "dms_longitude"; // DMS_LONG
//  _mil_columns << "mgrs";
//  _mil_columns << "jog";
//  _mil_columns << "feature_class"; // FC
//  _mil_columns << "feature_code"; // DSG
//  _mil_columns << "populated_place_code"; // PC
//  _mil_columns << "country_code"; // CC1
//  _mil_columns << "admin1_code"; // ADM1
//  _mil_columns << "population"; // POP - not populated
//  _mil_columns << "elevation"; // ELEV - not populated
//  _mil_columns << "secondary_country_code"; // CC2
//  _mil_columns << "name_type"; // NT
//  _mil_columns << "language_code"; // LC
//  _mil_columns << "short_name"; // SHORT_FORM
//  _mil_columns << "generic_name"; // GENERIC
//  _mil_columns << "sort_name_reading_order"; // SORT_NAME_RO
//  _mil_columns << "full_name_reading_order"; // FULL_NAME_RO
//  _mil_columns << "full_name_reading_order_no_diacritics"; // FULL_NAME_ND_RO
//  _mil_columns << "sort_name_reversed_generic"; // SORT_NAME_RG
//  _mil_columns << "full_name_reversed_generic"; // FULL_NAME_RG
//  _mil_columns << "full_name_reversed_generic_no_diacritics"; // FULL_NAME_ND_RO
//  _mil_columns << "note"; // NOTE
//  _mil_columns << "modification_date"; // MODIFY_DATE
//  _mil_columns << "display_scale"; // DISPLAY
//  _mil_columns << "name_rank"; // NAME_RANK
//  _mil_columns << "name_link"; // NAME_LINK
//  _mil_columns << "transliteration_code"; // TRANSL_CODE
//  _mil_columns << "name_modification_date"; // NM_MODIFY_DATE
//  _mil_columns << "effective_date"; // F_EFCTV_DDT
//  _mil_columns << "termination_date"; // F_TERM_DT

//  _mil_convertColumns << 1 << 3 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 18;

}

void GeoNamesReader::close()
{
  _fp.close();
}

std::shared_ptr<OGRSpatialReference> GeoNamesReader::getProjection() const
{
  if (!_wgs84)
  {
    _wgs84 = MapProjector::createWgs84Projection();
  }
  return _wgs84;
}

bool GeoNamesReader::hasMoreElements()
{
  return !_fp.atEnd();
}

bool GeoNamesReader::isSupported(const QString& url)
{
  QString path = QDir().absoluteFilePath(url);
  QFile f(path);

  bool result = path.toLower().endsWith(".geonames") && f.exists();
  return result;
}

void GeoNamesReader::open(const QString& url)
{
  OsmMapReader::open(url);
  _fp.close();
  _fp.setFileName(QDir().absoluteFilePath(url));
  _elementsRead = 0;

  if (_fp.open(QFile::ReadOnly) == false)
  {
    throw HootException("Error opening file for reading: " + url);
  }

  // Now figure out what type of Geonames file we have
  QString line = QString::fromUtf8(_fp.readLine().constData());
  QStringList fields;
  fields.append(line.split('\t'));

//  if (_mil_columns.size() == fields.size())
  if (fields.size() == 36)
  {
    _GEONAMESID = 1;  // UFI Field
    _LATITUDE = 3;
    _LONGITUDE = 4;

    // Geonames.nga.mil column names
    // https://geonames.nga.mil/gns/html/gis_countryfiles.html
    _columns << "region_font_code"; // RC
    _columns << "ufi";
    _columns << "uni";
    _columns << "latitude";
    _columns << "longitude";
    _columns << "dms_latitude"; // DMS_LAT
    _columns << "dms_longitude"; // DMS_LONG
    _columns << "mgrs";
    _columns << "jog";
    _columns << "feature_class"; // FC
    _columns << "feature_code"; // DSG
    _columns << "populated_place_code"; // PC
    _columns << "country_code"; // CC1
    _columns << "admin1_code"; // ADM1
    _columns << "population"; // POP - not populated
    _columns << "elevation"; // ELEV - not populated
    _columns << "secondary_country_code"; // CC2
    _columns << "name_type"; // NT
    _columns << "language_code"; // LC
    _columns << "short_name"; // SHORT_FORM
    _columns << "generic_name"; // GENERIC
    _columns << "sort_name_reading_order"; // SORT_NAME_RO
    _columns << "full_name_reading_order"; // FULL_NAME_RO
    _columns << "full_name_reading_order_no_diacritics"; // FULL_NAME_ND_RO
    _columns << "sort_name_reversed_generic"; // SORT_NAME_RG
    _columns << "full_name_reversed_generic"; // FULL_NAME_RG
    _columns << "full_name_reversed_generic_no_diacritics"; // FULL_NAME_ND_RO
    _columns << "note"; // NOTE
    _columns << "modification_date"; // MODIFY_DATE
    _columns << "display_scale"; // DISPLAY
    _columns << "name_rank"; // NAME_RANK
    _columns << "name_link"; // NAME_LINK
    _columns << "transliteration_code"; // TRANSL_CODE
    _columns << "name_modification_date"; // NM_MODIFY_DATE
    _columns << "effective_date"; // F_EFCTV_DDT
    _columns << "termination_date"; // F_TERM_DT
  }
  else
  {
    // Reset the text file since we do not have a header line with Geonames.Org
    _fp.seek(0);

    _GEONAMESID = 0;
    _LATITUDE = 4;
    _LONGITUDE = 5;

    // Geonames.org column names
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

  NodePtr n(Node::newSp(_status, id, x, y, _defaultCircularError));

  if (_columns.size() != fields.size())
  {
    throw HootException(QString("Expected %1 fields but found %2 fields. %3").arg(_columns.size()).
      arg(fields.size()).arg(line));
  }

  for (int i = 0; i < _columns.size(); i++)
  {
    int j = i; //convertColumns[i];
    QString val = fields[j].trimmed();
    if (!val.isEmpty())
    {
      n->getTags()[_columns[j]] = _saveMemory(val);
    }
  }

  LOG_VART(n);
  return n;
}

QString GeoNamesReader::_saveMemory(const QString& s)
{
  QString result;
  if (_strings.size() < _maxSaveMemoryStrings)
  {
    if (!_strings.contains(s))
    {
      result = s;
      _strings[s] = s;
    }
    else
    {
      result = _strings[s];
    }
  }
  else
  {
    result = s;
  }

  return result;
}

}
