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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonReviewReducer.h"

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/MultiUseCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonNameScoreExtractor.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/algorithms/extractors/OverlapExtractor.h>
#include <hoot/core/criterion/BuildingWayNodeCriterion.h>

// Std
#include <float.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

PoiPolygonReviewReducer::PoiPolygonReviewReducer(
  const ConstOsmMapPtr& map,
  const std::set<ElementId>& polyNeighborIds, double distance, double nameScoreThreshold,
  double nameScore, bool nameMatch, bool exactNameMatch, double typeScoreThreshold,
  double typeScore, bool typeMatch, double matchDistanceThreshold, bool addressMatch,
  bool addressParsingEnabled, PoiPolygonCachePtr infoCache) :
_map(map),
_polyNeighborIds(polyNeighborIds),
_distance(distance),
_nameScoreThreshold(nameScoreThreshold),
_nameScore(nameScore),
_nameMatch(nameMatch),
_exactNameMatch(exactNameMatch),
_typeScoreThreshold(typeScoreThreshold),
_typeScore(typeScore),
_typeMatch(typeMatch),
_matchDistanceThreshold(matchDistanceThreshold),
_addressMatch(addressMatch),
_addressParsingEnabled(addressParsingEnabled),
_infoCache(infoCache)
{
  LOG_VART(_polyNeighborIds.size());
  LOG_VART(_distance);
  LOG_VART(_nameScoreThreshold);
  LOG_VART(_nameMatch);
  LOG_VART(_typeScore);
  LOG_VART(_exactNameMatch);
  LOG_VART(_typeScoreThreshold);
  LOG_VART(_typeMatch);
  LOG_VART(_matchDistanceThreshold);
  LOG_VART(_addressMatch);
  LOG_VART(_addressParsingEnabled);

  LOG_VART(_infoCache.get());
}

void PoiPolygonReviewReducer::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);
}

bool PoiPolygonReviewReducer::_hasAddress(ConstElementPtr element)
{
  return _addressParser.numAddressesRecursive(element, *_map) > 0;
}

bool PoiPolygonReviewReducer::_nonDistanceSimilaritiesPresent() const
{
  return _typeScore > 0.03 || _nameScore > 0.35 || _addressMatch;
}

bool PoiPolygonReviewReducer::_polyContainsPoiAsMember(ConstWayPtr poly,
                                                       ConstElementPtr poi) const
{
  return poly && poly->containsNodeId(poi->getId());
}

bool PoiPolygonReviewReducer::_polyContainsPoiAsMember(ConstRelationPtr poly,
                                                       ConstElementPtr poi) const
{
  return poly && poly->contains(poi->getElementId());
}

bool PoiPolygonReviewReducer::_inCategory(ConstElementPtr element,
                                          const OsmSchemaCategory& category)
{
  return OsmSchema::getInstance().getCategories(element->getTags()).intersects(category);
}

