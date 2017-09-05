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
#ifndef OGRUTILITIES_H
#define OGRUTILITIES_H

// GDAL
#include <gdal.h>
// Forward declaration
class GDALDataset;

// Qt
#include <QString>

// Tgs
#include <tgs/SharedPtr.h>

#include <vector>

namespace hoot
{

class OgrDriverInfo
{
public:
  /**
   * @brief OgrDriverInfo Builds an object to hold driver information for loading GDAL
   * @param indicator Text prefix (i.e. PG: for PostgreSQL) or file extension
   *        (i.e. .shp for ESRI Shapefile)
   * @param driverName Text name of the driver
   * @param is_ext Value is true if the indcator is a file extension, false for prefix
   * @param is_rw Value is true if the driver is able to read and write, false for readonly
   * @param driverType GDAL_OF_VECTOR or GDAL_OF_ALL open flags
   */
  OgrDriverInfo(const char* indicator = NULL, const char* driverName = NULL, bool is_ext = false,
                bool is_rw = true, unsigned int driverType = GDAL_OF_ALL)
   : _indicator(indicator), _driverName(driverName), _is_ext(is_ext), _is_rw(is_rw), _driverType(driverType)
  {}
  const char* _indicator;
  const char* _driverName;
  bool _is_ext;
  bool _is_rw;
  unsigned int _driverType;
};

class OgrUtilities
{
public:

  virtual ~OgrUtilities();

  static OgrUtilities& getInstance();

  /**
   * Returns true if this is likely a data source OGR can open. This will just do a quick check
   * and doesn't verify that the source exists or is a proper format.
   */
  bool isReasonableUrl(const QString& url);

  /**
   * @brief createDataSource - Create an OGR datasource from the url to write to
   * @param url - Location of the datasource to create, pathname or API URL
   * @return pointer to the datasource created
   */
  boost::shared_ptr<GDALDataset> createDataSource(const QString& url);

  /**
   * @brief openDataSource - Open an OGR datasource from the url
   * @param url - Location of the datasource to open, pathname or API URL
   * @param readonly - Indicate if the datasource is read/write or read-only
   * @return pointer to the datasource opened
   */
  boost::shared_ptr<GDALDataset> openDataSource(const QString& url, bool readonly);

  /**
   * @brief getDriverInfo - Select the GDAL driver to use to open/create the datasource
   * @param url - Location of the datasource to open/create, pathname or API URL
   * @param readonly - Indicate if the datasource is read/write or read-only
   * @return OGR driver information based on the URL and read-only flag
   */
  OgrDriverInfo getDriverInfo(const QString& url, bool readonly);

private:
  /**
   * Use getInstance() instead of the constructor.
   */
  OgrUtilities();

  /**
   * @brief loadDriverInfo Loads a hard-coded set of GDAL driver information with file
   *    extensions, prefixes, and open flags used by getDriverInfo() function
   */
  void loadDriverInfo();

  static boost::shared_ptr<OgrUtilities> _theInstance;
  std::vector<OgrDriverInfo> _drivers;
};

}

#endif // OGRUTILITIES_H
