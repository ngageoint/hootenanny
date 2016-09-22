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
#include "PoiPolygonParkRuleApplier.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/conflate/MatchClassification.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
//#include <hoot/core/conflate/polygon/extractors/EdgeDistanceExtractor.h>

#include "PoiPolygonScorer.h"

// std
#include <float.h>

namespace hoot
{

PoiPolygonParkRuleApplier::PoiPolygonParkRuleApplier(const ConstOsmMapPtr& map,
                                                     const set<ElementId>& areaNeighborIds,
                                                     const set<ElementId>& poiNeighborIds,
                                                     double distance, double nameScoreThreshold,
                                                     bool exactNameMatch,
                                                     const QString testUuid = "") :
_map(map),
_areaNeighborIds(areaNeighborIds),
_poiNeighborIds(poiNeighborIds),
_distance(distance),
_nameScoreThreshold(nameScoreThreshold),
_exactNameMatch(exactNameMatch),
_testUuid(testUuid)
{
}

bool PoiPolygonParkRuleApplier::applyRules(ConstElementPtr poi, ConstElementPtr poly,
                                           MatchClassification& matchClass)
{
  bool triggersParkRule = false;

  shared_ptr<Geometry> polyGeom = ElementConverter(_map).convertToGeometry(poly);
  shared_ptr<Geometry> poiGeom = ElementConverter(_map).convertToGeometry(poi);

  const QString poiName = poi->getTags().get("name").toLower();
  //const QString polyName = poly->getTags().get("name").toLower();

  const bool poiHasType =
    OsmSchema::getInstance().getCategories(poi->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  const bool poiIsPark = _elementIsPark(poi);
  const bool poiIsParkish = _elementIsParkish(poi);
  const bool poiIsPlayArea = _elementIsPlayArea(poi);
  const bool poiIsPlayground = _elementIsPlayground(poi);
  const bool poiIsRecCenter = _elementIsRecCenter(poi);
  const bool poiIsSport = _elementIsSport(poi);

  const bool polyHasType =
    OsmSchema::getInstance().getCategories(poly->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  const bool polyIsPark = _elementIsPark(poly);
  const bool polyIsPlayground = _elementIsPlayground(poly);
  const bool polyIsBuilding = OsmSchema::getInstance().isBuilding(poly);
  const bool polyIsRecCenter = _elementIsRecCenter(poly);

  bool polyVeryCloseToAnotherParkPoly = false;
  double parkPolyAngleHistVal = -1.0;
  double parkPolyOverlapVal = -1.0;
  bool otherParkPolyNameMatch = false;
  double otherParkPolyNameScore = -1.0;
  double poiToPolyNodeDist = DBL_MAX;
  double poiToOtherParkPolyNodeDist = DBL_MAX;
  bool otherParkPolyHasName = false;
  bool polyContainsPlayAreaOrPlaygroundPoly = false;
  bool poiContainedInAnotherParkPoly = false;
  bool polyContainsAnotherParkOrPlaygroundPoi = false;
  bool containedOtherParkOrPlaygroundPoiHasName = false;
  bool sportPoiOnOtherSportPolyWithExactTypeMatch = false;

  const double polyArea = polyGeom->getArea();

  PoiPolygonScorer scorer(_testUuid);

  set<ElementId>::const_iterator poiNeighborItr = _poiNeighborIds.begin();
  while (poiNeighborItr != _poiNeighborIds.end())
  {
    ConstElementPtr poiNeighbor = _map->getElement(*poiNeighborItr);
    if (poiNeighbor->getElementId() != poi->getElementId())
    {
      shared_ptr<Geometry> poiNeighborGeom = ElementConverter(_map).convertToGeometry(poiNeighbor);

      /*if (Log::getInstance().getLevel() == Log::Debug &&
          (poi->getTags().get("uuid") == _testUuid ||
           poly->getTags().get("uuid") == _testUuid))
      {
        LOG_VARD(poiNeighbor->getTags().get("uuid"))
        LOG_VARD(_elementIsPark(poiNeighbor));
        LOG_VARD(_elementIsPlayground(poiNeighbor));
        LOG_VARD(_elementIsPlayArea(poiNeighbor))
        LOG_VARD(polyGeom->contains(poiNeighborGeom.get()));
      }*/

      if ((_elementIsPark(poiNeighbor) || _elementIsPlayground(poiNeighbor)) &&
          !_elementIsPlayArea(poiNeighbor) && polyGeom->contains(poiNeighborGeom.get()))
      {
        polyContainsAnotherParkOrPlaygroundPoi = true;
        if (!containedOtherParkOrPlaygroundPoiHasName)
        {
          containedOtherParkOrPlaygroundPoiHasName =
            !poiNeighbor->getTags().get("name").trimmed().isEmpty();
        }

        if (Log::getInstance().getLevel() == Log::Debug &&
            (poly->getTags().get("uuid") == _testUuid ||
             poi->getTags().get("uuid") == _testUuid))
        {
          LOG_DEBUG(
            "poly examined and found to contain another park or playground poi " <<
            poly->toString());
          LOG_DEBUG("park/playground poi it is very close to: " << poiNeighbor->toString());
        }
      }
    }
    poiNeighborItr++;
  }

  set<ElementId>::const_iterator areaNeighborItr = _areaNeighborIds.begin();
  while (areaNeighborItr != _areaNeighborIds.end())
  {
    ConstElementPtr area = _map->getElement(*areaNeighborItr);
    if (area->getElementId() != poly->getElementId())
    {
      shared_ptr<Geometry> areaGeom = ElementConverter(_map).convertToGeometry(area);

      if (QString::fromStdString(areaGeom->toString()).toUpper().contains("EMPTY"))
      {
        //if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
        //{
          LOG_WARN(
            "Invalid area neighbor polygon passed to PoiPolygonMatchCreator: " <<
            areaGeom->toString());
          //_badGeomCount++;
        //}
      }
      else
      {
        if (Log::getInstance().getLevel() == Log::Debug &&
            (poi->getTags().get("uuid") == _testUuid ||
             poly->getTags().get("uuid") == _testUuid))
        {
          LOG_VARD(area->getTags().get("uuid"))
          LOG_VARD(_elementIsPlayground(area));
          LOG_VARD(_elementIsPlayArea(area));
          LOG_VARD(polyGeom->contains(areaGeom.get()));
          LOG_VARD(_elementIsSport(area));
          LOG_VARD(poiIsSport);
          LOG_VARD(areaGeom->contains(poiGeom.get()));
          LOG_VARD(scorer.exactTypeMatch(poi, area));
          QString t1, t2;
          LOG_VARD(scorer.getTypeScore(poi, area, t1, t2));
          LOG_VARD(t1);
          LOG_VARD(t2);
        }

        if (_elementIsPark(area))
        {
          otherParkPolyHasName = !area->getTags().get("name").trimmed().isEmpty();
          otherParkPolyNameScore = scorer.getNameScore(poi, area);
          otherParkPolyNameMatch = otherParkPolyNameScore >= _nameScoreThreshold;

          if (areaGeom->contains(poiGeom.get()))
          {
            poiContainedInAnotherParkPoly = true;

            if (Log::getInstance().getLevel() == Log::Debug &&
                (poly->getTags().get("uuid") == _testUuid ||
                 poi->getTags().get("uuid") == _testUuid))
            {
              LOG_DEBUG(
                "poi examined and found to be contained within a park poly outside of this match " <<
                "comparison: " << poi->toString());
              LOG_DEBUG("park poly it is very close to: " << area->toString());
            }
          }

          if (areaGeom->intersects(polyGeom.get()))
          {
            parkPolyAngleHistVal = AngleHistogramExtractor().extract(*_map, area, poly);
            parkPolyOverlapVal = OverlapExtractor().extract(*_map, area, poly);

            //When just using intersection as the criteria, only found one instance when something
            //was considered as "very close" to a park poly when I didn't want it to be...so these
            //values set very low to weed that instance out...overlap at least.
            //TODO: not sure angle hist is actually doing much here.  Maybe I need to pull the
            //area to area search dist down even more...or bring back in edge dist?
            if (parkPolyAngleHistVal >= 0.05 && parkPolyOverlapVal >= 0.02)
            {
              polyVeryCloseToAnotherParkPoly = true;

              if (poly->getElementType() == ElementType::Way &&
                  area->getElementType() == ElementType::Way)
              {
                //Calc the distance from the poi to the poly line instead of the poly itself.
                //Calcing distance to the poly itself will always return 0 when the poi is in the
                //poly.
                ConstWayPtr polyWay = dynamic_pointer_cast<const Way>(poly);
                shared_ptr<const LineString> polyLineStr =
                  dynamic_pointer_cast<const LineString>(
                    ElementConverter(_map).convertToLineString(polyWay));
                poiToPolyNodeDist = polyLineStr->distance(poiGeom.get());
                ConstWayPtr areaWay = dynamic_pointer_cast<const Way>(area);;
                shared_ptr<const LineString> areaLineStr =
                  dynamic_pointer_cast<const LineString>(
                    ElementConverter(_map).convertToLineString(areaWay));
                poiToOtherParkPolyNodeDist = areaLineStr->distance(poiGeom.get());
              }

              if (Log::getInstance().getLevel() == Log::Debug &&
                  (poi->getTags().get("uuid") == _testUuid ||
                   poly->getTags().get("uuid") == _testUuid))
              {
                LOG_DEBUG(
                  "poly examined and found very close to a another park poly: " << poly->toString());
                LOG_DEBUG("park poly it is very close to: " << area->toString());
              }
            }
          }
        }
        else if ((_elementIsPlayground(area) || _elementIsPlayArea(area)) &&
                 polyGeom->contains(areaGeom.get()))
        {
          polyContainsPlayAreaOrPlaygroundPoly = true;

          if (Log::getInstance().getLevel() == Log::Debug &&
              (poi->getTags().get("uuid") == _testUuid ||
               poly->getTags().get("uuid") == _testUuid))
          {
            LOG_DEBUG("poly examined and found to contain a playground poly: " << poly->toString());
            LOG_DEBUG("playground poly it contains: " << area->toString());
          }
        }
        else if (poiIsSport)
        {
          /*if (_elementIsPark(area) && areaGeom->contains(poiGeom.get()))
          {
            sportPoiInPark = true;
          }*/

          //this is a little lose, b/c there could be more than one type match set of tags...
          if (_elementIsSport(area) && areaGeom->contains(poiGeom.get()) &&
              scorer.exactTypeMatch(poi, area))
          {
            sportPoiOnOtherSportPolyWithExactTypeMatch = true;
          }
        }
      }
    }
    areaNeighborItr++;
  }

  const bool poiContainedInParkPoly =
    poiContainedInAnotherParkPoly || (polyIsPark && _distance == 0);

  //If the POI is inside a poly that is very close to another park poly, declare miss if
  //the distance to the outer ring of the other park poly is shorter than the distance to the outer
  //ring of this poly and the other park poly has a name.
  if ((poiIsPark || poiIsParkish) && polyVeryCloseToAnotherParkPoly && _distance == 0 &&
      poiToOtherParkPolyNodeDist < poiToPolyNodeDist && poiToPolyNodeDist != DBL_MAX &&
      poiToOtherParkPolyNodeDist != DBL_MAX && otherParkPolyHasName)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #1...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //If the poi is not a park and being compared to a park polygon or a polygon that is "very close"
  //to another park poly, we want to be more restrictive on type matching, but only if the poi has
  //any type at all.  If the poi has no type, then behave as normal.  Also, let an exact name match
  //cause a review here, rather than a miss.
  else if ((polyIsPark || (polyVeryCloseToAnotherParkPoly && !polyHasType)) && !poiIsPark &&
           !poiIsParkish && poiHasType)
  {
    if (_exactNameMatch)
    {
      if (Log::getInstance().getLevel() == Log::Debug &&
          (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
      {
        LOG_DEBUG("Returning review per park rule #2...");
      }
      matchClass.setReview();
    }
    else
    {
      if (Log::getInstance().getLevel() == Log::Debug &&
          (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
      {
        LOG_DEBUG("Returning miss per park rule #2...");
      }
      matchClass.setMiss();
    }
    triggersParkRule = true;
  }
  //If the poi is a park, the poly it is being compared to is not a park or building, and that poly
  //is "very close" to another park poly that has a name match with the poi, then declare a miss
  //(exclude poly playgrounds from this).
  else if (poiIsPark && !polyIsPark && !polyIsBuilding && polyVeryCloseToAnotherParkPoly &&
           otherParkPolyNameMatch && !polyIsPlayground)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #3...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //Play areas areas can get matched to the larger parks they reside within.  We're setting a max
  //size for play area polys here to prevent that.  Unfortunately, most play areas are tagged as
  //just parks and not specifically as play areas, if they are tagged at all, so we're just scanning
  //the name tag here to determine if something is a play area vs actually verifying that face by
  //looking at its type.
  else if (poiName.contains("play area") && polyArea > 25000) //TODO: move this value to a config?
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #4...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //If the poi is a rec center and the poly is not a rec center or a building, return a miss.
  else if (poiIsRecCenter && !polyIsRecCenter && !polyIsBuilding)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #5...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //Commonly, parks will contain play areas along with other entities (basketball courts, etc.).
  //The play area is considered to be a subpart of the park polygon.  Sometimes parks are named with
  //"playground", which makes this confusing.  Here, we're differentiating between playgrounds,
  //parks, and play areas and attempting to match play area poi's to play area or playground polys.
  //We're not, however, trying to match playground poi's to play area or playground poly, b/c
  //sometimes those need to be matched to the surrounding park polys...that situation should
  //at the very least end up as a review.
  else if (poiIsPlayArea && polyIsPark && polyContainsPlayAreaOrPlaygroundPoly)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #6...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //If a park poi is contained within one park poly, then there's no reason for it to trigger
  //reviews in another one that its not contained in.
  else if (poiIsPark && polyIsPark && _distance > 0 && poiContainedInAnotherParkPoly)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #7...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //If this isn't a park or playground poi, then don't match it to any park poly that contains
  //another park or playground poi.
  else if (poiIsPlayArea && !poiIsPlayground && polyIsPark && _distance == 0 &&
           polyContainsAnotherParkOrPlaygroundPoi && containedOtherParkOrPlaygroundPoiHasName)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #8...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //This is a simple rule to prevent matching poi's not at all like a park with park polys.
  //TODO: this may render some of the previous rules obsolete.
  else if (!poiIsPark && !poiIsParkish && poiHasType && polyIsPark)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #9...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //Don't match sports fields to parks.  This seems a little simplistic, but has has positive
  //results so far.
  //TODO: Is this rule superceded by #11?
  else if (poi->getTags().contains("sport") && polyIsPark && _distance == 0)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #10...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }
  //If a sport poi is contained within an exact type match sport poi poly, don't let it be
  //matched against anything else.
  else if (poiIsSport && /*poiContainedInAnotherParkPoly*/poiContainedInParkPoly &&
           sportPoiOnOtherSportPolyWithExactTypeMatch)
  {
    if (Log::getInstance().getLevel() == Log::Debug &&
        (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
    {
      LOG_DEBUG("Returning miss per park rule #11...");
    }
    matchClass.setMiss();
    triggersParkRule = true;
  }

  if (Log::getInstance().getLevel() == Log::Debug &&
      (poi->getTags().get("uuid") == _testUuid || poly->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(poiHasType);
    LOG_VARD(polyHasType);
    LOG_VARD(polyIsPark);
    LOG_VARD(poiIsParkish);
    LOG_VARD(poiIsPark);
    LOG_VARD(poiIsPlayground);
    LOG_VARD(polyIsPlayground);
    LOG_VARD(polyIsBuilding);
    LOG_VARD(polyVeryCloseToAnotherParkPoly);
    LOG_VARD(parkPolyAngleHistVal);
    LOG_VARD(parkPolyOverlapVal);
    LOG_VARD(otherParkPolyNameMatch);
    LOG_VARD(otherParkPolyNameScore);
    LOG_VARD(poiToPolyNodeDist);
    LOG_VARD(poiToOtherParkPolyNodeDist);
    LOG_VARD(otherParkPolyHasName);
    LOG_VARD(polyArea);
    LOG_VARD(poiIsRecCenter);
    LOG_VARD(polyIsRecCenter);
    LOG_VARD(polyContainsPlayAreaOrPlaygroundPoly);
    LOG_VARD(poiIsPlayArea);
    LOG_VARD(poiContainedInAnotherParkPoly);
    LOG_VARD(polyContainsAnotherParkOrPlaygroundPoi);
    LOG_VARD(containedOtherParkOrPlaygroundPoiHasName);
    LOG_VARD(sportPoiOnOtherSportPolyWithExactTypeMatch);
    LOG_VARD(poiIsSport);
    LOG_VARD(poiContainedInParkPoly);
  }

  return triggersParkRule;
}

//TODO: make all this park name logic be translated

bool PoiPolygonParkRuleApplier::_elementIsRecCenter(ConstElementPtr element) const
{
  const QString elementName = element->getTags().get("name").toLower();
  return elementName.contains("recreation center") || elementName.contains("rec center");
}

bool PoiPolygonParkRuleApplier::_elementIsPlayground(ConstElementPtr element) const
{
  const Tags& tags = element->getTags();
  const QString elementName = tags.get("name").toLower();
  return
    tags.get("leisure") == "playground" || /*elementName.contains("play area") ||*/
    elementName.contains("playground");
}

bool PoiPolygonParkRuleApplier::_elementIsPlayArea(ConstElementPtr element) const
{
  return element->getTags().get("name").toLower().contains("play area");
}

bool PoiPolygonParkRuleApplier::_elementIsPark(ConstElementPtr element) const
{
  return !OsmSchema::getInstance().isBuilding(element) &&
         (element->getTags().get("leisure") == "park" /*||
          _containsPartial("park", element->getTags().getNames())*/);
}

//TODO: can hopefully eventually genericize this out to something like leisure=*park*
bool PoiPolygonParkRuleApplier::_elementIsParkish(ConstElementPtr element) const
{
  if (OsmSchema::getInstance().isBuilding(element))
  {
    return false;
  }
  const QString leisureVal = element->getTags().get("leisure").toLower();
  return
    leisureVal == "garden" || /*element->getTags().get("sport") == "tennis" ||*/
    leisureVal == "dog_park";
}

bool PoiPolygonParkRuleApplier::_elementIsSport(ConstElementPtr element) const
{
  return element->getTags().contains("sport");
}

}
