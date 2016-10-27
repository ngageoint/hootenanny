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
#include "PoiPolygonDistanceMatcher.h"

// Hoot
#include <hoot/core/util/Log.h>

namespace hoot
{

PoiPolygonDistanceMatcher::PoiPolygonDistanceMatcher(double matchDistanceDefault,
                                                                     double reviewDistanceDefault,
                                                                     const Tags& polyTags,
                                                                     long searchRadius) :
_matchDistanceDefault(matchDistanceDefault),
_reviewDistanceDefault(reviewDistanceDefault),
_polyTags(polyTags),
_searchRadius(searchRadius),
_surroundingPolyCount(-1),
_surroundingPoiCount(-1)
{
}

PoiPolygonDistanceMatcher::PoiPolygonDistanceMatcher(double matchDistanceDefault,
                                                                     double reviewDistanceDefault,
                                                                     const Tags& polyTags,
                                                                     long searchRadius,
                                                                     long surroundingPolyCount,
                                                                     long surroundingPoiCount) :
_matchDistanceDefault(matchDistanceDefault),
_reviewDistanceDefault(reviewDistanceDefault),
_polyTags(polyTags),
_searchRadius(searchRadius),
_surroundingPolyCount(surroundingPolyCount),
_surroundingPoiCount(surroundingPoiCount)
{
}

double PoiPolygonDistanceMatcher::getMatchDistanceForType(const QString /*typeKvp*/) const
{
  return _matchDistanceDefault;
}

double PoiPolygonDistanceMatcher::getReviewDistanceForType(const QString typeKvp) const
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

double PoiPolygonDistanceMatcher::_getPolyDensity() const
{
  //area of the search radius circle
  const double searchRadiusArea = M_PI * pow(_searchRadius, 2);
  LOG_VART(searchRadiusArea);
  //polys per square meter
  return _surroundingPolyCount / searchRadiusArea;
}

double PoiPolygonDistanceMatcher::_getPoiDensity() const
{
  //area of the search radius circle
  const double searchRadiusArea = M_PI * pow(_searchRadius, 2);
  LOG_VART(searchRadiusArea);
  //polys per square meter
  return _surroundingPoiCount / searchRadiusArea;
}

void PoiPolygonDistanceMatcher::modifyMatchDistanceForPolyDensity(double& distance)
{
  const double polyDensity = _getPolyDensity();
  LOG_VART(polyDensity);
  //tweak the match distance based on the surrounding poly density; as the density decreases,
  //raise it
  if (polyDensity >= 0.0005 && polyDensity <= 0.001)
  {
    distance += (distance * 0.4);
  }
  else if (polyDensity >= 0.001 && polyDensity <= 0.005)
  {
    distance += (distance * 0.3);
  }
  LOG_VART(distance);
}

void PoiPolygonDistanceMatcher::modifyReviewDistanceForPolyDensity(double& distance)
{
  const double polyDensity = _getPolyDensity();
  //tweak the review distance based on the surrounding poly density; as the density
  //increases, lower it
  LOG_VART(polyDensity);
  if (polyDensity >= 0.005)
  {
    distance -= (distance * 0.3);
  }
  else if (polyDensity >= 0.001 && polyDensity <= 0.005)
  {
    distance -= (distance * 0.2);
  }
  else if (polyDensity >= 0.0005 && polyDensity <= 0.001)
  {
    distance -= (distance * 0.1);
  }
  LOG_VART(distance);
}

}

