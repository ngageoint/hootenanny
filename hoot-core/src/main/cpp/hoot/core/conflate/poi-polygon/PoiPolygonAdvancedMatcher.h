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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONADVANCEDMATCHER_H
#define POIPOLYGONADVANCEDMATCHER_H

// Hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{

/**
 * A custom set of rules that allow for additional match tweaking beyond the additive model used
 * in PoiPolygonMatch.
 *
 * The logic in this class has been stripped down over time for maintainability purposes.
 * Currently, the logic accounts for adding one additional correct match in one test dataset.  Since
 * that number is so small and the logic is runtime expensive, its disabled by default.  Future
 * customization of this class may improve matching to the point that the logic should be
 * re-enabled.
 */
class PoiPolygonAdvancedMatcher
{

public:

  PoiPolygonAdvancedMatcher(const ConstOsmMapPtr& map, const set<ElementId>& polyNeighborIds,
                            const set<ElementId>& poiNeighborIds, double distance);

  bool triggersRule(ConstElementPtr poi, ConstElementPtr poly);

private:

  ConstOsmMapPtr _map;

  set<ElementId> _polyNeighborIds;
  set<ElementId> _poiNeighborIds;

  double _distance;

  QStringList _genericLandUseTagVals;

  int _badGeomCount;

};

}

#endif // POIPOLYGONADVANCEDMATCHER_H