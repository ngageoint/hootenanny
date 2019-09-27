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

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/algorithms/extractors/OverlapExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonNameScoreExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>
#include <hoot/core/criterion/BuildingWayNodeCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/MultiUseCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>

// Std
#include <float.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

QHash<QString, double> PoiPolygonReviewReducer::_elementDistanceCache;
QHash<QString, bool> PoiPolygonReviewReducer::_elementContainsCache;
QHash<QString, bool> PoiPolygonReviewReducer::_elementIntersectsCache;
QHash<QString, double> PoiPolygonReviewReducer::_elementAngleHistogramCache;
QHash<ElementId, double> PoiPolygonReviewReducer::_elementAreaCache;
QHash<ElementId, bool> PoiPolygonReviewReducer::_hasTypeCache;
QHash<ElementId, bool> PoiPolygonReviewReducer::_hasMoreThanOneTypeCache;
QHash<ElementId, int> PoiPolygonReviewReducer::_numAddressesCache;
QHash<QString, double> PoiPolygonReviewReducer::_elementOverlapCache;
QHash<ElementId, std::shared_ptr<geos::geom::Geometry>> PoiPolygonReviewReducer::_geometryCache;
QHash<ElementId, std::shared_ptr<geos::geom::LineString>> PoiPolygonReviewReducer::_lineStringCache;
QMap<QString, int> PoiPolygonReviewReducer::_numCacheHitsByCacheType;
QMap<QString, int> PoiPolygonReviewReducer::_cacheSizeByCacheType;
QHash<QString, double> PoiPolygonReviewReducer::_nameScoreCache;
QHash<QString, double> PoiPolygonReviewReducer::_typeScoreCache;
QHash<QString, bool> PoiPolygonReviewReducer::_specificSchoolMatchCache;
QHash<QString, bool> PoiPolygonReviewReducer::_isTypeCache;
QHash<QString, bool> PoiPolygonReviewReducer::_hasCriterionCache;
QHash<QString, ElementCriterionPtr> PoiPolygonReviewReducer::_criterionCache;

PoiPolygonReviewReducer::PoiPolygonReviewReducer(
  const ConstOsmMapPtr& map, const std::set<ElementId>& polyNeighborIds,
  const std::set<ElementId>& poiNeighborIds, double distance, double nameScoreThreshold,
  double nameScore, bool nameMatch, bool exactNameMatch, double typeScoreThreshold,
  double typeScore, bool typeMatch, double matchDistanceThreshold, bool addressMatch,
  bool addressParsingEnabled) :
_map(map),
_polyNeighborIds(polyNeighborIds),
_poiNeighborIds(poiNeighborIds),
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
_badGeomCount(0),
_keepClosestMatchesOnly(ConfigOptions().getPoiPolygonKeepClosestMatchesOnly()),
_addressParsingEnabled(addressParsingEnabled)
{
  LOG_VART(_polyNeighborIds.size());
  LOG_VART(_poiNeighborIds.size());
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

  _elementDistanceCache.reserve(_map->size());
  _elementAreaCache.reserve(_map->size());
  _elementContainsCache.reserve(_map->size());
  _elementIntersectsCache.reserve(_map->size());
  _elementAngleHistogramCache.reserve(_map->size());
  _elementOverlapCache.reserve(_map->size());
  _nameScoreCache.reserve(_map->size());
  _typeScoreCache.reserve(_map->size());
  _isTypeCache.reserve(_map->size());
  _hasCriterionCache.reserve(_map->size());
  _specificSchoolMatchCache.reserve(_map->size());
  _hasTypeCache.reserve(_map->size());
  _hasMoreThanOneTypeCache.reserve(_map->size());
  _numAddressesCache.reserve(_map->size());
}

void PoiPolygonReviewReducer::clearCaches()
{
  _elementDistanceCache.clear();
  _elementAreaCache.clear();
  _elementContainsCache.clear();
  _elementIntersectsCache.clear();
  _elementAngleHistogramCache.clear();
  _elementOverlapCache.clear();
  _geometryCache.clear();
  _lineStringCache.clear();
  _numCacheHitsByCacheType.clear();
  _cacheSizeByCacheType.clear();
}

void PoiPolygonReviewReducer::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);
}

bool PoiPolygonReviewReducer::_nonDistanceSimilaritiesPresent() const
{
  return _typeScore > 0.03 || _nameScore > 0.35 || _addressMatch;
}

bool PoiPolygonReviewReducer::_polyContainsPoiAsMember(ConstElementPtr poly,
                                                       ConstElementPtr poi) const
{
  ConstWayPtr polyWay = std::dynamic_pointer_cast<const Way>(poly);
  if (polyWay && polyWay->containsNodeId(poi->getId()))
  {
    return true;
  }
  ConstRelationPtr polyRelation = std::dynamic_pointer_cast<const Relation>(poly);
  if (polyRelation && polyRelation->contains(ElementId(ElementType::Node, poi->getId())))
  {
    return true;
  }
  return false;
}