bool PoiPolygonReviewReducer::triggersRule(ConstNodePtr poi, ConstElementPtr poly)
{
  LOG_TRACE("Checking review reduction rules...");
  _triggeredRuleDescription = "";
  QElapsedTimer timer;
  //const int timingThreshold = 10;

  //timer.restart();

  ConstWayPtr polyAsWay;
  ConstRelationPtr polyAsRelation;
  if (poly->getElementType() == ElementType::Way)
  {
    polyAsWay = std::dynamic_pointer_cast<const Way>(poly);
  }
  else if (poly->getElementType() == ElementType::Relation)
  {
    polyAsRelation = std::dynamic_pointer_cast<const Relation>(poly);
  }

  // Some of these rules could be obsolete.

  const Tags& poiTags = poi->getTags();
  const Tags& polyTags = poly->getTags();

  //The rules below are *roughly* ordered by increasing processing expense and by decreasing
  //likelihood of occurrence (probably needs some reordering at this point).

  const bool poiHasType = PoiPolygonTypeScoreExtractor::hasType(poi);
  LOG_VART(poiHasType);
  const bool polyHasType = PoiPolygonTypeScoreExtractor::hasType(poly);
  LOG_VART(polyHasType);

  // Yes, it is a pain to re-number these rules every time one is removed...think of a better way...

  LOG_TRACE("Checking rule #1...");
  if (_addressParsingEnabled)
  {
    //if both have addresses and they explicitly contradict each other, throw out the review; don't
    //do it if the poly has more than one address, like in many multi-use buildings.
    if (!_addressMatch && _hasAddress(poi) && _hasAddress(poly))
    {
      //check to make sure the only address the poly has isn't the poi itself as a way node /
      //relation member
      if (((polyAsWay && _polyContainsPoiAsMember(polyAsWay, poi)) ||
          (polyAsRelation && _polyContainsPoiAsMember(polyAsRelation, poi))) &&
           _addressParser.numAddressesRecursive(poly, *_map) < 2)
      {
      }
      else
      {
        LOG_TRACE("Returning miss per review reduction rule #1...");
        _triggeredRuleDescription = "#1";
        return true;
      }
    }
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #1: " << timer.elapsed());
//  }

  //timer.restart();
  LOG_TRACE("Checking rule #2...");
  if (_infoCache->hasCriterion(poly, QString::fromStdString(MultiUseCriterion::className())) &&
       poiHasType && _typeScore < 0.4)
  {
    LOG_TRACE("Returning miss per review reduction rule #2...");
    _triggeredRuleDescription = "#2";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #2: " << timer.elapsed());
//  }

  const QString poiPlaceVal = poiTags.get("place").toLower();
  const QString polyPlaceVal = polyTags.get("place").toLower();

  //be a little stricter on place related reviews
  //timer.restart();
  LOG_TRACE("Checking rule #3...");
  if ((poiPlaceVal == "neighbourhood" || poiPlaceVal == "suburb") && !polyTags.contains("place"))
  {
    LOG_TRACE("Returning miss per review reduction rule #3...");
    _triggeredRuleDescription = "#3";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #3: " << timer.elapsed());
//  }

  //timer.restart();

  const QString poiLanduseVal = poiTags.get("landuse").toLower();
  const bool poiHasLanduse = !poiLanduseVal.trimmed().isEmpty();
  const QString polyLanduseVal = polyTags.get("landuse").toLower();
  const bool polyHasLanduse = !polyLanduseVal.trimmed().isEmpty();

  // I think this one will eventually go away.
  LOG_TRACE("Checking rule #4...");
  if (poiTags.get("man_made").toLower() == "mine" && polyLanduseVal == "quarry" &&
      polyTags.get("man_made").toLower() != "mine")
  {
    LOG_TRACE("Returning miss per review reduction rule #4...");
    _triggeredRuleDescription = "#4";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #4: " << timer.elapsed());
//  }

  //points sitting on islands need to have an island type, or the match doesn't make any sense
  //timer.restart();
  LOG_TRACE("Checking rule #5...");
  if ((poiPlaceVal == "island" || polyPlaceVal == "island") && !_typeMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #5...");
    _triggeredRuleDescription = "#5";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #5: " << timer.elapsed());
//  }

  //timer.restart();

  const bool polyIsPark = _infoCache->isType(poly, "park");
  LOG_VART(polyIsPark);

  const QString poiLeisureVal = poiTags.get("leisure").toLower();
  const QString polyLeisureVal = polyTags.get("leisure").toLower();

  const QString poiName = poi->getTags().getName().toLower();
  const QString polyName = poly->getTags().getName().toLower();
  const bool poiHasName = !poiName.isEmpty();
  const bool polyHasName = !polyName.isEmpty();

  //similar to above, but for gardens
  LOG_TRACE("Checking rule #6...");
  if ((poiLeisureVal == "garden" || polyLeisureVal == "garden") &&
       (!_nonDistanceSimilaritiesPresent() ||
        (polyIsPark && !polyName.toLower().contains("garden"))))
  {
    LOG_TRACE("Returning miss per review reduction rule #6...");
    _triggeredRuleDescription = "#6";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #6: " << timer.elapsed());
//  }

  //lots of things have fountains, so let's raise the requirements a bit
  //timer.restart();
  LOG_TRACE("Checking rule #7...");
  if (poiTags.get("amenity") == "fountain" && _nameScore < 0.5)
  {
    LOG_TRACE("Returning miss per review reduction rule #7...");
    _triggeredRuleDescription = "#7";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #7: " << timer.elapsed());
//  }

  //these seem to be clustered together tightly a lot in cities, so up the requirement a bit
  // TODO: using custom match/review distances or custom score requirements may be a better way to
  // handle these types
  //timer.restart();
  LOG_TRACE("Checking rule #8...");
  if (poiTags.get("tourism") == "hotel" && polyTags.get("tourism") == "hotel" &&
      poiHasName && polyHasName && _nameScore < 0.75 && !_addressMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #8...");
    _triggeredRuleDescription = "#8";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #8: " << timer.elapsed());
//  }

  LOG_TRACE("Checking rule #9...");
  //timer.restart();
  if (_infoCache->isType(poi, "restaurant") && _infoCache->isType(poly, "restaurant") &&
      poiHasName && polyHasName && _nameScore < 0.5 && !_addressMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #9...");
    _triggeredRuleDescription = "#9";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #9: " << timer.elapsed());
//  }

  //similar to above, but for sport fields
  //timer.restart();
  LOG_TRACE("Checking rule #10...");
  const bool poiNameEndsWithField = poiName.endsWith("field");
  LOG_VART(poiNameEndsWithField);
  const bool polyIsSport = _infoCache->isType(poly, "sport");
  LOG_VART(polyIsSport);
  //we'll let this review pass if the poi ends with "field" in the name and is sitting on top of a
  //sport poly
  if (poiNameEndsWithField && polyIsSport)
  {
  }
  else if ((poiLeisureVal == "pitch" || polyLeisureVal == "pitch") &&
           !_nonDistanceSimilaritiesPresent())
  {
    LOG_TRACE("Returning miss per review reduction rule #10...");
    _triggeredRuleDescription = "#10";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #10: " << timer.elapsed());
//  }

  //Landuse polys often wrap a bunch of other features and don't necessarily match to POI's, so
  //be more strict with their reviews.
  //timer.restart();
  LOG_TRACE("Checking rule #11...");
  if (polyHasLanduse && !_nonDistanceSimilaritiesPresent())
  {
    LOG_TRACE("Returning miss per review reduction rule #11...");
    _triggeredRuleDescription = "#11";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #11: " << timer.elapsed());
//  }

  //timer.restart();
  LOG_TRACE("Checking rule #12...");
  if (_infoCache->isType(poi, "specificSchool") && _infoCache->isType(poly, "specificSchool") &&
      !PoiPolygonTypeScoreExtractor::specificSchoolMatch(poi, poly))
  {
    LOG_TRACE("Returning miss per review reduction rule #12...");
    _triggeredRuleDescription = "#12";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #12: " << timer.elapsed());
//  }

  //timer.restart();

  const bool poiIsNatural = _infoCache->isType(poi, "natural");
  const bool polyIsNatural = _infoCache->isType(poly, "natural");

  //Be more strict reviewing natural features and parks against building features.  This could be
  //extended
  LOG_TRACE("Checking rule #13...");
  if ((polyIsNatural || polyIsPark) && _inCategory(poi, OsmSchemaCategory::building()))
  {
    LOG_TRACE("Returning miss per review reduction rule #13...");
    _triggeredRuleDescription = "#13";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #13: " << timer.elapsed());
//  }

  //timer.restart();
  LOG_TRACE("Checking rule #14...");
  if (poiHasType && polyIsNatural && !_typeMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #14...");
    _triggeredRuleDescription = "#14";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #14: " << timer.elapsed());
//  }

  //inverse of above
  //timer.restart();
  LOG_TRACE("Checking rule #15...");
  if (polyHasType && poiIsNatural && !_typeMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #15...");
    _triggeredRuleDescription = "#15";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #15: " << timer.elapsed());
//  }

  //prevent athletic POIs within a park poly from being reviewed against that park poly
  //timer.restart();
  LOG_TRACE("Checking rule #16...");
  if (_distance == 0 && polyIsPark && poiLeisureVal == "pitch")
  {
    LOG_TRACE("Returning miss per review reduction rule #16...");
    _triggeredRuleDescription = "#16";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #16: " << timer.elapsed());
//  }

  //timer.restart();

  const bool poiIsPark = _infoCache->isType(poi, "park");
  LOG_VART(poiIsPark);

  //Don't match a park poi to a sport poly.  Simpler version of some rules above.
  //may make some previous rules obsolete
  LOG_TRACE("Checking rule #17...");
  if (poiIsPark && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #17...");
    _triggeredRuleDescription = "#17";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #17: " << timer.elapsed());
//  }

  //timer.restart();

  const bool polyIsBuilding =
    _infoCache->hasCriterion(poly, QString::fromStdString(BuildingCriterion::className()));
  LOG_VART(polyIsBuilding);

  // similar to previous, except more focused for restrooms
  LOG_TRACE("Checking rule #18...");
  if (poiHasType && polyHasType && _typeScore != 1.0 && _infoCache->isType(poi, "restroom") &&
      !_inCategory(poly, OsmSchemaCategory::building()))
  {
    LOG_TRACE("Returning miss per review reduction rule #18...");
    _triggeredRuleDescription = "#18";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #18: " << timer.elapsed());
//  }

  //Be more strict reviewing parking lots against other features
  //timer.restart();
  LOG_TRACE("Checking rule #19...");
  if (poiHasType && polyHasType && _infoCache->isType(poly, "parking") && !_typeMatch &&
      polyTags.get("parking") != "multi-storey")
  {
    LOG_TRACE("Returning miss per review reduction rule #19...");
    _triggeredRuleDescription = "#19";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//      LOG_DEBUG("rule #19: " << timer.elapsed());
//  }

  //Don't review schools against their sports fields.
  //timer.restart();
  LOG_TRACE("Checking rule #20...");
  if (_infoCache->isType(poi, "school") && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #20...");
    _triggeredRuleDescription = "#20";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #20: " << timer.elapsed());
//  }

  //Need to be stricter on tunnels since we don't want above ground things to review against them.
  //timer.restart();
  LOG_TRACE("Checking rule #21...");
  if (polyTags.get("tunnel") == "yes" && poiHasType &&
      (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    LOG_TRACE("Returning miss per review reduction rule #21...");
    _triggeredRuleDescription = "#21";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #21: " << timer.elapsed());
//  }

  //timer.restart();

  const double polyArea = _infoCache->getArea(poly);
  LOG_VART(polyArea);
  //Don't review park poi's against large non-park polys.
  LOG_TRACE("Checking rule #22...");
  if (polyArea != -1.0 && poiHasType && polyHasType && !_nameMatch && !polyIsPark &&
      (((polyHasLanduse && !poiHasLanduse) ||
       (polyIsNatural && !poiIsNatural) ||
        polyLeisureVal == "common") ||
       (polyPlaceVal == "neighborhood" || polyPlaceVal == "neighbourhood")) && polyArea > 50000)
  {
    LOG_TRACE("Returning miss per review reduction rule #22...");
    _triggeredRuleDescription = "#22";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #22: " << timer.elapsed());
//  }

  //timer.restart();

  const bool polyIsPlayground = _infoCache->isType(poly, "playground");
  LOG_VART(polyIsPlayground);
  const bool polyHasMoreThanOneType = PoiPolygonTypeScoreExtractor::hasMoreThanOneType(poly);
  LOG_VART(polyHasMoreThanOneType);
  const bool poiIsParkish = _infoCache->isType(poi, "parkish");
  LOG_VART(poiIsParkish);

  //This is a simple rule to prevent matching poi's not at all like a park with park polys.
  //this may render some of the previous rules obsolete.
  LOG_TRACE("Checking rule #23...");
  if (!poiIsPark && !poiIsParkish && poiHasType && polyIsPark && !polyHasMoreThanOneType)
  {
    LOG_TRACE("Returning miss per review reduction rule #23...");
    _triggeredRuleDescription = "#23";
    return true;
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #23: " << timer.elapsed());
//  }

  //If the poi is a way node, it belongs to a building way, and there is a type match between the
  //poi and the building way, then don't review the two.  This allows for tagged poi way nodes to
  //conflate cleanly with building ways they belong to, rather than being reviewed against other
  //building ways.
  //timer.restart();
  LOG_TRACE("Checking rule #24...");
  const long matchingWayId = BuildingWayNodeCriterion(_map).getMatchingWayId(poi);
  if (matchingWayId != 0)
  {
    ConstWayPtr matchingWay = _map->getWay(matchingWayId);
    assert(matchingWay.get());
    if (poly->getElementId() != matchingWay->getElementId() &&
        PoiPolygonTypeScoreExtractor().extract(*_map, poi, matchingWay)  >= _typeScoreThreshold)
    {
      LOG_TRACE("Returning miss per review reduction rule #24...");
      _triggeredRuleDescription = "#24";
      return true;
    }
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #24: " << timer.elapsed());
//  }

  timer.restart();

  //This portion is saved until last b/c it involves looping over all the neighboring data for
  //each of the features being compared.  This neighbor checking section could be abstracted to
  //types other than parks, if proven valuable to do so after testing against real world data.

  bool polyVeryCloseToAnotherParkPoly = false;
  double parkPolyAngleHistVal = -1.0;
  double parkPolyOverlapVal = -1.0;
  bool otherParkPolyNameMatch = false;
  double otherParkPolyNameScore = -1.0;
  double poiToPolyNodeDist = DBL_MAX;
  double poiToOtherParkPolyNodeDist = DBL_MAX;
  bool otherParkPolyHasName = false;
  bool poiContainedInAnotherParkPoly = false;
  bool sportPoiOnOtherSportPolyWithTypeMatch = false;
  bool poiOnBuilding = false;
  const bool poiIsSport = _infoCache->isType(poi, "sport");
  LOG_VART(poiIsSport);
  const bool poiContainedInParkPoly =
    poiContainedInAnotherParkPoly || (polyIsPark && _distance == 0);
  LOG_VART(poiContainedInParkPoly);

  const bool poiIsBuilding =
    _infoCache->hasCriterion(poi, QString::fromStdString(BuildingCriterion::className()));
  LOG_VART(poiIsBuilding);

  bool iterateOverPolyNeighbors = false;
  if ((poiIsPark || poiIsParkish) || // 25 pre-req
      (poiIsPark && !polyIsPark && !polyIsBuilding) || // 26 pre-req
      (poiIsPark && polyIsPark && _distance > 0) || // 27 pre-req
      (poiIsSport && poiContainedInParkPoly) || // 28 pre-req
      (poiIsBuilding && !polyIsBuilding) || // 29 pre-req
      // 30 pre-req
      (!polyHasMoreThanOneType && !poiIsPark && !poiIsParkish && poiHasType && _exactNameMatch))
  {
    iterateOverPolyNeighbors = true;
  }
  if (!iterateOverPolyNeighbors)
  {
//    if (timer.elapsed() > timingThreshold)
//    {
//      LOG_DEBUG("rule #25-30: " << timer.elapsed());
//    }
    return false;
  }

  LOG_VART(_polyNeighborIds.size());
  // This loop becomes more expensive as the search radius is increased.
  for (std::set<ElementId>::const_iterator polyNeighborItr = _polyNeighborIds.begin();
       polyNeighborItr != _polyNeighborIds.end(); ++polyNeighborItr)
  {
    ConstElementPtr polyNeighbor = _map->getElement(*polyNeighborItr);
    ConstWayPtr polyNeighborAsWay;
    if (polyNeighbor->getElementType() == ElementType::Way)
    {
      polyNeighborAsWay = std::dynamic_pointer_cast<const Way>(polyNeighbor);
    }
    if (polyNeighbor->getElementId() != poly->getElementId())
    {
      if (_infoCache->isType(polyNeighbor, "park"))
      {
        // Should this be OsmSchema::elementHasName instead?
        otherParkPolyHasName = !polyNeighbor->getTags().get("name").trimmed().isEmpty();
        otherParkPolyNameScore = PoiPolygonNameScoreExtractor().extract(*_map, poi, polyNeighbor);
        otherParkPolyNameMatch = otherParkPolyNameScore >= _nameScoreThreshold;

        if (_infoCache->polyContainsPoi(polyNeighborAsWay, poi))
        {
          poiContainedInAnotherParkPoly = true;
          LOG_TRACE(
            "poi examined and found to be contained within a park poly outside of this match " <<
            "comparison: " << poi->toString());
          LOG_TRACE("park poly it is very close to: " << polyNeighbor->toString());
        }

        if (_infoCache->elementIntersectsElement(polyNeighbor, poly))
        {
          parkPolyAngleHistVal = AngleHistogramExtractor().extract(*_map, polyNeighbor, poly);
          LOG_VART(parkPolyAngleHistVal);
          parkPolyOverlapVal =  OverlapExtractor().extract(*_map, polyNeighbor, poly);
          LOG_VART(parkPolyOverlapVal);

          //When just using intersection as the criteria, only found one instance when something
          //was considered as "very close" to a park poly when I didn't want it to be...so these
          //values set very low to weed that instance out...overlap at least.
          if (parkPolyAngleHistVal != -1.0 && parkPolyOverlapVal != 1.0 &&
              parkPolyAngleHistVal >= 0.05 && parkPolyOverlapVal >= 0.02)
          {
            polyVeryCloseToAnotherParkPoly = true;

            if (poly->getElementType() == ElementType::Way &&
                polyNeighbor->getElementType() == ElementType::Way)
            {
              //Calc the distance from the poi to the poly line instead of the poly itself.
              //Calcing distance to the poly itself will always return 0 when the poi is in the
              //poly.
              poiToPolyNodeDist = _infoCache->getPolyToPointDistance(polyAsWay, poi);
              poiToOtherParkPolyNodeDist =
                _infoCache->getPolyToPointDistance(polyNeighborAsWay, poi);
            }
            LOG_TRACE(
              "poly examined and found very close to a another park poly: " << poly->toString());
            LOG_TRACE("park poly it is very close to: " << polyNeighbor->toString());
          }
        }
      }
      else if (poiIsSport)
      {
        //this is a little loose, b/c there could be more than one type match set of tags...
        if (_infoCache->isType(polyNeighbor, "sport") &&
            _infoCache->polyContainsPoi(polyNeighborAsWay, poi) &&
            PoiPolygonTypeScoreExtractor().extract(*_map, poi, polyNeighbor) >= _typeScoreThreshold)
        {
          sportPoiOnOtherSportPolyWithTypeMatch = true;
        }
      }
      else if (_infoCache->hasCriterion(
                 polyNeighbor, QString::fromStdString(BuildingCriterion::className())))
      {
        if (_infoCache->polyContainsPoi(polyNeighborAsWay, poi))
        {
          poiOnBuilding = true;
        }
      }

      LOG_VART(poiToOtherParkPolyNodeDist);
      LOG_VART(polyVeryCloseToAnotherParkPoly);
      LOG_VART(poiToPolyNodeDist);
      LOG_VART(poiToOtherParkPolyNodeDist);
      LOG_VART(otherParkPolyHasName);
      LOG_VART(otherParkPolyNameMatch);
      LOG_VART(otherParkPolyNameScore);
      LOG_VART(poiContainedInAnotherParkPoly);
      LOG_VART(sportPoiOnOtherSportPolyWithTypeMatch);
      LOG_VART(poiOnBuilding);

      //If the POI is inside a poly that is very close to another park poly, declare miss if
      //the distance to the outer ring of the other park poly is shorter than the distance to the
      //outer ring of this poly and the other park poly has a name.
      LOG_TRACE("Checking rule #25...");
      if ((poiIsPark || poiIsParkish) && polyVeryCloseToAnotherParkPoly && _distance == 0 &&
          poiToOtherParkPolyNodeDist < poiToPolyNodeDist && poiToPolyNodeDist != DBL_MAX &&
          poiToPolyNodeDist != -1.0 && poiToOtherParkPolyNodeDist != DBL_MAX &&
          poiToOtherParkPolyNodeDist != -1.0 && otherParkPolyHasName && parkPolyOverlapVal < 0.9)
      {
        LOG_TRACE("Returning miss per review reduction rule #25...");
        _triggeredRuleDescription = "#25";
        return true;
      }

      //If the poi is a park, the poly it is being compared to is not a park or building, and that
      //poly is "very close" to another park poly that has a name match with the poi, then
      //declare a miss (exclude poly playgrounds from this).
      LOG_TRACE("Checking rule #26...");
      if (poiIsPark && !polyIsPark && !polyIsBuilding && polyVeryCloseToAnotherParkPoly &&
          otherParkPolyNameMatch)
      {
        LOG_TRACE("Returning miss per review reduction rule #26...");
        _triggeredRuleDescription = "#26";
        return true;
      }

      //If a park poi is contained within one park poly, then there's no reason for it to trigger
      //reviews in another one that its not contained in.
      LOG_TRACE("Checking rule #27...");
      if (poiIsPark && polyIsPark && _distance > 0 && poiContainedInAnotherParkPoly)
      {
        LOG_TRACE("Returning miss per review reduction rule #27...");
        _triggeredRuleDescription = "#27";
        return true;
      }

      //If a sport poi is contained within a type match sport poi poly, don't let it be
      //matched against anything else.
      LOG_TRACE("Checking rule #28...");
      if (poiIsSport && poiContainedInParkPoly && sportPoiOnOtherSportPolyWithTypeMatch)
      {
        LOG_TRACE("Returning miss per review reduction rule #28...");
        _triggeredRuleDescription = "#28";
        return true;
      }

      //If a poi is like and on top of a building, don't review it against a non-building poly.
      LOG_TRACE("Checking rule #29...");
      if (poiIsBuilding && poiOnBuilding && !polyIsBuilding)
      {
        LOG_TRACE("Returning miss per review reduction rule #29...");
        _triggeredRuleDescription = "#29";
        return true;
      }

      //If the poi is not a park and being compared to a park polygon or a polygon that is
      //"very close" to another park poly, we want to be more restrictive on type matching, but
      //only if the poi has any type at all.  If the poi has no type, then behave as normal.
      //Also, let an exact name match cause a review here, rather than a miss.
      LOG_TRACE("Checking rule #30...");
      if ((polyIsPark || (polyVeryCloseToAnotherParkPoly && !polyHasType)) &&
          !polyHasMoreThanOneType && !poiIsPark && !poiIsParkish && poiHasType && _exactNameMatch)
      {
        LOG_TRACE("Returning miss per review reduction rule #30...");
        _triggeredRuleDescription = "#30";
        return true;
      }
    }
  }
//  if (timer.elapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #25-30: " << timer.elapsed());
//  }

  return false;
}

}
