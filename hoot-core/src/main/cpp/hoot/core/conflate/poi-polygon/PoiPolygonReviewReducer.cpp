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
#include <hoot/core/algorithms/Translator.h>

#include "PoiPolygonTypeMatch.h"
#include "PoiPolygonNameMatch.h"
#include "PoiPolygonEvidenceScorer.h"

// std
#include <float.h>

namespace hoot
{

QSet<QString> PoiPolygonReviewReducer::_allTagKeys;

PoiPolygonReviewReducer::PoiPolygonReviewReducer(const ConstOsmMapPtr& map,
                                                     const set<ElementId>& areaNeighborIds,
                                                     const set<ElementId>& poiNeighborIds,
                                                     double distance, double nameScoreThreshold,
                                                     bool nameMatch,
                                                     bool exactNameMatch,
                                                     double typeScoreThreshold,
                                                     double typeScore,
                                                     bool typeMatch,
                                                     double matchDistance,
                                                     double reviewDistance,
                                                     shared_ptr<Geometry> polyGeom,
                                                     shared_ptr<Geometry> poiGeom,
                                                     const int evidence,
                                                     const QString testUuid = "") :
_map(map),
_areaNeighborIds(areaNeighborIds),
_poiNeighborIds(poiNeighborIds),
_distance(distance),
_nameScoreThreshold(nameScoreThreshold),
_nameMatch(nameMatch),
_exactNameMatch(exactNameMatch),
_typeScoreThreshold(typeScoreThreshold),
_typeScore(typeScore),
_typeMatch(typeMatch),
_matchDistance(matchDistance),
_reviewDistance(reviewDistance),
_polyGeom(polyGeom),
_poiGeom(poiGeom),
_badGeomCount(0),
_evidence(evidence),
_testUuid(testUuid)
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

bool PoiPolygonReviewReducer::triggersRule(ConstElementPtr poi, ConstElementPtr poly,
                                       MatchClassification& matchClass)
{
  if (!_poiGeom.get())
  {
    //TODO: change back to warn?
    /*LOG_WARN*/LOG_DEBUG("Invalid poi geometry.");
    return false;
  }
  if (!_polyGeom.get())
  {
    //TODO: change back to warn?
    /*LOG_WARN*/LOG_DEBUG("Invalid poly geometry.");
    return false;
  }
  const bool testFeatureFound =
    poly->getTags().get("uuid") == _testUuid || poi->getTags().get("uuid") == _testUuid;

  //The rules below are roughly grouped by processing expense (more granular ordering can still be
  //done to further reduce runtime), with the rules requiring the least expensive computations
  //occurring earlier.

  const QString poiName = poi->getTags().get("name").toLower();

  const bool poiHasType =
    OsmSchema::getInstance().getCategories(poi->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  const bool poiIsPark = _isPark(poi);
  const bool poiIsParkish = _isParkish(poi);
  const bool poiIsPlayArea = _isPlayArea(poi);
  const bool poiIsPlayground = _isPlayground(poi);
  const bool poiIsRecCenter = _isRecCenter(poi);
  const bool poiIsSport = _isSport(poi);

  const bool polyHasType =
    OsmSchema::getInstance().getCategories(poly->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  const bool polyIsPark = _isPark(poly);
  const bool polyIsPlayground = _isPlayground(poly);
  const bool polyIsBuilding = OsmSchema::getInstance().isBuilding(poly);
  const bool polyIsRecCenter = _isRecCenter(poly);
  const bool polyIsSport = _isSport(poly);

  //If the poi is a rec center and the poly is not a rec center or a building, return a miss.
  if (poiIsRecCenter && !polyIsRecCenter && !polyIsBuilding)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #1...");
    }
    matchClass.setMiss();
    return true;
  }

  //Don't match sports fields to parks.  This seems a little simplistic, but has had positive
  //results so far.
  //TODO: Is this rule superceded by #11?
  //TODO: make is sport method
  if (poi->getTags().contains("sport") && polyIsPark && _distance == 0)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #2...");
    }
    matchClass.setMiss();
    return true;
  }

  //Be a little stricter on place related reviews.
  //TODO: make is place method
  if (poi->getTags().get("place").toLower() == "neighbourhood" &&
           !poly->getTags().contains("place"))
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #3...");
    }
    matchClass.setMiss();
    return true;
  }

  //Reduce reviews against some non-use type polys.
  //TODO: replace with isArea/isPoi changes...tried already once and didn't seem to work
  if (poly->getTags().get("natural").toLower() == "coastline" ||
           poi->getTags().get("highway").toLower() == "traffic_signals" ||
           poi->getTags().get("amenity").toLower() == "atm" ||
           poi->getTags().get("man_made").toLower() == "surveillance")
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #4...");
    }
    matchClass.setMiss();
    return true;
  }

  //Gates and Parking are two separate things.
  if (poi->getTags().get("barrier").toLower() == "gate" &&
           poly->getTags().get("amenity").toLower() == "parking")
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #5...");
    }
    matchClass.setMiss();
    return true;
  }

  //Don't match a park poi to a sport poly.  Simpler version of some rules above.
  //TODO: may make some previous rules obsolete
  if (poiIsPark && polyIsSport)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #6...");
    }
    matchClass.setMiss();
    return true;
  }

  //Don't review schools against their sports fields.
  //TODO: make is sport/school methods
  if (poi->getTags().get("amenity").toLower() == "school" && poly->getTags().contains("sport"))
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #7...");
    }
    matchClass.setMiss();
    return true;
  }

  //Similar to previous, except more focused for restrooms.
  //TODO: make is restroom method - alternatively use building in the schema for this
  if (poiHasType && polyHasType && !_typeScore == 1.0 &&
           poi->getTags().get("amenity").toLower() == "toilets" &&
           !OsmSchema::getInstance().getCategories(poly->getTags()).intersects(
             OsmSchemaCategory::building()))
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #8...");
    }
    matchClass.setMiss();
    return true;
  }

  //Need to be stricter on tunnels since we don't want above ground things to review against them.
  //TODO: make is underground method or schema change
  if (poly->getTags().get("tunnel") == "yes" && poiHasType &&
           (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #9...");
    }
    matchClass.setMiss();
    return true;
  }

  //Be more strict reviewing parking lots against other features.
  //TODO: make is parking method
  if (poiHasType && polyHasType && poly->getTags().get("amenity") == "parking" &&
           _distance > _matchDistance &&
           (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #10...");
    }
    matchClass.setMiss();
    return true;
  }

  //Be more strict reviewing natural features against building features.
  //TODO: make is natural method
  if (poiHasType && polyHasType && poly->getTags().contains("natural") &&
           OsmSchema::getInstance().getCategories(
             poi->getTags()).intersects(OsmSchemaCategory::building()))
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #11...");
    }
    matchClass.setMiss();
    return true;
  }

  //Landuse polys often wrap a bunch of other features and don't necessarily match to POI's, so
  //be more strict with their reviews.
  //TODO: make is landuse method
  if (_genericLandUseTagVals.contains(poly->getTags().get("landuse")) &&
           _distance > _matchDistance &&
           (!(_typeMatch || _nameMatch) || (_nameMatch && _typeScore < 0.2)))
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #12...");
    }
    matchClass.setMiss();
    return true;
  }

  const bool polyHasMoreThanOneType = _hasMoreThanOneType(poly);

  //This is a simple rule to prevent matching poi's not at all like a park with park polys.
  //TODO: this may render some of the previous rules obsolete.
  //*
  if (!poiIsPark && !poiIsParkish && poiHasType && polyIsPark && !polyHasMoreThanOneType)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #13...");
    }
    matchClass.setMiss();
    return true;
  }

  double polyArea = -1.0;
  try
  {
    polyArea = _polyGeom->getArea();
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back
      /*LOG_WARN*/LOG_DEBUG(
        "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
        "geometry: " << _polyGeom->toString() << "\n" << e.what());
      _badGeomCount++;
    }
  }

  //Play areas areas can get matched to the larger parks they reside within.  We're setting a max
  //size for play area polys here to prevent that.  Unfortunately, most play areas are tagged as
  //just parks and not specifically as play areas, if they are tagged at all, so we're just scanning
  //the name tag here to determine if something is a play area vs actually verifying that face by
  //looking at its type.
  if (poiName.contains("play area") && polyArea > 25000) //TODO: move this value to a config?
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #14...");
    }
    matchClass.setMiss();
    return true;
  }

  //Don't review park poi's against large non-park polys.
  //TODO: !polyIsPark conditional adds two wrong matches to C but takes one away from D; look
  //into it
  if (poiHasType && polyHasType && !_nameMatch && !polyIsPark &&
           (((poly->getTags().contains("landuse") && !poi->getTags().contains("landuse")) ||
           (poly->getTags().contains("natural") && !poi->getTags().contains("natural")) ||
           poly->getTags().get("leisure").toLower() == "common") ||
           (poly->getTags().get("place").toLower() == "neighborhood" ||
            poly->getTags().get("place").toLower() == "neighbourhood")) &&
           polyArea > 50000)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #15...");
    }
    matchClass.setMiss();
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
  bool polyContainsPlayAreaOrPlaygroundPoly = false;
  bool poiContainedInAnotherParkPoly = false;
  bool polyContainsAnotherParkOrPlaygroundPoi = false;
  bool containedOtherParkOrPlaygroundPoiHasName = false;
  bool sportPoiOnOtherSportPolyWithExactTypeMatch = false;
  bool anotherPolyContainsPoiWithTypeMatch = false;
  bool poiOnBuilding = false;

  PoiPolygonTypeMatch typeScorer(_typeScoreThreshold, _testUuid);
  PoiPolygonNameMatch nameScorer(_nameScoreThreshold);

  set<ElementId>::const_iterator areaNeighborItr = _areaNeighborIds.begin();
  while (areaNeighborItr != _areaNeighborIds.end())
  {
    ConstElementPtr area = _map->getElement(*areaNeighborItr);
    if (area->getElementId() != poly->getElementId())
    {
      shared_ptr<Geometry> areaGeom;
      try
      {
        //TODO: temp suppress "unable to connect all ways..." message here?
        areaGeom = ElementConverter(_map).convertToGeometry(area);

        if (areaGeom.get() &&
            QString::fromStdString(areaGeom->toString()).toUpper().contains("EMPTY"))
        {
          if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
          {
            //TODO: change back
            /*LOG_WARN*/LOG_DEBUG(
              "Invalid area neighbor polygon passed to PoiPolygonMatchCreator: " <<
              areaGeom->toString());
            _badGeomCount++;
          }
        }
        else if (areaGeom.get())
        {
          /*if (testFeatureFound)
          {
            LOG_VARD(area->getTags().get("uuid"))
            LOG_VARD(_isPlayground(area));
            LOG_VARD(_isPlayArea(area));
            LOG_VARD(_polyGeom->contains(areaGeom.get()));
            LOG_VARD(_isSport(area));
            LOG_VARD(poiIsSport);
            LOG_VARD(areaGeom->contains(_poiGeom.get()));
            LOG_VARD(typeScorer.isExactTypeMatch(poi, area));
            QString t1, t2;
            LOG_VARD(typeScorer.getTypeScore(poi, area, t1, t2));
            LOG_VARD(t1);
            LOG_VARD(t2);
          }*/

          if (_isPark(area))
          {
            otherParkPolyHasName = !area->getTags().get("name").trimmed().isEmpty();
            otherParkPolyNameScore = nameScorer.getNameScore(poi, area);
            otherParkPolyNameMatch = otherParkPolyNameScore >= _nameScoreThreshold;

            if (areaGeom->contains(_poiGeom.get()))
            {
              poiContainedInAnotherParkPoly = true;

              if (testFeatureFound)
              {
                LOG_DEBUG(
                  "poi examined and found to be contained within a park poly outside of this match " <<
                  "comparison: " << poi->toString());
                LOG_DEBUG("park poly it is very close to: " << area->toString());
              }
            }

            if (areaGeom->intersects(_polyGeom.get()))
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
                  //TODO: temp suppress "unable to connect all ways..." message here?
                  shared_ptr<const LineString> polyLineStr =
                      dynamic_pointer_cast<const LineString>(
                        ElementConverter(_map).convertToLineString(polyWay));
                  poiToPolyNodeDist = polyLineStr->distance(_poiGeom.get());
                  ConstWayPtr areaWay = dynamic_pointer_cast<const Way>(area);
                  //TODO: temp suppress "unable to connect all ways..." message here?
                  shared_ptr<const LineString> areaLineStr =
                      dynamic_pointer_cast<const LineString>(
                        ElementConverter(_map).convertToLineString(areaWay));
                  poiToOtherParkPolyNodeDist = areaLineStr->distance(_poiGeom.get());
                }

                if (testFeatureFound)
                {
                  LOG_DEBUG(
                    "poly examined and found very close to a another park poly: " << poly->toString());
                  LOG_DEBUG("park poly it is very close to: " << area->toString());
                }
              }
            }
          }
          else if ((_isPlayground(area) || _isPlayArea(area)) && _polyGeom->contains(areaGeom.get()))
          {
            polyContainsPlayAreaOrPlaygroundPoly = true;

            if (testFeatureFound)
            {
              LOG_DEBUG("poly examined and found to contain a playground poly: " << poly->toString());
              LOG_DEBUG("playground poly it contains: " << area->toString());
            }
          }
          else if (poiIsSport)
          {
            //this is a little lose, b/c there could be more than one type match set of tags...
            if (_isSport(area) && areaGeom->contains(_poiGeom.get()) &&
                typeScorer.isExactTypeMatch(poi, area))
            {
              sportPoiOnOtherSportPolyWithExactTypeMatch = true;
            }
          }
          else if (OsmSchema::getInstance().isBuilding(area))
          {
            if (areaGeom->contains(_poiGeom.get()))
            {
              poiOnBuilding = true;
            }
          }
          //type generic rules
          else if (typeScorer.isTypeMatch(poi, area))
          {
            if (areaGeom->contains(_poiGeom.get()))
            {
              anotherPolyContainsPoiWithTypeMatch = true;
            }
          }
        }
      }
      catch (const geos::util::TopologyException& e)
      {
        if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
        {
          //TODO: change back
          /*LOG_WARN*/LOG_DEBUG(
            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
            "geometry: " << area->toString() << "\n" << e.what());
          _badGeomCount++;
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
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #16...");
    }
    matchClass.setMiss();
    return true;
  }

  //If the poi is not a park and being compared to a park polygon or a polygon that is "very close"
  //to another park poly, we want to be more restrictive on type matching, but only if the poi has
  //any type at all.  If the poi has no type, then behave as normal.  Also, let an exact name match
  //cause a review here, rather than a miss.
  //*
  if ((polyIsPark || (polyVeryCloseToAnotherParkPoly && !polyHasType)) &&
           !polyHasMoreThanOneType && !poiIsPark && !poiIsParkish && poiHasType)
  {
    if (_exactNameMatch)
    {
      //Only misnomer for "ReviewReducer"...can we get rid of this?
      if (testFeatureFound)
      {
        LOG_DEBUG("Returning review per rule #17...");
      }
      matchClass.setReview();
    }
    else
    {
      if (testFeatureFound)
      {
        LOG_DEBUG("Returning miss per rule #17...");
      }
      matchClass.setMiss();
    }
    return true;
  }

  //If the poi is a park, the poly it is being compared to is not a park or building, and that poly
  //is "very close" to another park poly that has a name match with the poi, then declare a miss
  //(exclude poly playgrounds from this).
  if (poiIsPark && !polyIsPark && !polyIsBuilding && polyVeryCloseToAnotherParkPoly &&
           otherParkPolyNameMatch && !polyIsPlayground)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #18...");
    }
    matchClass.setMiss();
    return true;
  }

  //Commonly, parks will contain play areas along with other entities (basketball courts, etc.).
  //The play area is considered to be a subpart of the park polygon.  Sometimes parks are named with
  //"playground", which makes this confusing.  Here, we're differentiating between playgrounds,
  //parks, and play areas and attempting to match play area poi's to play area or playground polys.
  //We're not, however, trying to match playground poi's to play area or playground poly, b/c
  //sometimes those need to be matched to the surrounding park polys...that situation should
  //at the very least end up as a review.
  if (poiIsPlayArea && polyIsPark && polyContainsPlayAreaOrPlaygroundPoly)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #19...");
    }
    matchClass.setMiss();
    return true;
  }

  //If a park poi is contained within one park poly, then there's no reason for it to trigger
  //reviews in another one that its not contained in.
  if (poiIsPark && polyIsPark && _distance > 0 && poiContainedInAnotherParkPoly)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #20...");
    }
    matchClass.setMiss();
    return true;
  }

  //If a sport poi is contained within an exact type match sport poi poly, don't let it be
  //matched against anything else.
  if (poiIsSport && poiContainedInParkPoly && sportPoiOnOtherSportPolyWithExactTypeMatch)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #21...");
    }
    matchClass.setMiss();
    return true;
  }

  //If a building is a building don't review it against a non-building poly.
  if (_isBuildingIsh(poi) && poiOnBuilding && !polyIsBuilding)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #22...");
    }
    matchClass.setMiss();
    return true;
  }

  set<ElementId>::const_iterator poiNeighborItr = _poiNeighborIds.begin();
  while (poiNeighborItr != _poiNeighborIds.end())
  {
    ConstElementPtr poiNeighbor = _map->getElement(*poiNeighborItr);
    if (poiNeighbor->getElementId() != poi->getElementId())
    {
      shared_ptr<Geometry> poiNeighborGeom ;
      try
      {
        poiNeighborGeom = ElementConverter(_map).convertToGeometry(poiNeighbor);

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

        if ((_isPark(poiNeighbor) || _isPlayground(poiNeighbor)) &&
            !_isPlayArea(poiNeighbor) && _polyGeom->contains(poiNeighborGeom.get()))
        {
          polyContainsAnotherParkOrPlaygroundPoi = true;
          if (!containedOtherParkOrPlaygroundPoiHasName)
          {
            containedOtherParkOrPlaygroundPoiHasName =
              !poiNeighbor->getTags().get("name").trimmed().isEmpty();
          }

          if (testFeatureFound)
          {
            LOG_DEBUG(
              "poly examined and found to contain another park or playground poi " <<
              poly->toString());
            LOG_DEBUG("park/playground poi it is very close to: " << poiNeighbor->toString());
          }
        }
      }
      catch (const geos::util::TopologyException& e)
      {
        if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
        {
          //TODO: change back
          /*LOG_WARN*/LOG_DEBUG(
            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
            "geometry: " << poiNeighbor->toString() << "\n" << e.what());
          _badGeomCount++;
        }
      }
    }
    poiNeighborItr++;
  }

  //If this isn't a park or playground poi, then don't match it to any park poly that contains
  //another park or playground poi.
  if (poiIsPlayArea && !poiIsPlayground && polyIsPark && _distance == 0 &&
           polyContainsAnotherParkOrPlaygroundPoi && containedOtherParkOrPlaygroundPoiHasName)
  {
    if (testFeatureFound)
    {
      LOG_DEBUG("Returning miss per rule #23...");
    }
    matchClass.setMiss();
    return true;
  }

  return false;
}

