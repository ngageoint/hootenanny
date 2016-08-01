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
#include <ogrsf_frmts.h>

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
static const char* beginName[][2] =  { { "PG:"      , "PGeo" },
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
  OGRRegisterAll();
}

OgrUtilities& OgrUtilities::getInstance()
{
  if (!_theInstance.get())
  {
      _theInstance.reset(new OgrUtilities());
  }
  return *_theInstance;
}

shared_ptr<OGRDataSource> OgrUtilities::createDataSource(QString url)
{
  const char* driverName = NULL;
  int i = 0;
  while (extensions[i][0] != NULL)
  {
    if (url.endsWith(extensions[i][0]))
    {
      driverName = extensions[i][1];
    }
    i++;
  }
  i = 0;
  while (beginName[i][0] != NULL)
  {
    if (url.startsWith(beginName[i][0]))
    {
      driverName = beginName[i][1];
    }
    i++;
  }
  if (driverName == NULL)
  {
    throw HootException("No driver found for: " + url);
  }

  OGRSFDriver* driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(driverName);
  if (driver == 0)
  {
    throw HootException("Error getting driver by name: " + QString(driverName));
  }

  // if the user specifies a shapefile then crop off the .shp and create a directory.
  if (url.toLower().endsWith(".shp"))
  {
    url = url.mid(0, url.length() - 4);
  }

  shared_ptr<OGRDataSource> result(driver->CreateDataSource(url.toAscii()));
  if (result == NULL)
  {
    throw HootException("Unable to create data source: " + url +
                        " (" + QString(CPLGetLastErrorMsg()) + ")");
  }
  result->SetDriver(driver);

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

bool OgrUtilities::isReasonableUrl(QString url)
{
  bool reasonable = false;

  int i = 0;
  while (extensions[i][0] != NULL)
  {
    if (url.endsWith(extensions[i][0]))
    {
      reasonable = true;
    }
    i++;
  }
  i = 0;
  while (beginName[i][0] != NULL)
  {
    if (url.startsWith(beginName[i][0]))
    {
      reasonable = true;
    }
    i++;
  }

  return reasonable;
}

shared_ptr<OGRDataSource> OgrUtilities::openDataSource(QString url)
{
  shared_ptr<OGRDataSource> result;

  result.reset(OGRSFDriverRegistrar::Open(url.toUtf8().data(), false));

  if (!result)
  {
    throw HootException("Unable to open: " + url);
  }

  return result;
}

}
