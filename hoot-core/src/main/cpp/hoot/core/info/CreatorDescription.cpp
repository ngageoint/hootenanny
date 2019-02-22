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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "CreatorDescription.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/LinearWaterwayCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/conflate/poi-polygon/criterion/PoiPolygonPoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/criterion/PoiPolygonPolyCriterion.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/PowerLineCriterion.h>

namespace hoot
{

CreatorDescription::CreatorDescription() :
experimental()
{
}

CreatorDescription::CreatorDescription(std::string className, QString description,
                                       bool experimental) :
experimental(experimental),
className(className),
description(description)
{
}

CreatorDescription::CreatorDescription(std::string className, QString description,
                                       BaseFeatureType featureType, bool experimental) :
experimental(experimental),
className(className),
description(description),
baseFeatureType(featureType)
{
}

QString CreatorDescription::baseFeatureTypeToString(BaseFeatureType t)
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
    case PoiPolygonPOI:
      return "Polygon Conflatable POI";
    case Polygon:
      return "Polygon";
    case Area:
      return "Area";
    case Railway:
      return "Railway";
    case PowerLine:
      return "Power Line";
    default:
      return "Unknown";
  }
}

CreatorDescription::BaseFeatureType CreatorDescription::stringToBaseFeatureType(QString s)
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
  else if (0 == s.compare("polygon conflatable poi"))
    return PoiPolygonPOI;
  else if (0 == s.compare("polygon"))
    return Polygon;
  else if (0 == s.compare("area"))
    return Area;
  else if (0 == s.compare("railway"))
    return Railway;
  else if (0 == s.compare("powerline"))
    return PowerLine;
  else
    return Unknown;
}

CreatorDescription::FeatureCalcType CreatorDescription::getFeatureCalcType (BaseFeatureType t)
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
    case PoiPolygonPOI:
      return CalcTypeNone;
    case Polygon:
      return CalcTypeArea;
    case Area:
      return CalcTypeArea;
    case Railway:
      return CalcTypeLength;
    case PowerLine:
      return CalcTypeLength;
    default:
      return CalcTypeNone;
  }
}

ElementCriterionPtr CreatorDescription::getElementCriterion(BaseFeatureType t, ConstOsmMapPtr map)
{
  switch (t)
  {
    case POI:
      return ElementCriterionPtr(new PoiCriterion());
    case Highway:
      return ElementCriterionPtr(new HighwayCriterion());
    case Building:
      return ElementCriterionPtr(new BuildingCriterion(map));
    case Waterway:
      return ElementCriterionPtr(new LinearWaterwayCriterion());
    case PoiPolygonPOI:
      return ElementCriterionPtr(new PoiPolygonPoiCriterion());
    case Polygon:
      return ElementCriterionPtr(new PoiPolygonPolyCriterion());
    case Area:
      return ElementCriterionPtr(new NonBuildingAreaCriterion());
    case Railway:
      return ElementCriterionPtr(new RailwayCriterion());
    case PowerLine:
      return ElementCriterionPtr(new PowerLineCriterion());
    default:
      return ElementCriterionPtr();
  }
}

QString CreatorDescription::toString() const
{
  return QString::fromStdString(className) + ";" + baseFeatureTypeToString(baseFeatureType);
}

}
