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
 * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OgrFeatureProvider.h"

// Local Includes
#include "Exception.h"

// GDAL Includes
#include <ogrsf_frmts.h>

// Qt Includes
#include <QDebug>
#include <QString>

using namespace boost;

namespace hoot
{

OgrFeatureProvider::OgrFeatureProvider(OGRLayer* layer)
{
    _dataSource = 0;
    _layer = layer;

    _layer->ResetReading();
    _next.reset(_layer->GetNextFeature());
}

OgrFeatureProvider::~OgrFeatureProvider()
{
    if (_dataSource != 0)
    {
        OGRDataSource::DestroyDataSource(_dataSource);
    }
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
        OGRRegisterAll();
        first = false;
    }

    OGRDataSource* dataSource(OGRSFDriverRegistrar::Open(ds.toAscii(), FALSE));

    QString errorMsg = CPLGetLastErrorMsg();
    qDebug() << errorMsg;
    if (dataSource->GetLayerCount() != 1)
    {
        throw Exception("Invalid layer count.");
    }

    shared_ptr<OgrFeatureProvider> result(
            new OgrFeatureProvider(dataSource->GetLayer(0)));
    result->_dataSource = dataSource;

    return result;
}

}