bool PoiPolygonReviewReducer::triggersRule(ConstElementPtr poi, ConstElementPtr poly)
{
  LOG_TRACE("Checking review reduction rules...");
  QElapsedTimer timer;
  timer.start();

  // Some of these rules may be obsolete...they need to be checked.

  const Tags& poiTags = poi->getTags();
  const Tags& polyTags = poly->getTags();

  //The rules below are *roughly* ordered by increasing processing expense and by decreasing
  //likelihood of occurrence (probably needs some reordering at this point).

  const bool poiHasType = _hasType(poi);
  LOG_VART(poiHasType);
  const bool polyHasType = _hasType(poly);
  LOG_VART(polyHasType);

  if (_addressParsingEnabled)
  {
    //if both have addresses and they explicitly contradict each other, throw out the review; don't
    //do it if the poly has more than one address, like in many multi-use buildings.
    if (!_addressMatch && _hasAddress(poi) && _hasAddress(poly))
    {
      //check to make sure the only address the poly has isn't the poi itself as a way node /
      //relation member
      if (_polyContainsPoiAsMember(poly, poi) && _getNumAddresses(poly) < 2)
      {
      }
      else
      {
        LOG_TRACE("Returning miss per review reduction rule #1...");
        return true;
      }
    }
  }
  if (_hasCrit(poly, QString::fromStdString(MultiUseCriterion::className())) && poiHasType &&
      _typeScore < 0.4)
  {
    LOG_TRACE("Returning miss per review reduction rule #2...");
    return true;
  }

  const QString poiPlaceVal = poiTags.get("place").toLower();
  const QString polyPlaceVal = polyTags.get("place").toLower();

  //be a little stricter on place related reviews
  if ((poiPlaceVal == "neighbourhood" || poiPlaceVal == "suburb") && !polyTags.contains("place"))
  {
    LOG_TRACE("Returning miss per review reduction rule #3...");
    return true;
  }

  const QString poiLanduseVal = poiTags.get("landuse").toLower();
  const bool poiHasLanduse = !poiLanduseVal.trimmed().isEmpty();
  const QString polyLanduseVal = polyTags.get("landuse").toLower();
  const bool polyHasLanduse = !polyLanduseVal.trimmed().isEmpty();

  // I think this one will eventually go away.
  if (poiTags.get("man_made").toLower() == "mine" && polyLanduseVal == "quarry" &&
      polyTags.get("man_made").toLower() != "mine")
  {
    LOG_TRACE("Returning miss per review reduction rule #4...");
    return true;
  }

  //points sitting on islands need to have an island type, or the match doesn't make any sense
  if ((poiPlaceVal == "island" || polyPlaceVal == "island") && !_typeMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #5...");
    return true;
  }

  const bool polyIsPark = _isType(poly, "park");
  LOG_VART(polyIsPark);

  const QString poiLeisureVal = poiTags.get("leisure").toLower();
  const QString polyLeisureVal = polyTags.get("leisure").toLower();

  const QString poiName = poi->getTags().getName().toLower();
  const QString polyName = poly->getTags().getName().toLower();
  const bool poiHasName = !poiName.isEmpty();
  const bool polyHasName = !polyName.isEmpty();

  //similar to above, but for gardens
  if ((poiLeisureVal == "garden" || polyLeisureVal == "garden") &&
       (!_nonDistanceSimilaritiesPresent() ||
        (polyIsPark && !polyName.toLower().contains("garden"))))
  {
    LOG_TRACE("Returning miss per review reduction rule #6...");
    return true;
  }

  //lots of things have fountains, so let's raise the requirements a bit
  if (poiTags.get("amenity") == "fountain" && _nameScore < 0.5)
  {
    LOG_TRACE("Returning miss per review reduction rule #7...");
    return true;
  }

  //these seem to be clustered together tightly a lot in cities, so up the requirement a bit
  // TODO: using custom match/review distances or custom score requirements may be a better way to
  // handle these types
  if (poiTags.get("tourism") == "hotel" && polyTags.get("tourism") == "hotel" &&
      poiHasName && polyHasName && _nameScore < 0.75 && !_addressMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #8...");
    return true;
  }

  if (_isType(poi, "restaurant") && _isType(poly, "restaurant") && poiHasName && polyHasName &&
      _nameScore < 0.5 && !_addressMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #9...");
    return true;
  }

  //similar to above, but for sport fields
  const bool poiNameEndsWithField = poiName.endsWith("field");
  LOG_VART(poiNameEndsWithField);
  const bool polyIsSport = _isType(poly, "sport");
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
    return true;
  }

  //Landuse polys often wrap a bunch of other features and don't necessarily match to POI's, so
  //be more strict with their reviews.
  if (polyHasLanduse && !_nonDistanceSimilaritiesPresent())
  {
    LOG_TRACE("Returning miss per review reduction rule #11...");
    return true;
  }

  LOG_VART(PoiPolygonTypeScoreExtractor::isSpecificSchool(poi));
  LOG_VART(PoiPolygonTypeScoreExtractor::isSpecificSchool(poly));
  LOG_VART(PoiPolygonTypeScoreExtractor::specificSchoolMatch(poi, poly));
  if (_isType(poi, "specificSchool") && _isType(poly, "specificSchool") &&
      !_specificSchoolMatch(poi, poly))
  {
    LOG_TRACE("Returning miss per review reduction rule #12...");
    return true;
  }

  const bool poiIsNatural = _isType(poi, "natural");
  const bool polyIsNatural = _isType(poly, "natural");

  //Be more strict reviewing natural features and parks against building features.  This could be
  //extended
  if ((polyIsNatural || polyIsPark) &&
      OsmSchema::getInstance().getCategories(poiTags).intersects(OsmSchemaCategory::building()))
  {
    LOG_TRACE("Returning miss per review reduction rule #13...");
    return true;
  }

  if (poiHasType && polyIsNatural && !_typeMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #14...");
    return true;
  }

  //inverse of above
  if (polyHasType && poiIsNatural && !_typeMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #15...");
    return true;
  }

  //prevent athletic POIs within a park poly from being reviewed against that park poly
  if (_distance == 0 && polyIsPark && poiLeisureVal == "pitch")
  {
    LOG_TRACE("Returning miss per review reduction rule #16...");
    return true;
  }

  const bool poiIsPark = _isType(poi, "park");
  LOG_VART(poiIsPark);

  //Don't match a park poi to a sport poly.  Simpler version of some rules above.
  //may make some previous rules obsolete
  if (poiIsPark && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #17...");
    return true;
  }

  BuildingCriterion buildingCrit;
  const bool polyIsBuilding =
    _hasCrit(poly, QString::fromStdString(BuildingCriterion::className()));
  LOG_VART(polyIsBuilding);

  //Similar to previous, except more focused for restrooms.
  if (poiHasType && polyHasType && _typeScore != 1.0 && _isType(poi, "restroom") &&
      !OsmSchema::getInstance().getCategories(polyTags).intersects(OsmSchemaCategory::building()))
  {
    LOG_TRACE("Returning miss per review reduction rule #18...");
    return true;
  }

  //Be more strict reviewing parking lots against other features.
  if (poiHasType && polyHasType && _isType(poly, "parking") && !_typeMatch &&
      polyTags.get("parking") != "multi-storey")
  {
    LOG_TRACE("Returning miss per review reduction rule #19...");
    return true;
  }

  //Don't review schools against their sports fields.
  if (_isType(poi, "school") && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #20...");
    return true;
  }

  //Need to be stricter on tunnels since we don't want above ground things to review against them.
  if (polyTags.get("tunnel") == "yes" && poiHasType &&
      (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    LOG_TRACE("Returning miss per review reduction rule #21...");
    return true;
  }

  std::shared_ptr<geos::geom::Geometry> polyGeom = _getGeometry(poly);
  LOG_VART(polyGeom.get());
  if (!polyGeom.get() ||
      QString::fromStdString(polyGeom->toString()).toUpper().contains("EMPTY")) // do we need still this?
  {
    throw geos::util::TopologyException();
  }

  const double polyArea = _getArea(std::dynamic_pointer_cast<const Way>(poly));
  LOG_VART(polyArea);
  //Don't review park poi's against large non-park polys.
  if (poiHasType && polyHasType && !_nameMatch && !polyIsPark &&
      (((polyHasLanduse && !poiHasLanduse) ||
       (polyIsNatural && !poiIsNatural) ||
        polyLeisureVal == "common") ||
       (polyPlaceVal == "neighborhood" || polyPlaceVal == "neighbourhood")) && polyArea > 50000)
  {
    LOG_TRACE("Returning miss per review reduction rule #22...");
    return true;
  }

  const bool polyIsPlayground = _isType(poly, "playground");
  LOG_VART(polyIsPlayground);
  const bool polyHasMoreThanOneType = _hasMoreThanOneType(poly);
  LOG_VART(polyHasMoreThanOneType);
  const bool poiIsParkish = _isType(poi, "parkish");
  LOG_VART(poiIsParkish);

  //This is a simple rule to prevent matching poi's not at all like a park with park polys.
  //this may render some of the previous rules obsolete.
  if (!poiIsPark && !poiIsParkish && poiHasType && polyIsPark && !polyHasMoreThanOneType)
  {
    LOG_TRACE("Returning miss per review reduction rule #23...");
    return true;
  }

  PoiPolygonTypeScoreExtractor typeScorer;

  //If the poi is a way node, it belongs to a building way, and there is a type match between the
  //poi and the building way, then don't review the two.  This allows for tagged poi way nodes to
  //conflate cleanly with building ways they belong to, rather than being reviewed against other
  //building ways.
  const long matchingWayId = BuildingWayNodeCriterion(_map).getMatchingWayId(poi);
  if (matchingWayId != 0)
  {
    ConstWayPtr matchingWay = _map->getWay(matchingWayId);
    assert(matchingWay.get());
    if (poly->getElementId() != matchingWay->getElementId() &&
        typeScorer.extract(*_map, poi, matchingWay) >= _typeScoreThreshold)
    {
      LOG_TRACE("Returning miss per review reduction rule #24...");
      return true;
    }
  }

  //This portion is saved until last b/c it involves looping over all the neighboring data for
  //each of the features being compared.  This neighbor checking section could be abstracted to
  //types other than parks, if desired.

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
  const bool poiIsSport = _isType(poi, "sport");
  LOG_VART(poiIsSport);
  const bool poiContainedInParkPoly =
    poiContainedInAnotherParkPoly || (polyIsPark && _distance == 0);
  LOG_VART(poiContainedInParkPoly);

  const bool poiIsBuilding = buildingCrit.isSatisfied(poi);
  LOG_VART(poiIsBuilding);

  //This will check the current poi against all neighboring pois.  If any neighboring poi is
  //closer to the current poly than the current poi, then the current poi will not be matched or
  //reviewed against the current poly.
  if (_keepClosestMatchesOnly && _poiNeighborIsCloserToPolyThanPoi(poi, poly))
  {
    LOG_TRACE("Returning miss per review reduction rule #25...");
    return true;
  }

  LOG_VART(_polyNeighborIds.size());
  //LOG_VART(_polyNeighborIds);
  // This loop becomes more expensive as the search radius is increased.
  for (std::set<ElementId>::const_iterator polyNeighborItr = _polyNeighborIds.begin();
       polyNeighborItr != _polyNeighborIds.end(); ++polyNeighborItr)
  {
    ConstElementPtr polyNeighbor = _map->getElement(*polyNeighborItr);
    if (polyNeighbor->getElementId() != poly->getElementId())
    {
      //This will check the current poly against all neighboring polys.  If any neighboring
      //poly is closer to the current poi than the current poly, then the current poi will
      //not be matched or reviewed against the current poly.
      if (_keepClosestMatchesOnly && poly->getElementType() == ElementType::Way &&
          polyNeighbor->getElementType() == ElementType::Way)
      {
        LOG_VART(poly);
        LOG_VART(polyNeighbor);

        const double poiToNeighborPolyDist =
          _getPolyToPointDistance(
            std::dynamic_pointer_cast<const Way>(polyNeighbor),
            std::dynamic_pointer_cast<const Node>(poi));
        LOG_VART(poiToNeighborPolyDist);
        if (poiToNeighborPolyDist != -1.0 && _distance > poiToNeighborPolyDist)
        {
          LOG_TRACE("Returning miss per review reduction rule #26...");
          return true;
        }
      }

      if (_isType(polyNeighbor, "park"))
      {
        // Should this be OsmSchema::elementHasName instead?
        otherParkPolyHasName = !polyNeighbor->getTags().get("name").trimmed().isEmpty();
        otherParkPolyNameScore = _getNameScore(poi, polyNeighbor);
        otherParkPolyNameMatch = otherParkPolyNameScore >= _nameScoreThreshold;

        if (_polyContainsPoi(
              std::dynamic_pointer_cast<const Way>(polyNeighbor),
              std::dynamic_pointer_cast<const Node>(poi)))
        {
          poiContainedInAnotherParkPoly = true;
          LOG_TRACE(
            "poi examined and found to be contained within a park poly outside of this match " <<
            "comparison: " << poi->toString());
          LOG_TRACE("park poly it is very close to: " << polyNeighbor->toString());
        }

        if (_elementIntersectsElement(polyNeighbor, poly))
        {
          parkPolyAngleHistVal = _getAngleHistogramVal(polyNeighbor, poly);
          LOG_VART(parkPolyAngleHistVal);
          parkPolyOverlapVal = _getOverlapVal(polyNeighbor, poly);
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
              poiToPolyNodeDist =
                _getPolyToPointDistance(
                  std::dynamic_pointer_cast<const Way>(poly),
                  std::dynamic_pointer_cast<const Node>(poi));
              poiToOtherParkPolyNodeDist =
                _getPolyToPointDistance(
                  std::dynamic_pointer_cast<const Way>(polyNeighbor),
                  std::dynamic_pointer_cast<const Node>(poi));
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
        if (_isType(polyNeighbor, "sport") &&
            _polyContainsPoi(
              std::dynamic_pointer_cast<const Way>(polyNeighbor),
              std::dynamic_pointer_cast<const Node>(poi)) &&
            _getTypeScore(poi, polyNeighbor) >= _typeScoreThreshold)
        {
          sportPoiOnOtherSportPolyWithTypeMatch = true;
        }
      }
      else if (_hasCrit(polyNeighbor, QString::fromStdString(BuildingCriterion::className())))
      {
        if (_polyContainsPoi(
              std::dynamic_pointer_cast<const Way>(polyNeighbor),
              std::dynamic_pointer_cast<const Node>(poi)))
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
      if ((poiIsPark || poiIsParkish) && polyVeryCloseToAnotherParkPoly && _distance == 0 &&
          poiToOtherParkPolyNodeDist < poiToPolyNodeDist && poiToPolyNodeDist != DBL_MAX &&
          poiToPolyNodeDist != -1.0 && poiToOtherParkPolyNodeDist != DBL_MAX &&
          poiToOtherParkPolyNodeDist != -1.0 && otherParkPolyHasName && parkPolyOverlapVal < 0.9)
      {
        LOG_TRACE("Returning miss per review reduction rule #27...");
        return true;
      }

      //If the poi is a park, the poly it is being compared to is not a park or building, and that
      //poly is "very close" to another park poly that has a name match with the poi, then
      //declare a miss (exclude poly playgrounds from this).
      if (poiIsPark && !polyIsPark && !polyIsBuilding && polyVeryCloseToAnotherParkPoly &&
          otherParkPolyNameMatch)
      {
        LOG_TRACE("Returning miss per review reduction rule #28...");
        return true;
      }

      //If a park poi is contained within one park poly, then there's no reason for it to trigger
      //reviews in another one that its not contained in.
      if (poiIsPark && polyIsPark && _distance > 0 && poiContainedInAnotherParkPoly)
      {
        LOG_TRACE("Returning miss per review reduction rule #29...");
        return true;
      }

      //If a sport poi is contained within a type match sport poi poly, don't let it be
      //matched against anything else.
      if (poiIsSport && poiContainedInParkPoly && sportPoiOnOtherSportPolyWithTypeMatch)
      {
        LOG_TRACE("Returning miss per review reduction rule #30...");
        return true;
      }

      //If a poi is like and on top of a building, don't review it against a non-building poly.
      if (poiIsBuilding && poiOnBuilding && !polyIsBuilding)
      {
        LOG_TRACE("Returning miss per review reduction rule #31...");
        return true;
      }

      //If the poi is not a park and being compared to a park polygon or a polygon that is
      //"very close" to another park poly, we want to be more restrictive on type matching, but
      //only if the poi has any type at all.  If the poi has no type, then behave as normal.
      //Also, let an exact name match cause a review here, rather than a miss.
      if ((polyIsPark || (polyVeryCloseToAnotherParkPoly && !polyHasType)) &&
          !polyHasMoreThanOneType && !poiIsPark && !poiIsParkish && poiHasType && _exactNameMatch)
      {
        LOG_TRACE("Returning miss per review reduction rule #32...");
        return true;
      }
    }
  }

  return false;
}

bool PoiPolygonReviewReducer::_poiNeighborIsCloserToPolyThanPoi(ConstElementPtr poi,
                                                                ConstElementPtr poly)
{
  LOG_VART(_poiNeighborIds.size());
  //LOG_VART(_poiNeighborIds);
  for (std::set<ElementId>::const_iterator poiNeighborItr = _poiNeighborIds.begin();
       poiNeighborItr != _poiNeighborIds.end(); ++poiNeighborItr)
  {
    ConstElementPtr poiNeighbor = _map->getElement(*poiNeighborItr);
    if (poiNeighbor->getElementId() != poi->getElementId())
    {
      long neighborPoiToPolyDist = -1.0;
      // This only handles ways as polygons and not relations. See #3474.
      ConstWayPtr polyWay = std::dynamic_pointer_cast<const Way>(poly);
      if (polyWay)
      {
        neighborPoiToPolyDist =
          _getPolyToPointDistance(polyWay, std::dynamic_pointer_cast<const Node>(poiNeighbor));
      }
      LOG_VART(neighborPoiToPolyDist);
      if (neighborPoiToPolyDist != -1.0 && _distance > neighborPoiToPolyDist)
      {
        return true;
      }
    }
  }

  return false;
}

void PoiPolygonReviewReducer::printCacheInfo()
{
  // TODO: change back
  LOG_INFO("Cache info:");
  for (QMap<QString, int>::const_iterator numCacheHitsByCacheTypeItr = _numCacheHitsByCacheType.begin();
       numCacheHitsByCacheTypeItr != _numCacheHitsByCacheType.end(); ++numCacheHitsByCacheTypeItr)
  {
    const QString line =
      QString("%1:\t%2 hits     size: %3")
        .arg(numCacheHitsByCacheTypeItr.key())
        .arg(StringUtils::formatLargeNumber(numCacheHitsByCacheTypeItr.value()))
        .arg(
          StringUtils::formatLargeNumber(
            _cacheSizeByCacheType[numCacheHitsByCacheTypeItr.key()]));
    LOG_INFO(line);
  }
}

void PoiPolygonReviewReducer::_incrementCacheHitCount(const QString& cacheTypeKey)
{
  if (!_numCacheHitsByCacheType.contains(cacheTypeKey))
  {
    _numCacheHitsByCacheType[cacheTypeKey] = 1;
  }
  else
  {
    _numCacheHitsByCacheType[cacheTypeKey] = _numCacheHitsByCacheType[cacheTypeKey] + 1;
  }
}

void PoiPolygonReviewReducer::_incrementCacheSizeCount(const QString& cacheTypeKey)
{
  if (!_cacheSizeByCacheType.contains(cacheTypeKey))
  {
    _cacheSizeByCacheType[cacheTypeKey] = 1;
  }
  else
  {
    _cacheSizeByCacheType[cacheTypeKey] = _cacheSizeByCacheType[cacheTypeKey] + 1;
  }
}


std::shared_ptr<geos::geom::Geometry> PoiPolygonReviewReducer::_getGeometry(ConstElementPtr element)
{
  if (!element)
  {
    return std::shared_ptr<geos::geom::Geometry>();
  }

  if (_geometryCache.contains(element->getElementId()))
  {
    std::shared_ptr<geos::geom::Geometry> cachedGeom = _geometryCache[element->getElementId()];
    _incrementCacheHitCount("geometry");
    LOG_TRACE("Found geometry in cache for: " << element->getElementId());
    return cachedGeom;
  }
  else
  {
    LOG_TRACE("No geometry in cache for: " << element->getElementId());
    std::shared_ptr<geos::geom::Geometry> newGeom;
    try
    {
      newGeom = ElementConverter(_map).convertToGeometry(element);
    }
    catch (const geos::util::TopologyException& e)
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE(
          "Feature passed to PoiPolygonReviewReducer caused topology exception on conversion to a " <<
          "geometry: " << element->toString() << "\n" << e.what());
        _badGeomCount++;
      }
    }
    LOG_VART(newGeom.get());
    if (newGeom.get() &&
        QString::fromStdString(newGeom->toString()).toUpper().contains("EMPTY"))
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE("Invalid element passed to PoiPolygonReviewReducer: " << newGeom->toString());
        _badGeomCount++;
      }
      newGeom.reset();
    }
    LOG_VART(newGeom.get());
    //if (_geometryCache.size() <= MAX_CACHE_SIZE)
    //{
      _geometryCache[element->getElementId()] = newGeom;
      LOG_TRACE("Updated geometry cache for: " << element->getElementId());
      _incrementCacheSizeCount("geometry");
    //}
    return newGeom;
  }
}

