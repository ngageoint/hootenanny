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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OgrUtilities.h"

// gdal
#include <gdal_frmts.h>
#include <gdal_priv.h>

// hoot
#include <hoot/core/io/OgrOptions.h>
#include <hoot/core/util/ConfigOptions.h>

using namespace std;

namespace hoot
{

void OgrUtilities::loadDriverInfo()
{
  //  Load the extension-based driver info
  //                    EXT          DESCRIPTION       EXT/PRE   R/W     VECTOR/RASTER/BOTH
  _drivers.emplace_back(".shp",      "ESRI Shapefile", true,     true,   GDAL_OF_VECTOR);
//  _drivers.emplace_back(".dbf",      "ESRI Shapefile", true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".sqlite",   "SQLite",         true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".db",       "SQLite",         true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".mif",      "MapInfo File",   true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".tab",      "MapInfo File",   true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".s57",      "S57",            true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".000",      "S57",            true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".bna",      "BNA",            true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".csv",      "CSV",            true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".gml",      "GML",            true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".gpx",      "GPX",            true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".kml",      "LIBKML",         true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".kmz",      "LIBKML",         true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".dxf",      "DXF",            true,     true,   GDAL_OF_VECTOR);
  // Order is important here for the two FileGDB drivers, grab the first for read ops and the second
  // for write
  _drivers.emplace_back(".gdb",      "OpenFileGDB",    true,     false,  GDAL_OF_VECTOR);
  _drivers.emplace_back(".gdb",      "FileGDB",        true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".gpkg",     "GPKG",           true,     true,   GDAL_OF_ALL);
  _drivers.emplace_back(".pix",      "PCIDSK",         true,     true,   GDAL_OF_ALL);
  _drivers.emplace_back(".sql",      "PGDump",         true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".gtm",      "GPSTrackMaker",  true,     true,   GDAL_OF_VECTOR);
  _drivers.emplace_back(".gmt",      "GMT",            true,     true,   GDAL_OF_ALL);
  _drivers.emplace_back(".vrt",      "VRT",            true,     true,   GDAL_OF_ALL);
  //  Load the prefix-based driver info
  _drivers.emplace_back("PG:",       "PostgreSQL",     false,    true,   GDAL_OF_VECTOR);
  _drivers.emplace_back("MySQL:",    "MySQL",          false,    true,   GDAL_OF_ALL);
  _drivers.emplace_back("CouchDB:",  "CouchDB",        false,    true,   GDAL_OF_VECTOR);
  _drivers.emplace_back("GFT:",      "GFT",            false,    true,   GDAL_OF_VECTOR);
  _drivers.emplace_back("gltp:",     "OGR_OGDI",       false,    true,   GDAL_OF_VECTOR);
  _drivers.emplace_back("MSSQL:",    "MSSQLSpatial",   false,    true,   GDAL_OF_VECTOR);
  _drivers.emplace_back("ODBC:",     "ODBC",           false,    true,   GDAL_OF_VECTOR);
  _drivers.emplace_back("OCI:",      "OCI",            false,    true,   GDAL_OF_ALL);
  _drivers.emplace_back("SDE:",      "SDE",            false,    true,   GDAL_OF_ALL);
  _drivers.emplace_back("WFS:",      "WFS",            false,    true,   GDAL_OF_ALL);
}

OgrUtilities::OgrUtilities()
{
  GDALAllRegister();
  loadDriverInfo();
  //  Turn off writing a properties file for GZIP operations in GDAL
  CPLSetConfigOption("CPL_VSIL_GZIP_WRITE_PROPERTIES", "NO");
}

OgrUtilities::~OgrUtilities()
{
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    GDALDumpOpenDatasets(stderr);
    CPLDumpSharedList(nullptr);
  }

  LOG_TRACE("Cleaning up OGR...");
  OGRCleanupAll();
  LOG_TRACE("Destroying driver manager...");
  GDALDestroyDriverManager();
}

OgrUtilities& OgrUtilities::getInstance()
{
  //  Local static singleton instance
  static OgrUtilities instance;
  return instance;
}

QSet<QString> OgrUtilities::getSupportedFormats(const bool readOnly) const
{
  QSet<QString> formats;
  for (vector<OgrDriverInfo>::const_iterator it = _drivers.begin(); it != _drivers.end(); ++it)
  {
    if (readOnly || it->_is_rw)
    {
      formats.insert(QString::fromLatin1(it->_indicator));
    }
  }
  return formats;
}

OgrDriverInfo OgrUtilities::getDriverInfo(const QString& url, bool readonly) const
{
  for (vector<OgrDriverInfo>::const_iterator it = _drivers.begin(); it != _drivers.end(); ++it)
  {
    if (((it->_is_ext && url.endsWith(it->_indicator)) ||
         (!it->_is_ext && url.startsWith(it->_indicator))) && (readonly || it->_is_rw))
    {
      return *it;
    }
  }
  return OgrDriverInfo();
}

std::shared_ptr<GDALDataset> OgrUtilities::createDataSource(const QString& url) const
{
  QString source = url;
  OgrDriverInfo driverInfo = getDriverInfo(url, false);
  if (driverInfo._driverName == nullptr)
    throw HootException("Error getting driver info for: " + url);
  GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(driverInfo._driverName);
  if (driver == nullptr)
    throw HootException("Error getting driver by name: " + QString(driverInfo._driverName));

  // If the user specifies a shapefile, then crop off the .shp and create a directory.
  if (url.toLower().endsWith(".shp"))
    source = url.mid(0, url.length() - 4);

  std::shared_ptr<GDALDataset> result(driver->Create(source.toLatin1(), 0, 0, 0, GDT_Unknown, nullptr));
  if (result == nullptr)
  {
    throw HootException("Unable to create data source: " + source +
                        " (" + QString(CPLGetLastErrorMsg()) + ")");
  }

  return result;
}

