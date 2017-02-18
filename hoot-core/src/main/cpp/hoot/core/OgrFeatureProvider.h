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
#ifndef OGRFEATUREPROVIDER_H
#define OGRFEATUREPROVIDER_H

#include "FeatureProvider.h"

// GDAL Includes
class GDALDataset;
class OGRLayer;

// Qt Includes
class QString;

namespace hoot
{

class OgrFeatureProvider : public FeatureProvider
{
public:
    /**
     * Creates an OgrFeatureProvider for the given layer. The OgrFeature provider takes owndership
     * of the layer.
     */
    OgrFeatureProvider(OGRLayer* layer);

    virtual ~OgrFeatureProvider();

    virtual bool hasNext();

    virtual boost::shared_ptr<OGRFeature> next();

    static boost::shared_ptr<OgrFeatureProvider> openDataSource(const QString& ds);

protected:

    OGRLayer* _layer;

    boost::shared_ptr<OGRFeature> _next;

    boost::shared_ptr<GDALDataset> _dataSource;
};

} // hoot

#endif // OGRFEATUREPROVIDER_H
