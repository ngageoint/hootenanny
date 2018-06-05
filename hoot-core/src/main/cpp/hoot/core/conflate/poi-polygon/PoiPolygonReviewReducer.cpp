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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonReviewReducer.h"

// geos
#include <geos/geom/LineString.h>
#include <geos/util/TopologyException.h>
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/filters/BuildingWayNodeCriterion.h>
#include <hoot/core/util/ConfigOptions.h>

#include "extractors/PoiPolygonTypeScoreExtractor.h"
#include "extractors/PoiPolygonNameScoreExtractor.h"
#include "extractors/PoiPolygonAddressScoreExtractor.h"

#include <float.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

PoiPolygonReviewReducer::PoiPolygonReviewReducer(const ConstOsmMapPtr& map,
                                                 const set<ElementId>& polyNeighborIds,
                                                 const set<ElementId>& poiNeighborIds,
                                                 double distance, double nameScoreThreshold,
                                                 double nameScore, bool nameMatch,
                                                 bool exactNameMatch, double typeScoreThreshold,
                                                 double typeScore, bool typeMatch,
                                                 double matchDistanceThreshold, bool addressMatch) :
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
_keepClosestMatchesOnly(ConfigOptions().getPoiPolygonKeepClosestMatchesOnly())
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

  _genericLandUseTagVals.append("cemetery");
  _genericLandUseTagVals.append("commercial");
  //_genericLandUseTagVals.append("construction");
  _genericLandUseTagVals.append("farm");
  _genericLandUseTagVals.append("forest");
  _genericLandUseTagVals.append("grass");
  _genericLandUseTagVals.append("industrial");
  _genericLandUseTagVals.append("meadow");
  _genericLandUseTagVals.append("residential");
  _genericLandUseTagVals.append("retail");
  _genericLandUseTagVals.append("village_green");
}

bool PoiPolygonReviewReducer::_nonDistanceSimilaritiesPresent() const
{
  return _typeScore > 0.03 || _nameScore > 0.35 || _addressMatch;
}