bool PoiPolygonReviewReducer::_isRecCenter(ConstElementPtr element) const
{
  const QString elementName =
    Translator::getInstance().toEnglish(element->getTags().get("name").toLower());
  return elementName.contains("recreation center") || elementName.contains("rec center");
}

bool PoiPolygonReviewReducer::_isRecCenter2(ConstElementPtr element) const
{
  const QString elementName =
    Translator::getInstance().toEnglish(element->getTags().get("name").toLower());
  return elementName.contains("recreation center") || elementName.contains("rec center") ||
    elementName.contains("clubhouse") || elementName.contains("fieldhouse");
}

bool PoiPolygonReviewReducer::_isPlayground(ConstElementPtr element) const
{
  const Tags& tags = element->getTags();
  const QString elementName = Translator::getInstance().toEnglish(tags.get("name").toLower());
  return tags.get("leisure") == "playground" || elementName.contains("playground");
}

bool PoiPolygonReviewReducer::_isPlayArea(ConstElementPtr element) const
{
  return element->getTags().get("name").toLower().contains("play area");
}

bool PoiPolygonReviewReducer::_isPark(ConstElementPtr element) const
{
  return !OsmSchema::getInstance().isBuilding(element) &&
         (element->getTags().get("leisure") == "park");
}

bool PoiPolygonReviewReducer::_isParkish(ConstElementPtr element) const
{
  if (OsmSchema::getInstance().isBuilding(element))
  {
    return false;
  }
  const QString leisureVal = element->getTags().get("leisure").toLower();
  return leisureVal == "garden" || leisureVal == "dog_park";
}

