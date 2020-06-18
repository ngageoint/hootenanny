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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef KNNWAYITERATOR_H
#define KNNWAYITERATOR_H

// GEOS
#include <geos/geom/LineString.h>

// tgs
#include <tgs/RStarTree/KnnIterator.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class Way;

/**
 */
class KnnWayIterator : public Tgs::KnnIterator
{
public:

  KnnWayIterator(const OsmMap& map, ConstWayPtr way, const Tgs::RStarTree* tree,
                 const std::vector<long>& treeIdToWid, bool addError = false);
  virtual ~KnnWayIterator() = default;

  long getWayId() const { return _treeIdToWid[getId()]; }

  ConstWayPtr getWay() const { return _map.getWay(getWayId()); }

  virtual bool hasNext();

  int getDistanceCount() { return _distanceCount; }

protected:

  virtual double _calculateDistance(const Tgs::BoxInternalData& box, int id) const;

  virtual double _calculateDistance(const Tgs::BoxInternalData& box) const;

private:

  const OsmMap& _map;

  double _indexSlush;
  mutable int _distanceCount;
  bool _addError;
  double _baseAccuracy;
  const std::vector<long>& _treeIdToWid;

  long _wayId;
  std::shared_ptr<geos::geom::LineString> _ls;
  const geos::geom::LineString* _lsFast;
};

}

#endif // KNNWAYITERATOR_H
