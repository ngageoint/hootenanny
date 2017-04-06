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

#include "KnnWayIterator.h"

// GEOS
#include <geos/geom/Envelope.h>
#include <geos/geom/GeometryFactory.h>

// hoot
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Log.h>

// Standard
#include <limits>

namespace hoot
{

KnnWayIterator::KnnWayIterator(const OsmMap& map, boost::shared_ptr<const Way> way,
  const RStarTree* tree, const vector<long>& treeIdToWid, bool addError) :
  KnnIterator(tree, 0.0, 0.0, Box()),
  _map(map),
  _treeIdToWid(treeIdToWid)
{
  _wayId = way->getId();
  _ls = ElementConverter(map.shared_from_this()).convertToLineString(way);
  _lsFast = _ls.get();
  _indexSlush = _map.getIndex().getIndexSlush();
  _distanceCount = 0;
  _addError = addError;
  _baseAccuracy = way->getCircularError();
}

double KnnWayIterator::_calculateDistance(const BoxInternalData&, int id)
  const
{
  // if the id in the index isn't valid, then report the maximum possible distance.
  double result = numeric_limits<double>::max();

  long otherWayId = _treeIdToWid[id];
  if (otherWayId == _wayId)
  {
    result = 0.0;
  }
  // if this is a valid way id.
  else if (_map.containsWay(otherWayId))
  {
    ConstWayPtr w = _map.getWay(otherWayId);

    // grab the geometry for the way that we're comparing against.
    boost::shared_ptr<LineString> ls = ElementConverter(_map.shared_from_this()).convertToLineString(w);

    Meters d = ls->distance(_lsFast);

    if (_addError)
    {
      Meters acc = w->getCircularError();

      d = std::max(0.0, ls->distance(_lsFast) - (acc + _baseAccuracy));
    }

    _distanceCount++;

    result = d * d;
  }

  return result;
}

double KnnWayIterator::_calculateDistance(const BoxInternalData& box)
  const
{
  Envelope e(box.getLowerBound(0), box.getUpperBound(0),
             box.getLowerBound(1), box.getUpperBound(1));

  Geometry* g = GeometryFactory::getDefaultInstance()->toGeometry(&e);

  // be optimistic with the distance. This accounts for the fact that some ways may move around
  // without updating the index.
  double d = std::max(0.0, _lsFast->distance(g) - _indexSlush);

  delete g;

  return d * d;
}

bool KnnWayIterator::hasNext()
{
  _calculateNextNn();
  // if the ID signifies the last id, or the returned id isn't valid, we're at the end.
  return (getId() != RESERVED_ID) && _map.containsWay(getWayId());
}

}
