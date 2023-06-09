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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#include "OgrUtilities.h"

// gdal
#include <gdal_frmts.h>
#include <gdal_priv.h>

// hoot
#include <hoot/core/util/ConfigOptions.h>

using namespace std;

namespace hoot
{

void OgrUtilities::_loadDriverInfo()
{
  //  Load the extension-based driver info
  //                    EXT          DESCRIPTION       EXT/PRE   R/W     VECTOR/RASTER/BOTH
  _drivers.emplace_back(".shp",      "ESRI Shapefile", true,     true,   GDAL_OF_VECTOR);
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
  _loadDriverInfo();
}

OgrUtilities::~OgrUtilities()
{
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    GDALDumpOpenDatasets(stderr);
    CPLDumpSharedList(nullptr);
  }
/** In certain instances these clean up functions cause a corrupt doubly-linked-list
  OGRCleanupAll();
  GDALDestroyDriverManager();
*/
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
  for (const auto& driver : _drivers)
  {
    if (readOnly || driver._is_rw)
      formats.insert(QString::fromLatin1(driver._indicator));
  }
  return formats;
}

OgrDriverInfo OgrUtilities::getDriverInfo(const QString& url, bool readonly)
{
  //  Check the already found drivers first
  auto d = _drivers_found.find(url);
  if (d != _drivers_found.end())
    return d->second;
  //  Iterate all drivers to find the correct one
  for (const auto& driver : _drivers)
  {
    if (((driver._is_ext && url.endsWith(driver._indicator)) ||
        (!driver._is_ext && url.startsWith(driver._indicator))) && (readonly || driver._is_rw))
    {
      _drivers_found[url] = driver;
      return driver;
    }
  }
  _drivers_found[url] = OgrDriverInfo();
  return OgrDriverInfo();
}

bool OgrUtilities::isReasonableUrl(const QString& url, bool isRead)
{
  QString relative_url = url;
  //  /vsi* files should have the "/vsi*/" portion of the URL removed before checking the file type
  if (url.startsWith("/vsi"))
    relative_url = url.right(url.length() - url.indexOf("/", 4) - 1);
  if (relative_url.endsWith("/"))
    relative_url = relative_url.left(relative_url.size() - 1);
  //  Check if there is a valid driver for this file type
  return getDriverInfo(relative_url, isRead)._driverName != nullptr;
}

std::shared_ptr<GDALDataset> OgrUtilities::createDataSource(const QString& url)
{
  QString source = url;
  OgrDriverInfo driverInfo = getDriverInfo(url, false);
  if (driverInfo._driverName == nullptr)
    throw HootException("Error getting driver info for: " + url);
  GDALDriver *driver = GetGDALDriverManager()->GetDriverByName(driverInfo._driverName);
  if (driver == nullptr)
    throw HootException("Error getting driver by name: " + QString(driverInfo._driverName));

  OgrOptions options = _getOgrOptions(url, driverInfo);

  // If the user specifies a shapefile, then crop off the .shp and create a directory.
  if (url.endsWith(".shp", Qt::CaseInsensitive))
    source = url.mid(0, url.length() - 4);

  std::shared_ptr<GDALDataset> result(driver->Create(source.toLatin1(), 0, 0, 0, GDT_Unknown, options.getCrypticOptions()));
  if (result == nullptr)
    throw HootException(QString("Unable to create data source: %1 (%2)").arg(source, QString(CPLGetLastErrorMsg())));
  return result;
}

std::shared_ptr<GDALDataset> OgrUtilities::openDataSource(const QString& url, bool readonly)
{
  /* Check for the correct driver name, if unknown try all drivers.
   * This can be an issue because drivers are tried in the order that they are
   * loaded which has been known to cause issues.
   */
  OgrDriverInfo driverInfo = getDriverInfo(url, readonly);

  // With GDALOpenEx, we need to specify the GDAL_OF_UPDATE option or the dataset will get opened
  // Read Only.
  if (!readonly)
    driverInfo._driverType = driverInfo._driverType | GDAL_OF_UPDATE;

  LOG_VART(driverInfo._driverName);
  LOG_VART(driverInfo._driverType);
  LOG_VART(url.toUtf8().data());

  const char* drivers[2] = { driverInfo._driverName, nullptr };

  // Setup read options for various file types
  OgrOptions options = _getOgrOptions(url, driverInfo);

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
      errorMsg += " Is your path within the zip file correct?";
    throw HootException(errorMsg);
  }
  return result;
}

QStringList OgrUtilities::getValidFilesInContainer(const QString& url)
{
  QString path = url;
  QStringList files;
  //  Ignore anything but files with these extensions
  QStringList extensions({".zip", ".tar", ".tar.gz", ".tgz", ".gz"});
  bool found = false;
  for (const auto& ext : qAsConst(extensions))
  {
    if (path.endsWith(ext, Qt::CaseInsensitive))
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
      if (isReasonableUrl(file, true))
        files.append(file);
    }
    //  Destroy the GDAL memory buffer
    CSLDestroy(file_list);
  }
  //  Return the list of valid files, if any
  return files;
}

OgrOptions OgrUtilities::_getOgrOptions(const QString& url, const OgrDriverInfo& driverInfo) const
{
  OgrOptions options;
  //  Turn off writing a properties file for GZIP operations in GDAL
  CPLSetConfigOption("CPL_VSIL_GZIP_WRITE_PROPERTIES", "NO");

  QString driver(driverInfo._driverName);

  if (driver == "CSV")
  {
    options["X_POSSIBLE_NAMES"] = ConfigOptions().getOgrReaderCsvLonfield();
    options["Y_POSSIBLE_NAMES"] = ConfigOptions().getOgrReaderCsvLatfield();
    options["KEEP_GEOM_COLUMNS"] = ConfigOptions().getOgrReaderCsvKeepGeomFields();
  }
  else if (driver == "OGR_OGDI")
  {
    // From GDAL/OGR 1.8.0 on, setting the OGR_OGDI_LAUNDER_LAYER_NAMES configuration option (or
    // environment variable) to YES causes the layer names to be simplified. For example,
    // watrcrsl_hydro instead of 'watrcrsl@hydro(*)_line'.
    options["OGR_OGDI_LAUNDER_LAYER_NAMES"] = ConfigOptions().getOgrReaderOgdiLaunderLayerNames();
  }
  else if (driver == "LIBKML")
  {
    //  Turn off custom indexing for KML output
    CPLSetConfigOption("OSM_USE_CUSTOM_INDEXING", "NO");
  }
  else if (driver == "S57")
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
  else if (driver == "ESRI Shapefile" && !url.startsWith("/vsi"))
  {
    // Restore broken or absent .shx file from associated .shp file during opening when the config
    // option is set (true by default)
    // NOTE: This is a GLOBAL setting, not one for the driver.
    // Also, this setting fights with the /vsi driver.  The vsi driver can not write to the .shx
    // file while it is reading from the zip/tar/etc
    CPLSetConfigOption("SHAPE_RESTORE_SHX", ConfigOptions().getOgrEsriShapeRestoreShx() ? "YES" : "NO");
  }
  else if (driver == "GPKG")
  {
    // Writing to GPKG files can be quite slow, these config options help speed it up.
    CPLSetConfigOption("OGR_SQLITE_CACHE", "512");
    CPLSetConfigOption("OGR_SQLITE_SYNCHRONOUS", "OFF");
    CPLSetConfigOption("SQLITE_USE_OGR_VFS", "YES");
  }
  return options;
}

}