std::shared_ptr<geos::geom::LineString> PoiPolygonReviewReducer::_getLineString(ConstWayPtr poly)
{
  if (!poly)
  {
    return std::shared_ptr<geos::geom::LineString>();
  }

  if (_lineStringCache.contains(poly->getElementId()))
  {
    std::shared_ptr<geos::geom::LineString> cachedGeom = _lineStringCache[poly->getElementId()];
    _incrementCacheHitCount("linestring");
    LOG_TRACE("Found line string in cache for: " << poly->getElementId());
    return cachedGeom;
  }
  else
  {
    LOG_TRACE("No line string in cache for: " << poly->getElementId());
    std::shared_ptr<geos::geom::LineString> newGeom;
    try
    {
      newGeom = ElementConverter(_map).convertToLineString(poly);
    }
    catch (const geos::util::TopologyException& e)
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE(
          "Feature passed to PoiPolygonReviewReducer caused topology exception on conversion to a " <<
          "geometry: " << poly->toString() << "\n" << e.what());
        _badGeomCount++;
      }
    }
    LOG_VART(newGeom.get());
    if (newGeom.get() &&
        QString::fromStdString(newGeom->toString()).toUpper().contains("EMPTY"))
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE("Invalid element passed to PoiPolygonReviewReducer: " << newGeom->toString());
        _badGeomCount++;
      }
      newGeom.reset();
    }
    LOG_VART(newGeom.get());
    //if (_lineStringCache.size() <= MAX_CACHE_SIZE)
    //{
      _lineStringCache[poly->getElementId()] = newGeom;
      LOG_TRACE("Updated line string cache for: " << poly->getElementId());
      _incrementCacheSizeCount("linestring");
    //}
    return newGeom;
  }
}

