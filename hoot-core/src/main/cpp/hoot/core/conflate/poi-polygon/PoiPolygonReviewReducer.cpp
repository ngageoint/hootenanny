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
#include "PoiPolygonReviewReducer.h"

// geos
#include <geos/geom/LineString.h>
#include <geos/util/TopologyException.h>
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/conflate/MatchClassification.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
//#include <hoot/core/algorithms/Translator.h>

#include "extractors/PoiPolygonTypeScoreExtractor.h"
#include "extractors/PoiPolygonNameScoreExtractor.h"
#include "extractors/PoiPolygonAddressScoreExtractor.h"

#include <float.h>

namespace hoot
{

PoiPolygonReviewReducer::PoiPolygonReviewReducer(const ConstOsmMapPtr& map,
                                                 const set<ElementId>& polyNeighborIds,
                                                 const set<ElementId>& poiNeighborIds,
                                                 double distance, double nameScoreThreshold,
                                                 bool nameMatch,
                                                 bool exactNameMatch,
                                                 double typeScore,
                                                 bool typeMatch,
                                                 double matchDistanceThreshold) :
_map(map),
_polyNeighborIds(polyNeighborIds),
_poiNeighborIds(poiNeighborIds),
_distance(distance),
_nameScoreThreshold(nameScoreThreshold),
_nameMatch(nameMatch),
_exactNameMatch(exactNameMatch),
_typeScore(typeScore),
_typeMatch(typeMatch),
_matchDistanceThreshold(matchDistanceThreshold),
_badGeomCount(0)
{
  //TODO: can probably get rid of this list and make the logic work against all landuse
  _genericLandUseTagVals.append("cemetery");
  _genericLandUseTagVals.append("commercial");
  _genericLandUseTagVals.append("construction");
  _genericLandUseTagVals.append("farm");
  _genericLandUseTagVals.append("forest");
  _genericLandUseTagVals.append("grass");
  _genericLandUseTagVals.append("industrial");
  _genericLandUseTagVals.append("meadow");
  _genericLandUseTagVals.append("residential");
  _genericLandUseTagVals.append("retail");
  _genericLandUseTagVals.append("village_green");
}

bool PoiPolygonReviewReducer::triggersRule(ConstElementPtr poi, ConstElementPtr poly)
{
  //to suppress the ElementConverter poly warnings...warnings worth looking into at some point
  DisableLog dl(Log::Warn);

  ElementConverter elementConverter(_map);
  shared_ptr<Geometry> polyGeom = elementConverter.convertToGeometry(poly);
  if (QString::fromStdString(polyGeom->toString()).toUpper().contains("EMPTY"))
  {
    throw geos::util::TopologyException();
  }
  shared_ptr<Geometry> poiGeom = elementConverter.convertToGeometry(poi);

  //The rules below are roughly ordered by increasing processing expense and by decreasing
  //likelihood of occurrence.

  //Reduce reviews against some non-use type polys.
  if (poly->getTags().get("natural").toLower() == "coastline" ||
      poi->getTags().get("highway").toLower() == "traffic_signals" ||
      poi->getTags().get("amenity").toLower() == "atm" ||
      poi->getTags().get("man_made").toLower() == "surveillance")
  {
    LOG_TRACE("Returning miss per review reduction rule #1...");
    return true;
  }

  //Be a little stricter on place related reviews.
  if (poi->getTags().get("place").toLower() == "neighbourhood" &&
      !poly->getTags().contains("place"))
  {
    LOG_TRACE("Returning miss per review reduction rule #2...");
    return true;
  }

  const bool poiHasType = PoiPolygonTypeScoreExtractor::hasType(poi);
  const bool polyHasType = PoiPolygonTypeScoreExtractor::hasType(poly);

  //Be more strict reviewing natural features against building features.
  if (poiHasType && polyHasType && poly->getTags().contains("natural") &&
      OsmSchema::getInstance().getCategories(
        poi->getTags()).intersects(OsmSchemaCategory::building()))
  {
    LOG_TRACE("Returning miss per review reduction rule #3...");
    return true;
  }

  //Landuse polys often wrap a bunch of other features and don't necessarily match to POI's, so
  //be more strict with their reviews.
  if (_genericLandUseTagVals.contains(poly->getTags().get("landuse")) &&
      _distance > _matchDistanceThreshold &&
      (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    LOG_TRACE("Returning miss per review reduction rule #4...");
    return true;
  }

  const bool polyIsPark = PoiPolygonTypeScoreExtractor::isPark(poly);
  //prevent athletic POIs within a park poly from being reviewed against that park poly
  if (_distance == 0 && polyIsPark && poi->getTags().get("leisure") == "pitch")
  {
    LOG_TRACE("Returning miss per review reduction rule #5...");
    return true;
  }

  const bool poiIsPark = PoiPolygonTypeScoreExtractor::isPark(poi);
  const bool polyIsSport = PoiPolygonTypeScoreExtractor::isSport(poly);

  //Don't match a park poi to a sport poly.  Simpler version of some rules above.
  //may make some previous rules obsolete
  if (poiIsPark && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #6...");
    return true;
  }

  //The name translations are very expensive here, so have gotten rid of all rules requiring
  //translated names
//  QString polyName = PoiPolygonNameScoreExtractor::getElementName(poi);
//  if (!polyName.isEmpty())
//  {
//    polyName = Translator::getInstance().toEnglish(polyName);
//  }
  //  QString poiName = PoiPolygonNameScoreExtractor::getElementName(poi);
  //  if (!poiName.isEmpty())
  //  {
  //    poiName = Translator::getInstance().toEnglish(poiName);
  //  }

  const bool polyIsBuilding = OsmSchema::getInstance().isBuilding(poly);

  //Similar to previous, except more focused for restrooms.
  if (poiHasType && polyHasType && !_typeScore == 1.0 &&
      PoiPolygonTypeScoreExtractor::isRestroom(poi) &&
      !OsmSchema::getInstance().getCategories(poly->getTags()).intersects(
        OsmSchemaCategory::building()))
  {
    LOG_TRACE("Returning miss per review reduction rule #7...");
    return true;
  }

  //Be more strict reviewing parking lots against other features.
  if (poiHasType && polyHasType && PoiPolygonTypeScoreExtractor::isParking(poly) &&
      _distance > _matchDistanceThreshold &&
      (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    LOG_TRACE("Returning miss per review reduction rule #7...");
    return true;
  }

  //Don't review schools against their sports fields.
  if (PoiPolygonTypeScoreExtractor::isSchool(poi) && polyIsSport)
  {
    LOG_TRACE("Returning miss per review reduction rule #9...");
    return true;
  }

  //Gates and Parking are two separate things.
  if (poi->getTags().get("barrier").toLower() == "gate" &&
      poly->getTags().get("amenity").toLower() == "parking")
  {
    LOG_TRACE("Returning miss per review reduction rule #10...");
    return true;
  }

  //Need to be stricter on tunnels since we don't want above ground things to review against them.
  if (poly->getTags().get("tunnel") == "yes" && poiHasType &&
      (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    LOG_TRACE("Returning miss per review reduction rule #11...");
    return true;
  }

  double polyArea = -1.0;
  try
  {
    polyArea = polyGeom->getArea();
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
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
    LOG_TRACE("Returning miss per review reduction rule #12...");
    return true;
  }

  const bool polyHasMoreThanOneType = PoiPolygonTypeScoreExtractor::hasMoreThanOneType(poly);
  const bool poiIsParkish = PoiPolygonTypeScoreExtractor::isParkish(poi);

  //This is a simple rule to prevent matching poi's not at all like a park with park polys.
  //this may render some of the previous rules obsolete.
  if (!poiIsPark && !poiIsParkish && poiHasType && polyIsPark && !polyHasMoreThanOneType)
  {
    LOG_TRACE("Returning miss per review reduction rule #13...");
    return true;
  }

  bool polyVeryCloseToAnotherParkPoly = false;
  double parkPolyAngleHistVal = -1.0;
  double parkPolyOverlapVal = -1.0;
  bool otherParkPolyNameMatch = false;
  double otherParkPolyNameScore = -1.0;
  double poiToPolyNodeDist = DBL_MAX;
  double poiToOtherParkPolyNodeDist = DBL_MAX;
  bool otherParkPolyHasName = false;
  bool poiContainedInAnotherParkPoly = false;
  bool sportPoiOnOtherSportPolyWithExactTypeMatch = false;
  bool poiOnBuilding = false;
  const bool poiIsSport = PoiPolygonTypeScoreExtractor::isSport(poi);
  const bool poiContainedInParkPoly =
    poiContainedInAnotherParkPoly || (polyIsPark && _distance == 0);
  const bool polyIsPlayground = PoiPolygonTypeScoreExtractor::isPlayground(poly);
  const bool poiIsBuilding = OsmSchema::getInstance().isBuilding(poi);

  PoiPolygonTypeScoreExtractor typeScorer;
  PoiPolygonNameScoreExtractor nameScorer;

  set<ElementId>::const_iterator polyNeighborItr = _polyNeighborIds.begin();
  while (polyNeighborItr != _polyNeighborIds.end())
  {
    ConstElementPtr polyNeighbor = _map->getElement(*polyNeighborItr);
    if (polyNeighbor->getElementId() != poly->getElementId())
    {
      shared_ptr<Geometry> polyNeighborGeom;
      try
      {
        polyNeighborGeom = ElementConverter(_map).convertToGeometry(polyNeighbor);

        if (polyNeighborGeom.get() &&
            QString::fromStdString(polyNeighborGeom->toString()).toUpper().contains("EMPTY"))
        {
          if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
          {
            LOG_TRACE(
              "Invalid area neighbor polygon passed to PoiPolygonMatchCreator: " <<
              polyNeighborGeom->toString());
            _badGeomCount++;
          }
        }
        else if (polyNeighborGeom.get())
        {
          if (PoiPolygonTypeScoreExtractor::isPark(polyNeighbor))
          {
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
              parkPolyOverlapVal = OverlapExtractor().extract(*_map, polyNeighbor, poly);

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
                  ConstWayPtr polyWay = dynamic_pointer_cast<const Way>(poly);
                  shared_ptr<const LineString> polyLineStr =
                      dynamic_pointer_cast<const LineString>(
                        ElementConverter(_map).convertToLineString(polyWay));
                  poiToPolyNodeDist = polyLineStr->distance(poiGeom.get());
                  ConstWayPtr polyNeighborWay = dynamic_pointer_cast<const Way>(polyNeighbor);
                  shared_ptr<const LineString> polyNeighborLineStr =
                      dynamic_pointer_cast<const LineString>(
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
                typeScorer.extract(*_map, poi, polyNeighbor) == 1.0)
            {
              sportPoiOnOtherSportPolyWithExactTypeMatch = true;
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

        //If the POI is inside a poly that is very close to another park poly, declare miss if
        //the distance to the outer ring of the other park poly is shorter than the distance to the
        //outer ring of this poly and the other park poly has a name.
        if ((poiIsPark || poiIsParkish) && polyVeryCloseToAnotherParkPoly && _distance == 0 &&
            poiToOtherParkPolyNodeDist < poiToPolyNodeDist && poiToPolyNodeDist != DBL_MAX &&
            poiToOtherParkPolyNodeDist != DBL_MAX && otherParkPolyHasName)
        {
          LOG_TRACE("Returning miss per review reduction rule #14...");
          return true;
        }

        //If the poi is a park, the poly it is being compared to is not a park or building, and that
        //poly is "very close" to another park poly that has a name match with the poi, then
        //declare a miss (exclude poly playgrounds from this).
        if (poiIsPark && !polyIsPark && !polyIsBuilding && polyVeryCloseToAnotherParkPoly &&
            otherParkPolyNameMatch && !polyIsPlayground)
        {
          LOG_TRACE("Returning miss per review reduction rule #15...");
          return true;
        }

        //If a park poi is contained within one park poly, then there's no reason for it to trigger
        //reviews in another one that its not contained in.
        if (poiIsPark && polyIsPark && _distance > 0 && poiContainedInAnotherParkPoly)
        {
          LOG_TRACE("Returning miss per review reduction rule #16...");
          return true;
        }

        //If a sport poi is contained within an exact type match sport poi poly, don't let it be
        //matched against anything else.
        if (poiIsSport && poiContainedInParkPoly && sportPoiOnOtherSportPolyWithExactTypeMatch)
        {
          LOG_TRACE("Returning miss per review reduction rule #17...");
          return true;
        }

        //If a poi is like and on top of a building, don't review it against a non-building poly.
        if (poiIsBuilding && poiOnBuilding && !polyIsBuilding)
        {
          LOG_TRACE("Returning miss per review reduction rule #18...");
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
            LOG_TRACE("Returning miss per review reduction rule #19...");
            return true;
          }
        }
      }
      catch (const geos::util::TopologyException& e)
      {
        if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
        {
          LOG_TRACE(
            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
            "geometry: " << polyNeighbor->toString() << "\n" << e.what());
          _badGeomCount++;
        }
      }
    }
    polyNeighborItr++;
  }

  //logic for parsing through surrounding POIs
//  set<ElementId>::const_iterator poiNeighborItr = _poiNeighborIds.begin();
//  while (poiNeighborItr != _poiNeighborIds.end())
//  {
//    ConstElementPtr poiNeighbor = _map->getElement(*poiNeighborItr);
//    if (poiNeighbor->getElementId() != poi->getElementId())
//    {
//      shared_ptr<Geometry> poiNeighborGeom ;
//      try
//      {
//        poiNeighborGeom = ElementConverter(_map).convertToGeometry(poiNeighbor);

//        if (polyGeom->contains(poiNeighborGeom.get()))
//        {
//
//
//        }
//      }
//      catch (const geos::util::TopologyException& e)
//      {
//        if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
//        {
//          LOG_TRACE(
//            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
//            "geometry: " << poiNeighbor->toString() << "\n" << e.what());
//          _badGeomCount++;
//        }
//      }
//    }
//    poiNeighborItr++;
//  }

  return false;
}

}