bool PoiPolygonReviewReducer::triggersRule(ConstElementPtr poi, ConstElementPtr poly)
{
  LOG_DEBUG("Checking review reduction rules...");

  //TODO: Many of these rules may be obsolete now after recent additions.

  //The rules below are *roughly* ordered by increasing processing expense and by decreasing
  //likelihood of occurrence (probably needs some reordering at this point).

  const bool poiHasType = PoiPolygonTypeScoreExtractor::hasType(poi);
  LOG_VART(poiHasType);
  const bool polyHasType = PoiPolygonTypeScoreExtractor::hasType(poly);
  LOG_VART(polyHasType);

  //if both have addresses and they explicitly contradict each other, throw out the review; don't
  //do it if the poly has more than one address, like in many multi-use buildings
  if (!_addressMatch && PoiPolygonAddressScoreExtractor::elementHasAddress(poi, *_map) &&
      PoiPolygonAddressScoreExtractor::getAddresses(poly, *_map).size() == 1)
  {
    LOG_TRACE("Returning miss per review reduction rule #1...");
    return true;
  }

  if (OsmSchema::getInstance().isMultiUse(*poly) && poiHasType && _typeScore < 0.4)
  {
    LOG_TRACE("Returning miss per review reduction rule #2...");
    return true;
  }

  //be a little stricter on place related reviews.
  if ((poi->getTags().get("place").toLower() == "neighbourhood" ||
       poi->getTags().get("place").toLower() == "suburb") && !poly->getTags().contains("place"))
  {
    LOG_TRACE("Returning miss per review reduction rule #3...");
    return true;
  }

  //TODO: this one will go away
  if (poi->getTags().get("man_made").toLower() == "mine" &&
      poly->getTags().get("landuse").toLower() == "quarry" &&
      poly->getTags().get("man_made").toLower() != "mine")
  {
    LOG_TRACE("Returning miss per review reduction rule #4...");
    return true;
  }

  //points sitting on islands need to have an island type, or the match doesn't make any sense
  if ((poi->getTags().get("place").toLower() == "island" ||
       poly->getTags().get("place").toLower() == "island") && !_typeMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #5...");
    return true;
  }

  const bool polyIsPark = PoiPolygonTypeScoreExtractor::isPark(poly);
  LOG_VART(polyIsPark);

  //similar to above, but for gardens
  if ((poi->getTags().get("leisure").toLower() == "garden" ||
       poly->getTags().get("leisure").toLower() == "garden") &&
       (!_nonDistanceSimilaritiesPresent() ||
        (polyIsPark &&
         !PoiPolygonNameScoreExtractor::getElementName(poly).toLower().contains("garden"))))
  {
    LOG_TRACE("Returning miss per review reduction rule #6...");
    return true;
  }

  //lots of things have fountains, so let's raise the requirements a bit
  if (poi->getTags().get("amenity") == "fountain" && _nameScore < 0.5)
  {
    LOG_TRACE("Returning miss per review reduction rule #7...");
    return true;
  }

  //these seem to be clustered together tightly a lot in cities, so up the requirement a bit
  //TODO: using custom match/review distances or custom score requirements may be a better way to
  //handle these types
  const bool poiHasName = !PoiPolygonNameScoreExtractor::getElementName(poi).isEmpty();
  const bool polyHasName = !PoiPolygonNameScoreExtractor::getElementName(poly).isEmpty();
  if (poi->getTags().get("tourism") == "hotel" && poly->getTags().get("tourism") == "hotel" &&
      poiHasName && polyHasName && _nameScore < 0.75 && !_addressMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #8...");
    return true;
  }
  //this is too destructive
//  if (PoiPolygonTypeScoreExtractor::isSchool(poi) && PoiPolygonTypeScoreExtractor::isSchool(poly) &&
//      poiHasName && polyHasName && _nameScore < 0.5 && !_addressMatch)
//  {
//    LOG_TRACE("Returning miss per review reduction rule #5d...");
//    return true;
//  }
  if (PoiPolygonTypeScoreExtractor::isRestaurant(poi) &&
      PoiPolygonTypeScoreExtractor::isRestaurant(poly) && poiHasName && polyHasName &&
      _nameScore < 0.5 && !_addressMatch)
  {
    LOG_TRACE("Returning miss per review reduction rule #9...");
    return true;
  }

  //similar to above, but for sport fields
  const bool poiNameEndsWithField =
    PoiPolygonNameScoreExtractor::getElementName(poi).toLower().endsWith("field");
  LOG_VART(poiNameEndsWithField);
  const bool polyIsSport = PoiPolygonTypeScoreExtractor::isSport(poly);
  LOG_VART(polyIsSport);
  //we'll let this review pass if the poi ends with "field" in the name and is sitting on top of a
  //sport poly
  if (poiNameEndsWithField && polyIsSport)
  {
  }
  else if ((poi->getTags().get("leisure").toLower() == "pitch" ||
            poly->getTags().get("leisure").toLower() == "pitch") &&
           !_nonDistanceSimilaritiesPresent())
  {   
    LOG_TRACE("Returning miss per review reduction rule #10...");
    return true;
  }

  //Landuse polys often wrap a bunch of other features and don't necessarily match to POI's, so
  //be more strict with their reviews.
  if (_genericLandUseTagVals.contains(poly->getTags().get("landuse")) &&
      !_nonDistanceSimilaritiesPresent())
  {
    LOG_TRACE("Returning miss per review reduction rule #11...");
    return true;
  }

  LOG_VART(PoiPolygonTypeScoreExtractor::isSpecificSchool(poi));
  LOG_VART(PoiPolygonTypeScoreExtractor::isSpecificSchool(poly));
  LOG_VART(PoiPolygonTypeScoreExtractor::specificSchoolMatch(poi, poly));
  if (PoiPolygonTypeScoreExtractor::isSpecificSchool(poi) &&
      PoiPolygonTypeScoreExtractor::isSpecificSchool(poly) &&
      !PoiPolygonTypeScoreExtractor::specificSchoolMatch(poi, poly))
  {
    LOG_TRACE("Returning miss per review reduction rule #12...");
    return true;
  }

  const bool poiIsNatural = poi->getTags().contains("natural");
  const bool polyIsNatural = poly->getTags().contains("natural");

  //Be more strict reviewing natural features and parks against building features.  This could be
  //extended
  if ((polyIsNatural || polyIsPark) &&
      OsmSchema::getInstance().getCategories(
        poi->getTags()).intersects(OsmSchemaCategory::building()))
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
  if (_distance == 0 && polyIsPark && poi->getTags().get("leisure") == "pitch")
  {
    LOG_TRACE("Returning miss per review reduction rule #16...");
    return true;
  }

  const bool poiIsPark = PoiPolygonTypeScoreExtractor::isPark(poi);
  LOG_VART(poiIsPark);

  //Don't match a park poi to a sport poly.  Simpler version of some rules above.
  //may make some previous rules obsolete
  if (poiIsPark && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #17...");
    return true;
  }

  const bool polyIsBuilding = OsmSchema::getInstance().isBuilding(poly);
  LOG_VART(polyIsBuilding);

  //Similar to previous, except more focused for restrooms.
  if (poiHasType && polyHasType && _typeScore != 1.0 &&
      PoiPolygonTypeScoreExtractor::isRestroom(poi) &&
      !OsmSchema::getInstance().getCategories(poly->getTags()).intersects(
        OsmSchemaCategory::building()))
  {
    LOG_TRACE("Returning miss per review reduction rule #18...");
    return true;
  }

  //Be more strict reviewing parking lots against other features.
  if (poiHasType && polyHasType && PoiPolygonTypeScoreExtractor::isParking(poly) &&
      !_typeMatch && poly->getTags().get("parking") != "multi-storey")
  {
    LOG_TRACE("Returning miss per review reduction rule #19...");
    return true;
  }

  //Don't review schools against their sports fields.
  if (PoiPolygonTypeScoreExtractor::isSchool(poi) && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #20...");
    return true;
  }

  //Need to be stricter on tunnels since we don't want above ground things to review against them.
  if (poly->getTags().get("tunnel") == "yes" && poiHasType &&
      (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    LOG_TRACE("Returning miss per review reduction rule #21...");
    return true;
  }

  ElementConverter elementConverter(_map);
  boost::shared_ptr<Geometry> polyGeom = elementConverter.convertToGeometry(poly);
  if (QString::fromStdString(polyGeom->toString()).toUpper().contains("EMPTY"))
  {
    throw geos::util::TopologyException();
  }
  boost::shared_ptr<Geometry> poiGeom = elementConverter.convertToGeometry(poi);

  double polyArea = -1.0;
  try
  {
    polyArea = polyGeom->getArea();
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE(
        "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
        "geometry: " << polyGeom->toString() << "\n" << e.what());
      _badGeomCount++;
    }
  }

  //Don't review park poi's against large non-park polys.
  if (poiHasType && polyHasType && !_nameMatch && !polyIsPark &&
      (((poly->getTags().contains("landuse") && !poi->getTags().contains("landuse")) ||
      (poly->getTags().contains("natural") && !poi->getTags().contains("natural")) ||
      poly->getTags().get("leisure").toLower() == "common") ||
      (poly->getTags().get("place").toLower() == "neighborhood" ||
      poly->getTags().get("place").toLower() == "neighbourhood")) && polyArea > 50000)
  {
    LOG_TRACE("Returning miss per review reduction rule #22...");
    return true;
  }

  const bool polyIsPlayground = PoiPolygonTypeScoreExtractor::isPlayground(poly);
  LOG_VART(polyIsPlayground);
  const bool polyHasMoreThanOneType = PoiPolygonTypeScoreExtractor::hasMoreThanOneType(poly);
  LOG_VART(polyHasMoreThanOneType);
  const bool poiIsParkish = PoiPolygonTypeScoreExtractor::isParkish(poi);
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
  const bool poiIsSport = PoiPolygonTypeScoreExtractor::isSport(poi);
  LOG_VART(poiIsSport);
  const bool poiContainedInParkPoly =
    poiContainedInAnotherParkPoly || (polyIsPark && _distance == 0);
  LOG_VART(poiContainedInParkPoly);

  const bool poiIsBuilding = OsmSchema::getInstance().isBuilding(poi);
  LOG_VART(poiIsBuilding);

  PoiPolygonNameScoreExtractor nameScorer;

  //This will check the current poi against all neighboring pois.  If any neighboring poi is
  //closer to the current poly than the current poi, then the current poi will not be matched or
  //reviewed against the current poly.
  if (_keepClosestMatchesOnly && _poiNeighborIsCloserToPolyThanPoi(poi, poly))
  {
    return true;
  }

  LOG_VART(_polyNeighborIds);
  //The loop becomes more expensive as the search radius is increased.
  for (set<ElementId>::const_iterator polyNeighborItr = _polyNeighborIds.begin();
       polyNeighborItr != _polyNeighborIds.end(); ++polyNeighborItr)
  {
    ConstElementPtr polyNeighbor = _map->getElement(*polyNeighborItr);
    if (polyNeighbor->getElementId() != poly->getElementId())
    {
      boost::shared_ptr<Geometry> polyNeighborGeom;
      try
      {
        polyNeighborGeom = ElementConverter(_map).convertToGeometry(polyNeighbor);

        if (polyNeighborGeom.get() &&
            QString::fromStdString(polyNeighborGeom->toString()).toUpper().contains("EMPTY"))
        {
          if (_badGeomCount <= Log::getWarnMessageLimit())
          {
            LOG_TRACE(
              "Invalid area neighbor polygon passed to PoiPolygonMatchCreator: " <<
              polyNeighborGeom->toString());
            _badGeomCount++;
          }
        }
        else if (polyNeighborGeom.get())
        {

          //This will check the current poly against all neighboring polys.  If any neighboring
          //poly is closer to the current poi than the current poly, then the current poi will
          //not be matched or reviewed against the current poly.
          if (_keepClosestMatchesOnly && poly->getElementType() == ElementType::Way &&
              polyNeighbor->getElementType() == ElementType::Way)
          {
            LOG_VART(poly);
            LOG_VART(polyNeighbor);
            ConstWayPtr polyNeighborWay = boost::dynamic_pointer_cast<const Way>(polyNeighbor);
            boost::shared_ptr<const LineString> polyNeighborLineStr =
              boost::dynamic_pointer_cast<const LineString>(
                ElementConverter(_map).convertToLineString(polyNeighborWay));
            const long poiToNeighborPolyDist = polyNeighborLineStr->distance(poiGeom.get());
            LOG_VART(poiToNeighborPolyDist);
            if (_distance > poiToNeighborPolyDist)
            {
              LOG_TRACE("Returning miss per review reduction rule #25b...");
              return true;
            }
          }

          if (PoiPolygonTypeScoreExtractor::isPark(polyNeighbor))
          {
            //TODO: should this be OsmSchema::elementHasName instead?
            otherParkPolyHasName = !polyNeighbor->getTags().get("name").trimmed().isEmpty();
            otherParkPolyNameScore = nameScorer.extract(*_map, poi, polyNeighbor);
            otherParkPolyNameMatch = otherParkPolyNameScore >= _nameScoreThreshold;

            if (polyNeighborGeom->contains(poiGeom.get()))
            {
              poiContainedInAnotherParkPoly = true;

              LOG_TRACE(
                "poi examined and found to be contained within a park poly outside of this match " <<
                "comparison: " << poi->toString());
              LOG_TRACE("park poly it is very close to: " << polyNeighbor->toString());
            }

            if (polyNeighborGeom->intersects(polyGeom.get()))
            {
              parkPolyAngleHistVal = AngleHistogramExtractor().extract(*_map, polyNeighbor, poly);
              LOG_VART(parkPolyAngleHistVal);
              parkPolyOverlapVal = OverlapExtractor().extract(*_map, polyNeighbor, poly);
              LOG_VART(parkPolyOverlapVal);

              //When just using intersection as the criteria, only found one instance when something
              //was considered as "very close" to a park poly when I didn't want it to be...so these
              //values set very low to weed that instance out...overlap at least.
              if (parkPolyAngleHistVal >= 0.05 && parkPolyOverlapVal >= 0.02)
              {
                polyVeryCloseToAnotherParkPoly = true;

                if (poly->getElementType() == ElementType::Way &&
                    polyNeighbor->getElementType() == ElementType::Way)
                {
                  //Calc the distance from the poi to the poly line instead of the poly itself.
                  //Calcing distance to the poly itself will always return 0 when the poi is in the
                  //poly.
                  ConstWayPtr polyWay = boost::dynamic_pointer_cast<const Way>(poly);
                  boost::shared_ptr<const LineString> polyLineStr =
                    boost::dynamic_pointer_cast<const LineString>(
                      ElementConverter(_map).convertToLineString(polyWay));
                  poiToPolyNodeDist = polyLineStr->distance(poiGeom.get());
                  ConstWayPtr polyNeighborWay = boost::dynamic_pointer_cast<const Way>(polyNeighbor);
                  boost::shared_ptr<const LineString> polyNeighborLineStr =
                    boost::dynamic_pointer_cast<const LineString>(
                      ElementConverter(_map).convertToLineString(polyNeighborWay));
                  poiToOtherParkPolyNodeDist = polyNeighborLineStr->distance(poiGeom.get());
                }
                LOG_TRACE(
                  "poly examined and found very close to a another park poly: " <<
                  poly->toString());
                LOG_TRACE("park poly it is very close to: " << polyNeighbor->toString());
              }
            }
          }
          else if (poiIsSport)
          {
            //this is a little loose, b/c there could be more than one type match set of tags...
            if (PoiPolygonTypeScoreExtractor::isSport(polyNeighbor) &&
                polyNeighborGeom->contains(poiGeom.get()) &&
                typeScorer.extract(*_map, poi, polyNeighbor) >= _typeScoreThreshold)
            {
              sportPoiOnOtherSportPolyWithTypeMatch = true;
            }
          }
          else if (OsmSchema::getInstance().isBuilding(polyNeighbor))
          {
            if (polyNeighborGeom->contains(poiGeom.get()))
            {
              poiOnBuilding = true;
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
        if ((poiIsPark || poiIsParkish) && polyVeryCloseToAnotherParkPoly && _distance == 0 &&
            poiToOtherParkPolyNodeDist < poiToPolyNodeDist && poiToPolyNodeDist != DBL_MAX &&
            poiToOtherParkPolyNodeDist != DBL_MAX && otherParkPolyHasName &&
            parkPolyOverlapVal < 0.9)
        {
          LOG_TRACE("Returning miss per review reduction rule #26...");
          return true;
        }

        //If the poi is a park, the poly it is being compared to is not a park or building, and that
        //poly is "very close" to another park poly that has a name match with the poi, then
        //declare a miss (exclude poly playgrounds from this).
        if (poiIsPark && !polyIsPark && !polyIsBuilding && polyVeryCloseToAnotherParkPoly &&
            //Unfortunately, include polyIsPlayground here blows up too many matches.
            otherParkPolyNameMatch /*&& !polyIsPlayground*/)
        {
          LOG_TRACE("Returning miss per review reduction rule #27...");
          return true;
        }

        //If a park poi is contained within one park poly, then there's no reason for it to trigger
        //reviews in another one that its not contained in.
        if (poiIsPark && polyIsPark && _distance > 0 && poiContainedInAnotherParkPoly)
        {
          LOG_TRACE("Returning miss per review reduction rule #28...");
          return true;
        }

        //If a sport poi is contained within a type match sport poi poly, don't let it be
        //matched against anything else.
        if (poiIsSport && poiContainedInParkPoly && sportPoiOnOtherSportPolyWithTypeMatch)
        {
          LOG_TRACE("Returning miss per review reduction rule #29...");
          return true;
        }

        //If a poi is like and on top of a building, don't review it against a non-building poly.
        if (poiIsBuilding && poiOnBuilding && !polyIsBuilding)
        {
          LOG_TRACE("Returning miss per review reduction rule #30...");
          return true;
        }

        //If the poi is not a park and being compared to a park polygon or a polygon that is
        //"very close" to another park poly, we want to be more restrictive on type matching, but
        //only if the poi has any type at all.  If the poi has no type, then behave as normal.
        //Also, let an exact name match cause a review here, rather than a miss.
        if ((polyIsPark || (polyVeryCloseToAnotherParkPoly && !polyHasType)) &&
            !polyHasMoreThanOneType && !poiIsPark && !poiIsParkish && poiHasType)
        {
          if (!_exactNameMatch)
          {
            LOG_TRACE("Returning miss per review reduction rule #31...");
            return true;
          }
        }
      }
      catch (const geos::util::TopologyException& e)
      {
        if (_badGeomCount <= Log::getWarnMessageLimit())
        {
          LOG_TRACE(
            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
            "geometry: " << polyNeighbor->toString() << "\n" << e.what());
          _badGeomCount++;
        }
      }
    }
  }

  return false;
}

bool PoiPolygonReviewReducer::_poiNeighborIsCloserToPolyThanPoi(ConstElementPtr poi,
                                                                ConstElementPtr poly)
{
  LOG_VART(_poiNeighborIds);
  for (set<ElementId>::const_iterator poiNeighborItr = _poiNeighborIds.begin();
       poiNeighborItr != _poiNeighborIds.end(); ++poiNeighborItr)
  {
    ConstElementPtr poiNeighbor = _map->getElement(*poiNeighborItr);
    if (poiNeighbor->getElementId() != poi->getElementId())
    {
      boost::shared_ptr<Geometry> poiNeighborGeom;
      try
      {
        poiNeighborGeom = ElementConverter(_map).convertToGeometry(poiNeighbor);

        if (poiNeighborGeom.get() &&
            QString::fromStdString(poiNeighborGeom->toString()).toUpper().contains("EMPTY"))
        {
          if (_badGeomCount <= Log::getWarnMessageLimit())
          {
            LOG_TRACE(
              "Invalid neighbor POI passed to PoiPolygonMatchCreator: " <<
              poiNeighborGeom->toString());
            _badGeomCount++;
          }
        }
        else if (poiNeighborGeom.get())
        {
          LOG_VART(poi);
          LOG_VART(poiNeighbor);
          ConstWayPtr polyWay = boost::dynamic_pointer_cast<const Way>(poly);
            boost::shared_ptr<const LineString> polyLineStr =
              boost::dynamic_pointer_cast<const LineString>(
                ElementConverter(_map).convertToLineString(polyWay));
          const long neighborPoiToPolyDist = polyLineStr->distance(poiNeighborGeom.get());
          LOG_VART(neighborPoiToPolyDist);
          if (_distance > neighborPoiToPolyDist)
          {
            LOG_TRACE("Returning miss per review reduction rule #25a...");
            return true;
          }
        }
      }
      catch (const geos::util::TopologyException& e)
      {
        if (_badGeomCount <= Log::getWarnMessageLimit())
        {
          LOG_TRACE(
            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
            "geometry: " << poiNeighbor->toString() << "\n" << e.what());
          _badGeomCount++;
        }
      }
    }
  }

  return false;
}

}