double PoiPolygonReviewReducer::_getPolyToPointDistance(ConstWayPtr poly, ConstNodePtr point)
{
  if (!poly || !point)
  {
    return -1.0;
  }

  const QString key =
    poly->getElementId().toString() + ";" + point->getElementId().toString();
  if (_elementDistanceCache.contains(key))
  {
    const double cachedDistance = _elementDistanceCache[key];
    _incrementCacheHitCount("distance");
    LOG_TRACE("Found distance of " << cachedDistance << " in cache for: " << key);
    return cachedDistance;
  }
  else
  {
    LOG_TRACE("No distance in cache for: " << key);
    std::shared_ptr<geos::geom::LineString> polyGeom = _getLineString(poly);
    std::shared_ptr<geos::geom::Geometry> pointGeom = _getGeometry(point);
    LOG_VART(polyGeom.get());
    LOG_VART(pointGeom.get());
    double distance = -1.0;
    if (polyGeom && pointGeom)
    {
      distance = polyGeom->distance(pointGeom.get());
    }
    _elementDistanceCache[key] = distance;
    LOG_TRACE("Updated distance cache with: " << distance << " for: " << key);
    _incrementCacheSizeCount("distance");
    return distance;
  }
}

double PoiPolygonReviewReducer::_getArea(ConstWayPtr poly)
{
  if (!poly)
  {
    return -1.0;
  }

  if (_elementAreaCache.contains(poly->getElementId()))
  {
    const double cachedArea = _elementAreaCache[poly->getElementId()];
    _incrementCacheHitCount("area");
    LOG_TRACE("Found area of " << cachedArea << " in cache for: " << poly->getElementId());
    return cachedArea;
  }
  else
  {
    LOG_TRACE("No area in cache for: " << poly->getElementId());
    std::shared_ptr<geos::geom::Geometry> polyGeom = _getGeometry(poly);
    LOG_VART(polyGeom.get());
    double area = -1.0;
    if (polyGeom)
    {
      area = polyGeom->getArea();
    }
    _elementAreaCache[poly->getElementId()] = area;
    LOG_TRACE("Updated area cache with: " << area << " for: " << poly->getElementId());
    _incrementCacheSizeCount("area");
    return area;
  }
}

