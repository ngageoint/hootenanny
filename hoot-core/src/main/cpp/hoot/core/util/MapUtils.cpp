#include "MapUtils.h"

// Hoot
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/filters/BuildingCriterion.h>
#include <hoot/core/filters/NonBuildingAreaCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPolyCriterion.h>

namespace hoot
{
bool MapUtils::containsTwoOrMorePois(ConstOsmMapPtr map)
{
  const long poiCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(poiCount);
  return poiCount >= 2;
}

bool MapUtils::containsTwoOrMoreBuildings(ConstOsmMapPtr map)
{
  const long buildingCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new BuildingCriterion(map)),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(buildingCount);
  return buildingCount >= 2;
}

bool MapUtils::containsTwoOrMoreAreas(ConstOsmMapPtr map)
{
  const long areaCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new NonBuildingAreaCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(areaCount);
  return areaCount >= 2;
}

bool MapUtils::containsOnePolygonAndOnePoi(ConstOsmMapPtr map)
{
  const long poiCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiPolygonPoiCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  const long polyCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiPolygonPolyCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(poiCount);
  LOG_VART(polyCount);
  return poiCount == 1 && polyCount == 1;
}

bool MapUtils::containsPolys(ConstOsmMapPtr map)
{
  const long polyCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiPolygonPolyCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(polyCount);
  return polyCount > 0;
}

bool MapUtils::containsAreas(ConstOsmMapPtr map)
{
  const long areaCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new NonBuildingAreaCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(areaCount);
  return areaCount > 0;
}

bool MapUtils::containsBuildings(ConstOsmMapPtr map)
{
  const long buildingCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new BuildingCriterion(map)),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(buildingCount);
  return buildingCount > 0;
}

bool MapUtils::containsPois(ConstOsmMapPtr map)
{
  const long poiCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  return poiCount > 0;
}
}
