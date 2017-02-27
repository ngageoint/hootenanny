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
#include "MatchCreator.h"

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/filters/BuildingCriterion.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/filters/WaterwayCriterion.h>
#include <hoot/core/filters/HighwayFilter.h>

namespace hoot
{

QString MatchCreator::BaseFeatureTypeToString(BaseFeatureType t)
{
  switch (t)
  {
    case POI:
      return "POI";
    case Highway:
      return "Highway";
    case Building:
      return "Building";
    case Waterway:
      return "Waterway";
    case Unknown:
    default:
      return "Unknown";
  }
}

MatchCreator::BaseFeatureType MatchCreator::StringToBaseFeatureType(QString s)
{
  s = s.toLower();
  if (0 == s.compare("poi"))
    return POI;
  else if (0 == s.compare("highway"))
    return Highway;
  else if (0 == s.compare("building"))
    return Building;
  else if (0 == s.compare("waterway"))
    return Waterway;
  else
    return Unknown;
}

MatchCreator::FeatureCalcType MatchCreator::getFeatureCalcType (BaseFeatureType t)
{
  switch (t)
  {
    case POI:
      return CalcTypeNone;
    case Highway:
      return CalcTypeLength;
    case Building:
      return CalcTypeArea;
    case Waterway:
      return CalcTypeLength;
    case Unknown:
    default:
      return CalcTypeNone;
  }
}

ElementCriterion* MatchCreator::getElementCriterion (BaseFeatureType t, ConstOsmMapPtr map)
{
  switch (t)
  {
    case POI:
      return new PoiCriterion();
    case Highway:
      return new HighwayFilter(Filter::KeepMatches);
    case Building:
      return new BuildingCriterion(map);
    case Waterway:
      return new WaterwayCriterion();
    case Unknown:
    default:
      return NULL;
  }
}

}