bool PoiPolygonReviewReducer::_polyContainsPoi(ConstWayPtr poly, ConstNodePtr point)
{
  if (!poly || !point)
  {
    return false;
  }

  const QString key =
    poly->getElementId().toString() + ";" + point->getElementId().toString();
  if (_elementContainsCache.contains(key))
  {
    const bool cachedContains = _elementContainsCache[key];
    _incrementCacheHitCount("contains");
    LOG_TRACE("Found contains=" << cachedContains << " in cache for: " << key);
    return cachedContains;
  }
  else
  {
    LOG_TRACE("No contains in cache for: " << key);
    std::shared_ptr<geos::geom::LineString> polyGeom = _getLineString(poly);
    std::shared_ptr<geos::geom::Geometry> pointGeom = _getGeometry(point);
    LOG_VART(polyGeom.get());
    LOG_VART(pointGeom.get());
    bool contains = false;
    if (polyGeom && pointGeom)
    {
      contains = polyGeom->contains(pointGeom.get());
    }
    _elementContainsCache[key] = contains;
    LOG_TRACE("Updated contains cache with: " << contains << " for: " << key);
    _incrementCacheSizeCount("contains");
    return contains;
  }
}

bool PoiPolygonReviewReducer::_elementIntersectsElement(
  ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return false;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  if (_elementIntersectsCache.contains(key))
  {
    const bool cachedIntersects = _elementIntersectsCache[key];
    _incrementCacheHitCount("intersects");
    LOG_TRACE("Found intersects=" << cachedIntersects << " in cache for: " << key);
    return cachedIntersects;
  }
  else
  {
    LOG_TRACE("No intersects in cache for: " << key);
    std::shared_ptr<geos::geom::Geometry> geom1 = _getGeometry(element1);
    std::shared_ptr<geos::geom::Geometry> geom2 = _getGeometry(element2);
    LOG_VART(geom1.get());
    LOG_VART(geom2.get());
    bool intersects = false;
    if (geom1 && geom2)
    {
      intersects = geom1->intersects(geom2.get());
    }
    _elementIntersectsCache[key] = intersects;
    LOG_TRACE("Updated intersects cache with: " << intersects << " for: " << key);
    _incrementCacheSizeCount("intersects");
    return intersects;
  }
}

