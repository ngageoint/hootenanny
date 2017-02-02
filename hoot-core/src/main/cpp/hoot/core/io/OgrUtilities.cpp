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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OgrUtilities.h"

// gdal
#include <gdal_frmts.h>
#include <gdal_priv.h>

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QStringList>

#include "OgrOptions.h"

namespace hoot
{

shared_ptr<OgrUtilities> OgrUtilities::_theInstance;

void OgrUtilities::loadDriverInfo()
{
  //  Load the extension-based driver info
  _drivers.push_back(OgrDriverInfo(".shp",      "ESRI Shapefile", true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".dbf",      "ESRI Shapefile", true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".sqlite",   "SQLite",         true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".db",       "SQLite",         true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".mif",      "MapInfo File",   true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".tab",      "MapInfo File",   true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".s57",      "S57",            true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".bna",      "BNA",            true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".csv",      "CSV",            true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gml",      "GML",            true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gpx",      "GPX",            true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".kml",      "KML/LIBKML",     true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo(".kmz",      "LIBKML",         true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo(".json",     "GeoJSON",        true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".geojson",  "GeoJSON",        true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".dxf",      "DXF",            true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gdb",      "FileGDB",        true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".pix",      "PCIDSK",         true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo(".sql",      "PGDump",         true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gtm",      "GPSTrackMaker",  true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gmt",      "GMT",            true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo(".vrt",      "VRT",            true,   GDAL_OF_ALL));
  //  Load the prefix-based driver info
  _drivers.push_back(OgrDriverInfo("PG:",       "PostgreSQL",     false,  GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("MySQL:",    "MySQL",          false,  GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo("CouchDB:",  "CouchDB",        false,  GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("GFT:",      "GFT",            false,  GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("MSSQL:",    "MSSQLSpatial",   false,  GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("ODBC:",     "ODBC",           false,  GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("OCI:",      "OCI",            false,  GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo("SDE:",      "SDE",            false,  GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo("WFS:",      "WFS",            false,  GDAL_OF_ALL));
}

OgrUtilities::OgrUtilities()
{
  GDALAllRegister();
  loadDriverInfo();
}

OgrUtilities& OgrUtilities::getInstance()
{
  if (!_theInstance.get())
      _theInstance.reset(new OgrUtilities());
  return *_theInstance;
}

OgrDriverInfo OgrUtilities::getDriverInfo(const QString& url)
{
  for (vector<OgrDriverInfo>::iterator it = _drivers.begin(); it != _drivers.end(); it++)
  {
    if ((it->_is_ext && url.endsWith(it->_indicator)) || (!it->_is_ext && url.startsWith(it->_indicator)))
        return *it;
  }
  return OgrDriverInfo();
}


shared_ptr<GDALDataset> OgrUtilities::createDataSource(const QString& url)
{
  QString source = url;
  OgrDriverInfo driverInfo = getDriverInfo(url);
  if (driverInfo._driverName == NULL)
    throw HootException("Error getting driver info for: " + url);
  GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(driverInfo._driverName);
  if (driver == 0)
    throw HootException("Error getting driver by name: " + QString(driverInfo._driverName));

  // if the user specifies a shapefile then crop off the .shp and create a directory.
  if (url.toLower().endsWith(".shp"))
    source = url.mid(0, url.length() - 4);

  shared_ptr<GDALDataset> result(driver->Create(source.toAscii(), 0, 0, 0, GDT_Unknown, NULL));
  if (result == NULL)
  {
    throw HootException("Unable to create data source: " + source +
                        " (" + QString(CPLGetLastErrorMsg()) + ")");
  }

  return result;
}

bool OgrUtilities::isReasonableUrl(const QString& url)
{
  return getDriverInfo(url)._driverName != NULL;
}

shared_ptr<GDALDataset> OgrUtilities::openDataSource(const QString& url)
{
  /* Check for the correct driver name, if unknown try all drivers.
   * This can be an issue because drivers are tried in the order that they are
   * loaded which has been known to cause issues.
   */
  OgrDriverInfo driverInfo = getDriverInfo(url);
  const char* drivers[2] = { driverInfo._driverName, NULL };
  shared_ptr<GDALDataset> result((GDALDataset*)GDALOpenEx(url.toUtf8().data(),
    driverInfo._driverType, (driverInfo._driverName != NULL ? drivers : NULL), NULL, NULL));

  if (!result)
    throw HootException("Unable to open: " + url);

  return result;
}

}
