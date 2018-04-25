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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONREVIEWREDUCER_H
#define POIPOLYGONREVIEWREDUCER_H

// Hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{

/**
 * Class intended to reduce the number of unnecessary reviews between POI's and polygons with the
 * goal of never causing a miss where there should be a match.  Any rule that results in an
 * incorrect match found over the course of time testing against different datasets should be
 * removed from this class.
 *
 * I would like to get rid of most, if not all, of this class, as it is kind of brittle.  It may be
 * possible via modifications to the definition of what poi/poly conflation conflates, as well as
 * modifications to the evidence calculation in PoiPolygonMatch.
 */
class PoiPolygonReviewReducer
{

public:

  //TODO: encapsulate all these params in a class
  PoiPolygonReviewReducer(const ConstOsmMapPtr& map, const std::set<ElementId>& polyNeighborIds,
                          const std::set<ElementId>& poiNeighborIds, double distance,
                          double nameScoreThreshold, double nameScore, bool nameMatch,
                          bool exactNameMatch, double typeScoreThreshold, double typeScore,
                          bool typeMatch, double matchDistanceThreshold, bool addressMatch);

  bool triggersRule(ConstElementPtr poi, ConstElementPtr poly);

private:

  ConstOsmMapPtr _map;

  std::set<ElementId> _polyNeighborIds;
  std::set<ElementId> _poiNeighborIds;

  double _distance;
  double _nameScoreThreshold;
  double _nameScore;
  bool _nameMatch;
  bool _exactNameMatch;
  double _typeScoreThreshold;
  double _typeScore;
  bool _typeMatch;
  double _matchDistanceThreshold;
  bool _addressMatch;

  QStringList _genericLandUseTagVals;

  unsigned int _badGeomCount;

  bool _keepClosestMatchesOnly;

  bool _nonDistanceSimilaritiesPresent() const;
};

}

#endif // POIPOLYGONREVIEWREDUCER_H