double PoiPolygonReviewReducer::_getAngleHistogramVal(
  ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return -1.0;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  if (_elementAngleHistogramCache.contains(key))
  {
    const double cachedAngleHistVal = _elementAngleHistogramCache[key];
    _incrementCacheHitCount("angleHist");
    LOG_TRACE("Found angle hist: " << cachedAngleHistVal << " in cache for: " << key);
    return cachedAngleHistVal;
  }
  else
  {
    LOG_TRACE("No angle hist in cache for: " << key);
    const double angleHistVal = AngleHistogramExtractor().extract(*_map, element1, element2);
    _elementAngleHistogramCache[key] = angleHistVal;
    LOG_TRACE("Updated angle hist cache with: " << angleHistVal << " for: " << key);
    _incrementCacheSizeCount("angleHist");
    return angleHistVal;
  }
}

double PoiPolygonReviewReducer::_getOverlapVal(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return -1.0;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  if (_elementOverlapCache.contains(key))
  {
    const double cachedOverlapVal = _elementOverlapCache[key];
    _incrementCacheHitCount("overlap");
    LOG_TRACE("Found overlap: " << cachedOverlapVal << " in cache for: " << key);
    return cachedOverlapVal;
  }
  else
  {
    LOG_TRACE("No overlap in cache for: " << key);
    const double overlapVal = OverlapExtractor().extract(*_map, element1, element2);
    _elementOverlapCache[key] = overlapVal;
    LOG_TRACE("Updated overlap cache with: " << overlapVal << " for: " << key);
    _incrementCacheSizeCount("overlap");
    return overlapVal;
  }
}

