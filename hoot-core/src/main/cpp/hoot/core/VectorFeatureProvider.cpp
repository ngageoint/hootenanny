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
#include "VectorFeatureProvider.h"

// GDAL Includes
#include <ogrsf_frmts.h>

// Qt Includes
#include <QString>

using namespace boost;

VectorFeatureProvider::VectorFeatureProvider(
        const std::vector< boost::shared_ptr<OGRFeature> >& v) :
        _v(v)
{
    _index = 0;
}

bool VectorFeatureProvider::hasNext()
{
    return _index != _v.size();
}

shared_ptr<OGRFeature> VectorFeatureProvider::next()
{
    return _v[_index++];
}