bool OgrUtilities::isReasonableUrl(const QString& url) const
{
  QString relative_url = url;
  //  /vsi* files should have the "/vsi*/" portion of the URL removed before checking the file type
  if (url.startsWith("/vsi"))
    relative_url = url.right(url.length() - url.indexOf("/", 4) - 1);
  //  Check if there is a valid driver for this file type
  return getDriverInfo(relative_url, true)._driverName != nullptr;
}

std::shared_ptr<GDALDataset> OgrUtilities::openDataSource(const QString& url, bool readonly) const
{
  /* Check for the correct driver name, if unknown try all drivers.
   * This can be an issue because drivers are tried in the order that they are
   * loaded which has been known to cause issues.
   */
  OgrDriverInfo driverInfo = getDriverInfo(url, readonly);

  // With GDALOpenEx, we need to specify the GDAL_OF_UPDATE option or the dataset will get opened
  // Read Only.
  if (!readonly)
  {
    driverInfo._driverType = driverInfo._driverType | GDAL_OF_UPDATE;
  }
  LOG_VART(driverInfo._driverName);
  LOG_VART(driverInfo._driverType);
  LOG_VART(url.toUtf8().data());

  const char* drivers[2] = { driverInfo._driverName, nullptr };

  // Setup read options for various file types
  OgrOptions options;
  if (QString(driverInfo._driverName) == "CSV")
  {
    options["X_POSSIBLE_NAMES"] = ConfigOptions().getOgrReaderCsvLonfield();
    options["Y_POSSIBLE_NAMES"] = ConfigOptions().getOgrReaderCsvLatfield();
    options["KEEP_GEOM_COLUMNS"] = ConfigOptions().getOgrReaderCsvKeepGeomFields();
  }
  else if (QString(driverInfo._driverName) == "OGR_OGDI")
  {
    // From GDAL/OGR 1.8.0 on, setting the OGR_OGDI_LAUNDER_LAYER_NAMES configuration option (or
    // environment variable) to YES causes the layer names to be simplified. For example,
    // watrcrsl_hydro instead of 'watrcrsl@hydro(*)_line'.
    options["OGR_OGDI_LAUNDER_LAYER_NAMES"] = ConfigOptions().getOgrReaderOgdiLaunderLayerNames();
  }
  else if (QString(driverInfo._driverName) == "LIBKML")
  {
    options["OSM_USE_CUSTOM_INDEXING"] = "NO";
  }
  else if (QString(driverInfo._driverName) == "S57")
  {
    // SPLIT_MULTIPOINT=ON/OFF: Should multipoint soundings be split into many single point sounding
    // features. Multipoint geometries are not well handle by many formats, so it can be convenient
    // to split single sounding features with many points into many single point features.Default is
    // OFF.
    options["SPLIT_MULTIPOINT"] = "ON";

    // ADD_SOUNDG_DEPTH=ON/OFF: Should a DEPTH attribute be added on SOUNDG features and assign the
    // depth of the sounding. This should only be enabled with SPLIT_MULTIPOINT is also enabled.
    // Default is OFF.
    options["ADD_SOUNDG_DEPTH"] = "ON";
  }

  std::shared_ptr<GDALDataset> result(
      static_cast<GDALDataset*>(
        GDALOpenEx(
          url.toUtf8().data(), driverInfo._driverType,
          (driverInfo._driverName != nullptr ? drivers : nullptr),
          options.getCrypticOptions(), nullptr)));
  if (!result)
  {
    QString errorMsg = "Unable to open: " + url + ".";
    if (url.contains(".zip"))
    {
      errorMsg += " Is your path within the zip file correct?";
    }
    throw HootException(errorMsg);
  }
  return result;
}

QStringList OgrUtilities::getValidFilesInContainer(const QString& url) const
{
  QString path = url;
  QStringList files;
  //  Ignore anything but files with these extensions
  QStringList extensions({".zip", ".tar", ".tar.gz", ".tgz", ".gz"});
  bool found = false;
  for (int i = 0; i < extensions.length(); ++i)
  {
    if (path.endsWith(extensions[i], Qt::CaseInsensitive))
      found = true;
  }
  //  If this file doesn't end with any of the above extensions return an empty list
  if (!found)
    return files;
  //  Prepend /vsizip/ to the URL if needed
  if (path.endsWith(".zip", Qt::CaseInsensitive))
  {
    if (!path.startsWith("/vsizip/"))
      path.prepend("/vsizip/");
  }
  else if (path.endsWith(".tar", Qt::CaseInsensitive) ||
           path.endsWith(".tar.gz", Qt::CaseInsensitive) ||
           path.endsWith(".tgz", Qt::CaseInsensitive))
  {
    if (!path.startsWith("/vsitar/"))
      path.prepend("/vsitar/");
  }
  else if (path.endsWith(".gz", Qt::CaseInsensitive))
  {
    if (!path.startsWith("/vsigzip/"))
      path.prepend("/vsitar/");
  }
  //  Open the container file with GDAL
  char** file_list = VSIReadDirRecursive(path.toStdString().c_str());
  if (file_list)
  {
    //  Iterate all of the files found
    int count = CSLCount(file_list);
    for (int i = 0; i < count; ++i)
    {
      //  Check if the file found is a reasonable filename and store it
      QString file = path + "/" + file_list[i];
      if (isReasonableUrl(file))
        files.append(file);
    }
    //  Destroy the GDAL memory buffer
    CSLDestroy(file_list);
  }
  //  Return the list of valid files, if any
  return files;
}

}