double PoiPolygonReviewReducer::_getNameScore(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return -1.0;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  if (_nameScoreCache.contains(key))
  {
    const double cachedNameScore = _nameScoreCache[key];
    _incrementCacheHitCount("name");
    LOG_TRACE("Found name score: " << cachedNameScore << " in cache for: " << key);
    return cachedNameScore;
  }
  else
  {
    LOG_TRACE("No name score in cache for: " << key);
    const double nameScore = PoiPolygonNameScoreExtractor().extract(*_map, element1, element2);
    _nameScoreCache[key] = nameScore;
    LOG_TRACE("Updated name score cache with: " << nameScore << " for: " << key);
    _incrementCacheSizeCount("name");
    return nameScore;
  }
}

double PoiPolygonReviewReducer::_getTypeScore(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return -1.0;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  if (_typeScoreCache.contains(key))
  {
    const double cachedTypeScore = _typeScoreCache[key];
    _incrementCacheHitCount("type");
    LOG_TRACE("Found type score: " << cachedTypeScore << " in cache for: " << key);
    return cachedTypeScore;
  }
  else
  {
    LOG_TRACE("No type score in cache for: " << key);
    const double typeScore = PoiPolygonTypeScoreExtractor().extract(*_map, element1, element2);
    _typeScoreCache[key] = typeScore;
    LOG_TRACE("Updated type score cache with: " << typeScore << " for: " << key);
    _incrementCacheSizeCount("type");
    return typeScore;
  }
}

bool PoiPolygonReviewReducer::_isType(ConstElementPtr element, const QString& type)
{
  if (!element || type.trimmed().isEmpty())
  {
    return false;
  }

  const QString key = element->getElementId().toString() + ";" + type;
  if (_isTypeCache.contains(key))
  {
    const bool cachedIsType = _isTypeCache[key];
    _incrementCacheHitCount("isType");
    LOG_TRACE("Found is type: " << cachedIsType << " in cache for: " << key);
    return cachedIsType;
  }
  else
  {
    LOG_TRACE("No is type in cache for: " << key);
    // A redesign is need to make this less maintenance prone.
    bool isType = false;
    if (type == "park")
    {
      isType = PoiPolygonTypeScoreExtractor::isPark(element);
    }
    else if (type == "sport")
    {
      isType = PoiPolygonTypeScoreExtractor::isSport(element);
    }
    else if (type == "restaurant")
    {
      isType = PoiPolygonTypeScoreExtractor::isRestaurant(element);
    }
    else if (type == "specificSchool")
    {
      isType = PoiPolygonTypeScoreExtractor::isSpecificSchool(element);
    }
    else if (type == "natural")
    {
      isType = PoiPolygonTypeScoreExtractor::isNatural(element);
    }
    else if (type == "restroom")
    {
      isType = PoiPolygonTypeScoreExtractor::isRestroom(element);
    }
    else if (type == "parking")
    {
      isType = PoiPolygonTypeScoreExtractor::isParking(element);
    }
    else if (type == "school")
    {
      isType = PoiPolygonTypeScoreExtractor::isSchool(element);
    }
    else if (type == "playground")
    {
      isType = PoiPolygonTypeScoreExtractor::isPlayground(element);
    }
    else if (type == "parkish")
    {
      isType = PoiPolygonTypeScoreExtractor::isParkish(element);
    }
    else
    {
      throw IllegalArgumentException("TODO");
    }
    _isTypeCache[key] = isType;
    LOG_TRACE("Updated isType cache with: " << isType << " for: " << key);
    _incrementCacheSizeCount("isType");
    return isType;
  }
}

