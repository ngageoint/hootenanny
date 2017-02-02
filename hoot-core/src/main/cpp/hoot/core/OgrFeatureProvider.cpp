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
#include "OgrFeatureProvider.h"
#include <hoot/core/io/OgrUtilities.h>

// Local Includes
#include "Exception.h"

// GDAL Includes
#include <ogrsf_frmts.h>
#include <gdal_frmts.h>

// Qt Includes
#include <QDebug>
#include <QString>

using namespace boost;

namespace hoot
{

OgrFeatureProvider::OgrFeatureProvider(OGRLayer* layer)
{
  _layer = layer;

  _layer->ResetReading();
  _next.reset(_layer->GetNextFeature());
}

OgrFeatureProvider::~OgrFeatureProvider()
{
  if (_dataSource)
    GDALClose(_dataSource.get());
}

bool OgrFeatureProvider::hasNext()
{
  return _next != 0;
}

shared_ptr<OGRFeature> OgrFeatureProvider::next()
{
  shared_ptr<OGRFeature> result = _next;
  _next.reset(_layer->GetNextFeature());
  return result;
}

shared_ptr<OgrFeatureProvider> OgrFeatureProvider::openDataSource(const QString& ds)
{
  static bool first = true;
  if (first == true)
  {
    GDALAllRegister();
    first = false;
  }

  /* Check for the correct driver name in OgrUtilities, if unknown try all drivers.
   * This can be an issue because drivers are tried in the order that they are
   * loaded which has been known to cause issues.
   */
  OgrDriverInfo driverInfo = OgrUtilities::getInstance().getDriverInfo(ds);
  const char* drivers[2] = { driverInfo._driverName, NULL };
  GDALDataset* dataSource = (GDALDataset*)GDALOpenEx(ds.toAscii(), driverInfo._driverType,
    (driverInfo._driverName != NULL ? drivers : NULL), NULL, NULL);

  QString errorMsg = CPLGetLastErrorMsg();
  qDebug() << errorMsg;
  if (dataSource->GetLayerCount() != 1)
  {
    throw Exception("Invalid layer count.");
  }

  shared_ptr<OgrFeatureProvider> result(
    new OgrFeatureProvider(dataSource->GetLayer(0)));
  result->_dataSource.reset(dataSource);

  return result;
}

}
