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
#include <gdal.h>
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

static const char* extensions[][2] = { { "shp"    , "ESRI Shapefile" },
                                       { "dbf"    , "ESRI Shapefile" },
                                       { "sqlite" , "SQLite" },
                                       { "db"     , "SQLite" },
                                       { "mif"    , "MapInfo File" },
                                       { "tab"    , "MapInfo File" },
                                       { "s57"    , "S57" },
                                       { "bna"    , "BNA" },
                                       { "csv"    , "CSV" },
                                       { "gml"    , "GML" },
                                       { "gpx"    , "GPX" },
                                       { "kml"    , "KML/LIBKML" },
                                       { "kmz"    , "LIBKML" },
                                       { "json"   , "GeoJSON" },
                                       { "geojson", "GeoJSON" },
                                       { "dxf"    , "DXF" },
                                       { "gdb"    , "FileGDB" },
                                       { "pix"    , "PCIDSK" },
                                       { "sql"    , "PGDump" },
                                       { "gtm"    , "GPSTrackMaker" },
                                       { "gmt"    , "GMT" },
                                       { "vrt"    , "VRT" },
                                       { NULL, NULL }
                                      };
static const char* beginName[][2] =  { { "PG:"      , "PostgreSQL" },
                                       { "MySQL:"   , "MySQL" },
                                       { "CouchDB:" , "CouchDB" },
                                       { "GFT:"     , "GFT" },
                                       { "MSSQL:"   , "MSSQLSpatial" },
                                       { "ODBC:"    , "ODBC" },
                                       { "OCI:"     , "OCI" },
                                       { "SDE:"     , "SDE" },
                                       { "WFS:"     , "WFS" },
                                       { NULL, NULL }
                                     };

shared_ptr<OgrUtilities> OgrUtilities::_theInstance;

OgrUtilities::OgrUtilities()
{
  GDALAllRegister();
}

OgrUtilities& OgrUtilities::getInstance()
{
  if (!_theInstance.get())
      _theInstance.reset(new OgrUtilities());
  return *_theInstance;
}

const char* OgrUtilities::getDriverName(const QString& url)
{
  const char* driverName = NULL;
  int i = 0;
  while (extensions[i][0] != NULL)
  {
    if (url.endsWith(extensions[i][0]))
      driverName = extensions[i][1];
    i++;
  }
  i = 0;
  while (beginName[i][0] != NULL)
  {
    if (url.startsWith(beginName[i][0]))
      driverName = beginName[i][1];
    i++;
  }
  return driverName;
}


shared_ptr<GDALDataset> OgrUtilities::createDataSource(const QString& url)
{
  QString source = url;
  const char *driverName = getDriverName(url);
  GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(driverName);
  if (driver == 0)
    throw HootException("Error getting driver by name: " + QString(driverName));

  // if the user specifies a shapefile then crop off the .shp and create a directory.
  if (url.toLower().endsWith(".shp"))
    source = url.mid(0, url.length() - 4);

  shared_ptr<GDALDataset> result(driver->Create(source.toAscii(), 0, 0, 0, GDT_Unknown, NULL));
  if (result == NULL)
  {
    throw HootException("Unable to create data source: " + source +
                        " (" + QString(CPLGetLastErrorMsg()) + ")");
  }

  if (QString(driverName) == "FileGDB")
  {
    long v = GDAL_VERSION_MAJOR * 1000000 + GDAL_VERSION_MINOR * 1000 + GDAL_VERSION_REV;
    long lowest = 1 * 1000000 + 10 * 1000 + 1;
    if (v < lowest)
    {
      LOG_WARN("Writing to FileGDB with GDAL v" << GDAL_RELEASE_NAME << ". FileGDB with a GDAL "
               "v1.9.0 is known to create files that can't be read by ArcMap 10.2. "
               "GDAL v1.10.1 is known to work.");
    }
  }

  return result;
}

bool OgrUtilities::isReasonableUrl(const QString& url)
{
  return getDriverName(url) != NULL;
}

shared_ptr<GDALDataset> OgrUtilities::openDataSource(const QString& url)
{
  /* Check for the correct driver name, if unknown try all drivers.
   * This can be an issue because drivers are tried in the order that they are
   * loaded which has been known to cause issues.
   */
  const char* driver = getDriverName(url);
  const char* drivers[2] = { driver, NULL };
  shared_ptr<GDALDataset> result((GDALDataset*)GDALOpenEx(url.toUtf8().data(), GDAL_OF_ALL,
    (driver != NULL ? drivers : NULL), NULL, NULL));

  if (!result)
    throw HootException("Unable to open: " + url);

  return result;
}

}