bool PoiPolygonReviewReducer::_hasCrit(ConstElementPtr element, const QString& criterionClassName)
{
  if (!element || criterionClassName.trimmed().isEmpty())
  {
    return false;
  }

  const QString key = element->getElementId().toString() + ";" + criterionClassName;
  if (_hasCriterionCache.contains(key))
  {
    const bool cachedHasCrit = _hasCriterionCache[key];
    _incrementCacheHitCount("hasCrit");
    LOG_TRACE("Found is has crit: " << cachedHasCrit << " in cache for: " << key);
    return cachedHasCrit;
  }
  else
  {
    LOG_TRACE("No has crit in cache for: " << key);
    ElementCriterionPtr crit = _getCrit(criterionClassName);
    const bool hasCrit = crit->isSatisfied(element);

    _hasCriterionCache[key] = hasCrit;
    LOG_TRACE("Updated has crit cache with: " << hasCrit << " for: " << key);
    _incrementCacheSizeCount("hasCrit");
    return hasCrit;
  }
}

ElementCriterionPtr PoiPolygonReviewReducer::_getCrit(const QString& criterionClassName)
{
  if (criterionClassName.trimmed().isEmpty())
  {
    return ElementCriterionPtr();
  }

  if (_criterionCache.contains(criterionClassName))
  {
    return _criterionCache[criterionClassName];
  }
  else
  {
    ElementCriterionPtr crit =
      ElementCriterionPtr(
        Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
    _criterionCache[criterionClassName] = crit;
    return crit;
  }
}

bool PoiPolygonReviewReducer::_hasType(ConstElementPtr element)
{
  if (!element)
  {
    return false;
  }

  if (_hasTypeCache.contains(element->getElementId()))
  {
    const bool cachedHasType = _hasTypeCache[element->getElementId()];
    _incrementCacheHitCount("hasType");
    LOG_TRACE("Found has type: " << cachedHasType << " in cache for: " << element->getElementId());
    return cachedHasType;
  }
  else
  {
    LOG_TRACE("No has type in cache for: " << element->getElementId());
    const bool hasType = PoiPolygonTypeScoreExtractor::hasType(element);
    _hasTypeCache[element->getElementId()] = hasType;
    LOG_TRACE("Updated has type cache with: " << hasType << " for: " << element->getElementId());
    _incrementCacheSizeCount("hasType");
    return hasType;
  }
}

bool PoiPolygonReviewReducer::_hasMoreThanOneType(ConstElementPtr element)
{
  if (!element)
  {
    return false;
  }

  if (_hasMoreThanOneTypeCache.contains(element->getElementId()))
  {
    const bool cachedHasMoreThanOneType = _hasMoreThanOneTypeCache[element->getElementId()];
    _incrementCacheHitCount("hasMoreThanOneType");
    LOG_TRACE(
      "Found has type: " << cachedHasMoreThanOneType << " in cache for: " << element->getElementId());
    return cachedHasMoreThanOneType;
  }
  else
  {
    LOG_TRACE("No has more than one type in cache for: " << element->getElementId());
    const bool hasMoreThanOneType = PoiPolygonTypeScoreExtractor::hasMoreThanOneType(element);
    _hasMoreThanOneTypeCache[element->getElementId()] = hasMoreThanOneType;
    LOG_TRACE(
      "Updated has more than one type cache with: " << hasMoreThanOneType << " for: " <<
      element->getElementId());
    _incrementCacheSizeCount("hasMoreThanOneType");
    return hasMoreThanOneType;
  }
}

bool PoiPolygonReviewReducer::_specificSchoolMatch(ConstElementPtr element1,
                                                   ConstElementPtr element2)
{
  // We could also check for school type here to move it out of triggersRule.
  if (!element1 || !element2)
  {
    return false;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  if (_specificSchoolMatchCache.contains(key))
  {
    const bool cachedSpecificSchoolMatch = _specificSchoolMatchCache[key];
    _incrementCacheHitCount("specificSchoolMatch");
    LOG_TRACE(
      "Found specific school match:" << cachedSpecificSchoolMatch << " in cache for: " << key);
    return cachedSpecificSchoolMatch;
  }
  else
  {
    LOG_TRACE("No specific school match in cache for: " << key);
    const bool specificSchoolMatch =
      PoiPolygonTypeScoreExtractor::specificSchoolMatch(element1, element2);
    _specificSchoolMatchCache[key] = specificSchoolMatch;
    LOG_TRACE(
      "Updated specific school match cache with: " << specificSchoolMatch << " for: " << key);
    _incrementCacheSizeCount("specificSchoolMatch");
    return specificSchoolMatch;
  }
}

int PoiPolygonReviewReducer::_getNumAddresses(ConstElementPtr element)
{
  if (!element)
  {
    return false;
  }

  if (_numAddressesCache.contains(element->getElementId()))
  {
    const int cachedNumAddresses = _numAddressesCache[element->getElementId()];
    _incrementCacheHitCount("numAddresses");
    LOG_TRACE(
      "Found num addresses: " << cachedNumAddresses << " in cache for: " <<
      element->getElementId());
    return cachedNumAddresses;
  }
  else
  {
    LOG_TRACE("No num addresses in cache for: " << element->getElementId());
    const int numAddresses = _addressParser.numAddressesRecursive(element, *_map);
    _numAddressesCache[element->getElementId()] = numAddresses;
    LOG_TRACE(
      "Updated num addresses cache with: " << numAddresses << " for: " << element->getElementId());
    _incrementCacheSizeCount("numAddresses");
    return numAddresses;
  }
}

bool PoiPolygonReviewReducer::_hasAddress(ConstElementPtr element)
{
  return _getNumAddresses(element) > 0;
}

}