bool PoiPolygonReviewReducer::_isSport(ConstElementPtr element) const
{
  return element->getTags().contains("sport");
}

bool PoiPolygonReviewReducer::_isBuildingIsh(ConstElementPtr element) const
{
  const QString elementName =
    Translator::getInstance().toEnglish(element->getTags().get("name").toLower());
  return OsmSchema::getInstance().isBuilding(element) || elementName.contains("building") ||
    elementName.contains("bldg");
}

bool PoiPolygonReviewReducer::_hasMoreThanOneType(ConstElementPtr element) const
{
//  int typeCount = 0;
//  QStringList typesParsed;
//  if (_allTags.size() == 0)
//  {
//    _allTags = OsmSchema::getInstance().getAllTags();
//  }
//  for (vector<SchemaVertex>::const_iterator it = _allTags.begin(); it != _allTags.end(); ++it)
//  {
//    SchemaVertex vertex = *it;
//    //LOG_DEBUG("Key: " << vertex.key);
//    //there may be duplicate keys in allTags
//    if (element->getTags().contains(vertex.key) && !typesParsed.contains(vertex.key))
//    {
//      LOG_DEBUG("Has key: " << vertex.key);
//      typeCount++;
//      if (typeCount > 1)
//      {
//        return true;
//      }
//    }

//    typesParsed.append(vertex.key);
//  }
//  return false;

  int typeCount = 0;
  QStringList typesParsed;
  if (_allTagKeys.size() == 0)
  {
    _allTagKeys = OsmSchema::getInstance().getAllTagKeys();
  }
  const Tags elementTags = element->getTags();
  for (Tags::const_iterator it = elementTags.begin(); it != elementTags.end(); ++it)
  {
    const QString elementTagKey = it.key();
    //LOG_DEBUG("Key: " << vertex.key);
    //there may be duplicate keys in allTags
    if (_allTagKeys.contains(elementTagKey) && !typesParsed.contains(elementTagKey))
    {
      LOG_DEBUG("Has key: " << elementTagKey);
      typeCount++;
      if (typeCount > 1)
      {
        return true;
      }
    }

    typesParsed.append(elementTagKey);
  }
  return false;
}

}
