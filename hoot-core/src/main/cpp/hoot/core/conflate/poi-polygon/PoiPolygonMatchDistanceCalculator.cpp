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
#include "PoiPolygonMatchDistanceCalculator.h"

// Hoot
#include <hoot/core/util/Log.h>

namespace hoot
{

PoiPolygonMatchDistanceCalculator::PoiPolygonMatchDistanceCalculator(double matchDistanceDefault,
                                                                     double reviewDistanceDefault,
                                                                     const Tags& polyTags,
                                                                     long searchRadius,
                                                                     long surroundingPolyCount) :
_matchDistanceDefault(matchDistanceDefault),
_reviewDistanceDefault(reviewDistanceDefault),
_polyTags(polyTags),
_searchRadius(searchRadius),
_surroundingPolyCount(surroundingPolyCount)
{

}

double PoiPolygonMatchDistanceCalculator::getMatchDistanceForType(const QString /*typeKvp*/) const
{
  //TODO: haven't actually done anything here yet
  return _matchDistanceDefault;
}

double PoiPolygonMatchDistanceCalculator::getReviewDistanceForType(const QString typeKvp) const
{
  if (typeKvp == "leisure=park")
  {
    return 25.0;
  }
  else if ((typeKvp == "station=light_rail" || typeKvp == "railway=platform") &&
           (_polyTags.get("subway") == "yes" || _polyTags.get("tunnel") == "yes"))
  {
    return 150.0;
  }
  return _reviewDistanceDefault;
}

double PoiPolygonMatchDistanceCalculator::_getPolyDensity() const
{
  //area of the search radius circle
  const double searchRadiusArea = M_PI * pow(_searchRadius, 2);
  LOG_VARD(searchRadiusArea);
  //polys per square meter
  return _surroundingPolyCount / searchRadiusArea;
}

//C densities (roughly): .00014 to .011
//D densities (roughly): .0008 to .01

void PoiPolygonMatchDistanceCalculator::modifyMatchDistanceForPolyDensity(double& distance)
{
  const double polyDensity = _getPolyDensity();
  //tweak the match distance based on the surrounding poly density; ?
  //TODO: finish

  LOG_VARD(polyDensity);
  LOG_VARD(distance);
}

void PoiPolygonMatchDistanceCalculator::modifyReviewDistanceForPolyDensity(double& distance)
{
  const double polyDensity = _getPolyDensity();
  //tweak the review distance based on the surrounding poly density; as the density
  //increases, lower it
  if (polyDensity > 0.01)
  {
    distance -= (distance * 0.8);
  }
  else if (polyDensity >= 0.005 && polyDensity <= 0.01)
  {
    distance -= (distance * 0.7);
  }
  else if (polyDensity >= 0.001 && polyDensity <= 0.005)
  {
    distance -= (distance * 0.6);
  }
  else if (polyDensity >= 0.0005 && polyDensity <= 0.001)
  {
    distance -= (distance * 0.1);
  }

  //TODO: change back
  LOG_VARE(polyDensity);
  LOG_VARD(distance);
}

}

