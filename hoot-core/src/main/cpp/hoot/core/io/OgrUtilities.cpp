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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
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

using namespace std;

namespace hoot
{

boost::shared_ptr<OgrUtilities> OgrUtilities::_theInstance;

void OgrUtilities::loadDriverInfo()
{
  //  Load the extension-based driver info
  //                               EXT          DESCRIPTION       EXT/PRE   R/W     VECTOR/RASTER/BOTH
  _drivers.push_back(OgrDriverInfo(".shp",      "ESRI Shapefile", true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".dbf",      "ESRI Shapefile", true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".sqlite",   "SQLite",         true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".db",       "SQLite",         true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".mif",      "MapInfo File",   true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".tab",      "MapInfo File",   true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".s57",      "S57",            true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".bna",      "BNA",            true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".csv",      "CSV",            true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gml",      "GML",            true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gpx",      "GPX",            true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".kml",      "KML/LIBKML",     true,     true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo(".kmz",      "LIBKML",         true,     true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo(".json",     "GeoJSON",        true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".geojson",  "GeoJSON",        true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".dxf",      "DXF",            true,     true,   GDAL_OF_VECTOR));
  //  Order is important here for the two FileGDB drivers, grab the first for read ops and the second for write
  _drivers.push_back(OgrDriverInfo(".gdb",      "OpenFileGDB",    true,     false,  GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gdb",      "FileGDB",        true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".pix",      "PCIDSK",         true,     true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo(".sql",      "PGDump",         true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gtm",      "GPSTrackMaker",  true,     true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo(".gmt",      "GMT",            true,     true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo(".vrt",      "VRT",            true,     true,   GDAL_OF_ALL));
  //  Load the prefix-based driver info
  _drivers.push_back(OgrDriverInfo("PG:",       "PostgreSQL",     false,    true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("MySQL:",    "MySQL",          false,    true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo("CouchDB:",  "CouchDB",        false,    true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("GFT:",      "GFT",            false,    true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("MSSQL:",    "MSSQLSpatial",   false,    true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("ODBC:",     "ODBC",           false,    true,   GDAL_OF_VECTOR));
  _drivers.push_back(OgrDriverInfo("OCI:",      "OCI",            false,    true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo("SDE:",      "SDE",            false,    true,   GDAL_OF_ALL));
  _drivers.push_back(OgrDriverInfo("WFS:",      "WFS",            false,    true,   GDAL_OF_ALL));
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

OgrDriverInfo OgrUtilities::getDriverInfo(const QString& url, bool readonly)
{
  for (vector<OgrDriverInfo>::iterator it = _drivers.begin(); it != _drivers.end(); ++it)
  {
    if (((it->_is_ext && url.endsWith(it->_indicator)) || (!it->_is_ext && url.startsWith(it->_indicator))) &&
        (readonly || it->_is_rw))
    {
        return *it;
    }
  }
  return OgrDriverInfo();
}


boost::shared_ptr<GDALDataset> OgrUtilities::createDataSource(const QString& url)
{
  QString source = url;
  OgrDriverInfo driverInfo = getDriverInfo(url, false);
  if (driverInfo._driverName == NULL)
    throw HootException("Error getting driver info for: " + url);
  GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(driverInfo._driverName);
  if (driver == 0)
    throw HootException("Error getting driver by name: " + QString(driverInfo._driverName));

  // if the user specifies a shapefile then crop off the .shp and create a directory.
  if (url.toLower().endsWith(".shp"))
    source = url.mid(0, url.length() - 4);

  boost::shared_ptr<GDALDataset> result(driver->Create(source.toAscii(), 0, 0, 0, GDT_Unknown, NULL));
  if (result == NULL)
  {
    throw HootException("Unable to create data source: " + source +
                        " (" + QString(CPLGetLastErrorMsg()) + ")");
  }

  return result;
}

bool OgrUtilities::isReasonableUrl(const QString& url)
{
  // Not a pretty way to sort out the /vsizip/ driver problem
  // We are searching for "/vsi" which will match /vsizip, /vsigzip, /vsitar, /vsicurl
  // I am not sure if matching this many things is a good idea.
  if (url.startsWith("/vsi"))
  {
    return true;
  }

  return getDriverInfo(url, true)._driverName != NULL;
}

boost::shared_ptr<GDALDataset> OgrUtilities::openDataSource(const QString& url, bool readonly)
{
  /* Check for the correct driver name, if unknown try all drivers.
   * This can be an issue because drivers are tried in the order that they are
   * loaded which has been known to cause issues.
   */
  OgrDriverInfo driverInfo = getDriverInfo(url, readonly);
  const char* drivers[2] = { driverInfo._driverName, NULL };
  boost::shared_ptr<GDALDataset> result(static_cast<GDALDataset*>(GDALOpenEx(url.toUtf8().data(),
    driverInfo._driverType, (driverInfo._driverName != NULL ? drivers : NULL), NULL, NULL)));

  if (!result)
    throw HootException("Unable to open: " + url);

  return result;
}

}
