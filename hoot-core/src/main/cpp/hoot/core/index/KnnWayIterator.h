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

#ifndef KNNWAYITERATOR_H
#define KNNWAYITERATOR_H

// GEOS
#include <geos/geom/LineString.h>

// tgs
#include <tgs/RStarTree/KnnIterator.h>

// Hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{
  class Way;
  class OsmMapIndex;

  using namespace geos::geom;
  using namespace Tgs;
  using namespace std;

/**
 */
class KnnWayIterator : public KnnIterator
{
public:
  KnnWayIterator(const OsmMap& map, boost::shared_ptr<const Way> way, const RStarTree* tree,
                 const vector<long>& treeIdToWid, bool addError = false);

  long getWayId() const { return _treeIdToWid[getId()]; }

  boost::shared_ptr<const Way> getWay() const { return _map.getWay(getWayId()); }

  virtual bool hasNext();

  int getDistanceCount() { return _distanceCount; }

protected:
  virtual double _calculateDistance(const BoxInternalData& box, int id) const;

  virtual double _calculateDistance(const BoxInternalData& box) const;

private:
  const OsmMap& _map;

  double _indexSlush;
  mutable int _distanceCount;
  bool _addError;
  double _baseAccuracy;
  const vector<long>& _treeIdToWid;

  long _wayId;
  boost::shared_ptr<LineString> _ls;
  const LineString* _lsFast;
};

}

#endif // KNNWAYITERATOR_H
