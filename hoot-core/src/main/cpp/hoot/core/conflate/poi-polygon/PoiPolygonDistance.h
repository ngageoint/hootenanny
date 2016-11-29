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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONDISTANCE_H
#define POIPOLYGONDISTANCE_H

// Hoot
#include <hoot/core/elements/Element.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Various utilities related to distance used when gather poi/polygon match evidence.
 */
class PoiPolygonDistance
{

public:

  PoiPolygonDistance(double matchDistanceThresholdDefault, double reviewDistanceThresholdDefault,
                     const Tags& polyTags, long searchRadius);

  PoiPolygonDistance(double matchDistanceThresholdDefault, double reviewDistanceThresholdDefault,
                     const Tags& polyTags, long searchRadius, long surroundingPolyCount,
                     long surroundingPoiCount);

  /**
   * Returns a custom match distance for the given type
   *
   * @param tags the tags containing the type to return a custom distance for
   * @return the match distance for the given type
   */
  double getMatchDistanceForType(const Tags& tags) const;

  /**
   * Returns a custom review distance for the given type
   *
   * @param tags the tags containing the type to return a custom distance for
   * @return the review distance for the given type
   */
  double getReviewDistanceForType(const Tags& tags) const;

  //The density based match/review distance modifications proved to have no positive effect for
  //the data tested.  The logic is being left intact in case we find a use for the technique with
  //future datasets.

  /**
   * Adjusts the match distance based on the polygon density in the surrounding search radius area
   *
   * @param distance the current match distance
   */
  void modifyMatchDistanceForPolyDensity(double& distance);

  /**
   * Adjusts the review distance based on the polygon density in the surrounding search radius area
   *
   * @param distance the current review distance
   */
  void modifyReviewDistanceForPolyDensity(double& distance);

private:

  double _matchDistanceThresholdDefault;
  double _reviewDistanceThresholdDefault;
  Tags _polyTags;
  long _searchRadius;
  long _surroundingPolyCount;
  long _surroundingPoiCount;

  double _getPolyDensity() const;
  double _getPoiDensity() const;

};

}

#endif // POIPOLYGONDISTANCE_H
