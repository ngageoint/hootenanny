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
#include <hoot/core/conflate/poi-polygon/PoiPolygonType.h>

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

bool PoiPolygonReviewReducer::_nonDistanceSimilaritiesPresent() const
{
  return _typeScore > 0.03 || _nameScore > 0.35 || _addressMatch;
}

bool PoiPolygonReviewReducer::triggersRule(ConstNodePtr poi, ConstElementPtr poly)
{
  LOG_TRACE("Checking review reduction rules...");
  _triggeredRuleDescription = "";
  //QElapsedTimer timer;
  //const int timingThreshold = 1000000;  //nanosec

  // Some of these rules could be obsolete.

  const Tags& poiTags = poi->getTags();
  const Tags& polyTags = poly->getTags();

  //The rules below are *roughly* ordered by increasing processing expense and by decreasing
  //likelihood of occurrence (probably needs some reordering at this point).

  // Yes, it is a pain to re-number these rules every time one is removed...think of a better way...

  //timer.restart();
  const bool poiHasType = _infoCache->hasRelatedType(poi);
  LOG_VART(poiHasType);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("poiHasType: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  LOG_TRACE("Checking rule #2...");
  if (_typeScore < 0.4 &&
      poiHasType &&
      _infoCache->hasCriterion(poly, QString::fromStdString(MultiUseCriterion::className())))
  {
    LOG_TRACE("Returning miss per review reduction rule #2...");
    _triggeredRuleDescription = "#2";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #2: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const QString poiPlaceVal = poiTags.get("place").toLower();
  const QString polyPlaceVal = polyTags.get("place").toLower();
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("place: " << timer.nsecsElapsed());
//  }

  //be a little stricter on place related reviews
  //timer.restart();
  LOG_TRACE("Checking rule #3...");
  if (!polyTags.contains("place") &&
      (poiPlaceVal == QLatin1String("neighbourhood") || poiPlaceVal == QLatin1String("suburb")))
  {
    LOG_TRACE("Returning miss per review reduction rule #3...");
    _triggeredRuleDescription = "#3";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #3: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const QString poiLanduseVal = poiTags.get("landuse").toLower();
  const bool poiHasLanduse = !poiLanduseVal.trimmed().isEmpty();
  const QString polyLanduseVal = polyTags.get("landuse").toLower();
  const bool polyHasLanduse = !polyLanduseVal.trimmed().isEmpty();
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("landuse: " << timer.nsecsElapsed());
//  }

  // I think this one will eventually go away.
  //timer.restart();
  LOG_TRACE("Checking rule #4...");
  if (poiTags.get("man_made").toLower() == QLatin1String("mine") &&
      polyLanduseVal == QLatin1String("quarry") &&
      polyTags.get("man_made").toLower() != QLatin1String("mine"))
  {
    LOG_TRACE("Returning miss per review reduction rule #4...");
    _triggeredRuleDescription = "#4";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #4: " << timer.nsecsElapsed());
//  }

  //points sitting on islands need to have an island type, or the match doesn't make any sense
  //timer.restart();
  LOG_TRACE("Checking rule #5...");
  if (!_typeMatch &&
      (poiPlaceVal == QLatin1String("island") || polyPlaceVal == QLatin1String("island")))
  {
    LOG_TRACE("Returning miss per review reduction rule #5...");
    _triggeredRuleDescription = "#5";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #5: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool polyIsPark = _infoCache->isType(poly, "park");
  LOG_VART(polyIsPark);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("polyIsPark: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const QString poiLeisureVal = poiTags.get("leisure").toLower();
  const QString polyLeisureVal = polyTags.get("leisure").toLower();
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("leisure: " << timer.nsecsElapsed());
//  }

  const QString poiName = poi->getTags().getName().toLower();
  const QString polyName = poly->getTags().getName().toLower();
  const bool poiHasName = !poiName.isEmpty();
  const bool polyHasName = !polyName.isEmpty();

  //similar to above, but for gardens
  //timer.restart();
  LOG_TRACE("Checking rule #6...");
  if ((!_nonDistanceSimilaritiesPresent() ||
       (polyIsPark && !polyName.contains("garden"))) &&
      (poiLeisureVal == QLatin1String("garden") || polyLeisureVal == QLatin1String("garden")))
  {
    LOG_TRACE("Returning miss per review reduction rule #6...");
    _triggeredRuleDescription = "#6";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #6: " << timer.nsecsElapsed());
//  }

  //lots of things have fountains, so let's raise the requirements a bit
  //timer.restart();
  LOG_TRACE("Checking rule #7...");
  if (_nameScore < 0.5 && poiTags.get("amenity").toLower() == QLatin1String("fountain"))
  {
    LOG_TRACE("Returning miss per review reduction rule #7...");
    _triggeredRuleDescription = "#7";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #7: " << timer.nsecsElapsed());
//  }

  //these seem to be clustered together tightly a lot in cities, so up the requirement a bit
  // TODO: using custom match/review distances or custom score requirements may be a better way to
  // handle these types
  //timer.restart();
  LOG_TRACE("Checking rule #8...");
  if (poiHasName && polyHasName && _nameScore < 0.75 && !_addressMatch &&
      poiTags.get("tourism").toLower() == QLatin1String("hotel") &&
      polyTags.get("tourism").toLower() == QLatin1String("hotel"))
  {
    LOG_TRACE("Returning miss per review reduction rule #8...");
    _triggeredRuleDescription = "#8";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #8: " << timer.nsecsElapsed());
//  }

  LOG_TRACE("Checking rule #9...");
  //timer.restart();
  if (poiHasName && polyHasName && _nameScore < 0.5 && !_addressMatch &&
      _infoCache->isType(poi, "restaurant") && _infoCache->isType(poly, "restaurant"))
  {
    LOG_TRACE("Returning miss per review reduction rule #9...");
    _triggeredRuleDescription = "#9";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #9: " << timer.nsecsElapsed());
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
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #11: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  LOG_TRACE("Checking rule #12...");
  if (_infoCache->isType(poi, "specificSchool") && _infoCache->isType(poly, "specificSchool") &&
      !PoiPolygonType::specificSchoolMatch(poi, poly))
  {
    LOG_TRACE("Returning miss per review reduction rule #12...");
    _triggeredRuleDescription = "#12";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #12: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool polyIsNatural = _infoCache->isType(poly, "natural");
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("polyIsNatural: " << timer.nsecsElapsed());
//  }

  //Be more strict reviewing natural features and parks against building features.  This could be
  //extended
  //timer.restart();
  LOG_TRACE("Checking rule #13...");
  if ((polyIsNatural || polyIsPark) && _infoCache->inBuildingCategory(poi))
  {
    LOG_TRACE("Returning miss per review reduction rule #13...");
    _triggeredRuleDescription = "#13";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #13: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  LOG_TRACE("Checking rule #14...");
  if (poiHasType && polyIsNatural && !_typeMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #14...");
    _triggeredRuleDescription = "#14";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #14: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool polyHasType = _infoCache->hasRelatedType(poly);
  LOG_VART(polyHasType);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("polyHasType: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool poiIsNatural = _infoCache->isType(poi, "natural");
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("poiIsNatural: " << timer.nsecsElapsed());
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
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #15: " << timer.nsecsElapsed());
//  }

  //prevent athletic POIs within a park poly from being reviewed against that park poly
  //timer.restart();
  LOG_TRACE("Checking rule #16...");
  if (_distance == 0 && polyIsPark && poiLeisureVal == QLatin1String("pitch"))
  {
    LOG_TRACE("Returning miss per review reduction rule #16...");
    _triggeredRuleDescription = "#16";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #16: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool poiIsPark = _infoCache->isType(poi, "park");
  LOG_VART(poiIsPark);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("poiIsPark: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool polyIsSport = _infoCache->isType(poly, "sport");
  LOG_VART(polyIsSport);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("polyIsSport: " << timer.nsecsElapsed());
//  }

  //similar to above, but for sport fields
  //timer.restart();
  LOG_TRACE("Checking rule #10...");
  const bool poiNameEndsWithField = poiName.endsWith("field");
  LOG_VART(poiNameEndsWithField);
  //we'll let this review pass if the poi ends with "field" in the name and is sitting on top of a
  //sport poly
  if (poiNameEndsWithField && polyIsSport)
  {
  }
  else if (!_nonDistanceSimilaritiesPresent() &&
           (poiLeisureVal == QLatin1String("pitch") || polyLeisureVal == QLatin1String("pitch")))
  {
    LOG_TRACE("Returning miss per review reduction rule #10...");
    _triggeredRuleDescription = "#10";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #10: " << timer.nsecsElapsed());
//  }

  //Don't match a park poi to a sport poly.  Simpler version of some rules above.
  //may make some previous rules obsolete
  //timer.restart();
  LOG_TRACE("Checking rule #17...");
  if (poiIsPark && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #17...");
    _triggeredRuleDescription = "#17";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #17: " << timer.nsecsElapsed());
//  }

  // similar to previous, except more focused for restrooms
  //timer.restart();
  LOG_TRACE("Checking rule #18...");
  if (poiHasType && polyHasType && _typeScore != 1.0 && _infoCache->isType(poi, "restroom") &&
      !_infoCache->inBuildingCategory(poly))
  {
    LOG_TRACE("Returning miss per review reduction rule #18...");
    _triggeredRuleDescription = "#18";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #18: " << timer.nsecsElapsed());
//  }

  //Be more strict reviewing parking lots against other features
  //timer.restart();
  LOG_TRACE("Checking rule #19...");
  if (poiHasType && polyHasType && !_typeMatch && _infoCache->isType(poly, "parking") &&
      polyTags.get("parking").toLower() != QLatin1String("multi-storey"))
  {
    LOG_TRACE("Returning miss per review reduction rule #19...");
    _triggeredRuleDescription = "#19";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #19: " << timer.nsecsElapsed());
//  }

  //Don't review schools against their sports fields.
  //timer.restart();
  LOG_TRACE("Checking rule #20...");
  if (polyIsSport && _infoCache->isType(poi, "school"))
  {
    LOG_TRACE("Returning miss per review reduction rule #20...");
    _triggeredRuleDescription = "#20";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #20: " << timer.nsecsElapsed());
//  }

  //Need to be stricter on tunnels since we don't want above ground things to review against them.
  //timer.restart();
  LOG_TRACE("Checking rule #21...");
  if (poiHasType &&
      (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)) &&
      polyTags.get("tunnel").toLower() == QLatin1String("yes"))
  {
    LOG_TRACE("Returning miss per review reduction rule #21...");
    _triggeredRuleDescription = "#21";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #21: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  //Don't review park poi's against large non-park polys.
  LOG_TRACE("Checking rule #22...");
  if (poiHasType && polyHasType && !_nameMatch && !polyIsPark &&
      (((polyHasLanduse && !poiHasLanduse) ||
       (polyIsNatural && !poiIsNatural) ||
        polyLeisureVal == QLatin1String("common")) ||
       (polyPlaceVal == QLatin1String("neighborhood") ||
        polyPlaceVal == QLatin1String("neighbourhood"))) &&
      _infoCache->getArea(poly) > 50000)
  {
    LOG_TRACE("Returning miss per review reduction rule #22...");
    _triggeredRuleDescription = "#22";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #22: " << timer.nsecsElapsed());
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
        PoiPolygonTypeScoreExtractor(_infoCache).extract(*_map, poi, matchingWay) >=
          _typeScoreThreshold)
    {
      LOG_TRACE("Returning miss per review reduction rule #24...");
      _triggeredRuleDescription = "#24";
      return true;
    }
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #24: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool polyHasMoreThanOneType = _infoCache->hasMoreThanOneType(poly);
  LOG_VART(polyHasMoreThanOneType);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("polyHasMoreThanOneType: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool poiIsParkish = _infoCache->isType(poi, "parkish");
  LOG_VART(poiIsParkish);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("playground/parkish: " << timer.nsecsElapsed());
//  }

  //This is a simple rule to prevent matching poi's not at all like a park with park polys.
  //this may render some of the previous rules obsolete.
  //timer.restart();
  LOG_TRACE("Checking rule #23...");
  if (!poiIsPark && !poiIsParkish && poiHasType && polyIsPark && !polyHasMoreThanOneType)
  {
    LOG_TRACE("Returning miss per review reduction rule #23...");
    _triggeredRuleDescription = "#23";
    return true;
  }
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #23: " << timer.nsecsElapsed());
//  }

  LOG_TRACE("Checking rule #1...");
  if (_addressParsingEnabled)
  {
    //if both have addresses and they explicitly contradict each other, throw out the review; don't
    //do it if the poly has more than one address, like in many multi-use buildings.

    if (!_addressMatch)
    {
      //timer.restart();
      const int numPoiAddresses = _infoCache->numAddresses(poi);
      const int numPolyAddresses = _infoCache->numAddresses(poly);
//      if (timer.nsecsElapsed() > timingThreshold)
//      {
//        LOG_DEBUG("rule #1 numAddresses: " << timer.nsecsElapsed());
//      }

      if (numPoiAddresses > 0 && numPolyAddresses > 0)
      {
        //check to make sure the only address the poly has isn't the poi itself as a way node /
        //relation member
        //timer.restart();
        if (numPolyAddresses < 2 && _infoCache->containsMember(poly, poi->getElementId()))
        {
        }
        else
        {
          LOG_TRACE("Returning miss per review reduction rule #1...");
          _triggeredRuleDescription = "#1";
          return true;
        }
//        if (timer.nsecsElapsed() > timingThreshold)
//        {
//          LOG_DEBUG("rule #1 contains: " << timer.nsecsElapsed());
//        }
      }
    }
  }

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
  //timer.restart();
  const bool poiIsSport = _infoCache->isType(poi, "sport");
  LOG_VART(poiIsSport);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("poiIsSport: " << timer.nsecsElapsed());
//  }
  const bool poiContainedInParkPoly =
    poiContainedInAnotherParkPoly || (polyIsPark && _distance == 0);
  LOG_VART(poiContainedInParkPoly);

  //timer.restart();
  const bool poiIsBuilding =
    _infoCache->hasCriterion(poi, QString::fromStdString(BuildingCriterion::className()));
  LOG_VART(poiIsBuilding);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("poiIsBuilding: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  const bool polyIsBuilding =
    _infoCache->hasCriterion(poly, QString::fromStdString(BuildingCriterion::className()));
  LOG_VART(polyIsBuilding);
//  if (timer.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("polyIsBuilding: " << timer.nsecsElapsed());
//  }

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
    return false;
  }

  //QElapsedTimer timer2;
  //timer2.start();
  //timer.restart();
  LOG_VART(_polyNeighborIds.size());
  // This loop becomes more expensive as the search radius is increased.
  for (std::set<ElementId>::const_iterator polyNeighborItr = _polyNeighborIds.begin();
       polyNeighborItr != _polyNeighborIds.end(); ++polyNeighborItr)
  {
    ConstElementPtr polyNeighbor = _map->getElement(*polyNeighborItr);
    if (polyNeighbor->getElementId() != poly->getElementId())
    {
      //this is a little loose, b/c there could be more than one type match set of tags...
      //timer.restart();
      if (poiIsSport && _infoCache->isType(polyNeighbor, "sport") &&
          _infoCache->polyContainsPoint(polyNeighbor, poi) &&
          PoiPolygonTypeScoreExtractor(_infoCache).extract(*_map, poi, polyNeighbor) >=
           _typeScoreThreshold)
      {
        sportPoiOnOtherSportPolyWithTypeMatch = true;
//        if (timer.nsecsElapsed() > timingThreshold)
//        {
//          LOG_DEBUG("sportPoiOnOtherSportPolyWithTypeMatch: " << timer.nsecsElapsed());
//        }
      }
      else if (_infoCache->hasCriterion(
                 polyNeighbor, QString::fromStdString(BuildingCriterion::className())) &&
               _infoCache->polyContainsPoint(polyNeighbor/*AsWay*/, poi))
      {
        poiOnBuilding = true;
//        if (timer.nsecsElapsed() > timingThreshold)
//        {
//          LOG_DEBUG("poiOnBuilding: " << timer.nsecsElapsed());
//        }
      }
      else if (_infoCache->isType(polyNeighbor, "park"))
      {
        // Should this be OsmSchema::elementHasName instead?
        otherParkPolyHasName = !polyNeighbor->getTags().get("name").trimmed().isEmpty();
        otherParkPolyNameScore = PoiPolygonNameScoreExtractor().extract(*_map, poi, polyNeighbor);
        otherParkPolyNameMatch = otherParkPolyNameScore >= _nameScoreThreshold;

        if (_infoCache->polyContainsPoint(polyNeighbor/*AsWay*/, poi))
        {
          poiContainedInAnotherParkPoly = true;
          LOG_TRACE(
            "poi examined and found to be contained within a park poly outside of this match " <<
            "comparison: " << poi->toString());
          LOG_TRACE("park poly it is very close to: " << polyNeighbor->toString());
        }

        //timer.restart();
        if (_infoCache->elementIntersectsElement(polyNeighbor, poly))
        {
//          if (timer.nsecsElapsed() > timingThreshold)
//          {
//            LOG_DEBUG("elementIntersectsElement: " << timer.nsecsElapsed());
//          }

          //timer.restart();
          parkPolyAngleHistVal = AngleHistogramExtractor().extract(*_map, polyNeighbor, poly);
          LOG_VART(parkPolyAngleHistVal);
//          if (timer.nsecsElapsed() > timingThreshold)
//          {
//            LOG_DEBUG("parkPolyAngleHistVal: " << timer.nsecsElapsed());
//          }
          //timer.restart();
          parkPolyOverlapVal =  OverlapExtractor().extract(*_map, polyNeighbor, poly);
          LOG_VART(parkPolyOverlapVal);
//          if (timer.nsecsElapsed() > timingThreshold)
//          {
//            LOG_DEBUG("parkPolyOverlapVal: " << timer.nsecsElapsed());
//          }

          //When just using intersection as the criteria, only found one instance when something
          //was considered as "very close" to a park poly when I didn't want it to be...so these
          //values set very low to weed that instance out...overlap at least.
          if (parkPolyAngleHistVal != -1.0 && parkPolyOverlapVal != 1.0 &&
              parkPolyAngleHistVal >= 0.05 && parkPolyOverlapVal >= 0.02)
          {
            polyVeryCloseToAnotherParkPoly = true;

            //Calc the distance from the poi to the poly line instead of the poly itself.
            //Calcing distance to the poly itself will always return 0 when the poi is in the
            //poly.
            poiToPolyNodeDist = _infoCache->getPolyToPointDistance(poly, poi);
            poiToOtherParkPolyNodeDist =
              _infoCache->getPolyToPointDistance(polyNeighbor, poi);
            LOG_TRACE(
              "poly examined and found very close to a another park poly: " << poly->toString());
            LOG_TRACE("park poly it is very close to: " << polyNeighbor->toString());
          }
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
//  if (timer2.nsecsElapsed() > timingThreshold)
//  {
//    LOG_DEBUG("rule #25-30: " << timer2.nsecsElapsed());
//  }

  return false;
}

}
