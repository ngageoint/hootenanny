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
#include "PoiPolygonMatch.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/algorithms/ExactStringDistance.h>

#include "PoiPolygonReviewReducer.h"
#include "PoiPolygonTypeMatch.h"
#include "PoiPolygonNameMatch.h"
#include "PoiPolygonAddressMatch.h"
#include "PoiPolygonMatchDistanceCalculator.h"

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

QString PoiPolygonMatch::_testUuid = "{b18057ff-736d-5d20-b873-837f0c172e33}";
QMultiMap<QString, double> PoiPolygonMatch::_poiMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_polyMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_poiReviewRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_polyReviewRefIdsToDistances;

//TODO: reduce these constructors

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold,
                                 shared_ptr<const PoiPolygonRfClassifier> rf) :
Match(threshold),
_eid1(eid1),
_eid2(eid2),
_rf(rf),
_badGeomCount(0),
_map(map),
_distance(-1.0),
_nameScore(-1.0),
_nameMatch(false),
_exactNameMatch(false),
_typeScore(-1.0),
_typeMatch(false),
_matchDistance(ConfigOptions().getPoiPolygonMatchDistance()),
_reviewDistance(ConfigOptions().getPoiPolygonMatchReviewDistance()),
_nameScoreThreshold(ConfigOptions().getPoiPolygonMatchNameThreshold()),
_typeScoreThreshold(ConfigOptions().getPoiPolygonMatchTypeThreshold())
{
  _calculateMatch(eid1, eid2);
}

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold,
                                 shared_ptr<const PoiPolygonRfClassifier> rf,
                                 const set<ElementId>& areaNeighborIds = set<ElementId>(),
                                 const set<ElementId>& poiNeighborIds = set<ElementId>()) :
Match(threshold),
_eid1(eid1),
_eid2(eid2),
_rf(rf),
_badGeomCount(0),
_map(map),
_distance(-1.0),
_nameScore(-1.0),
_nameMatch(false),
_exactNameMatch(false),
_typeScore(-1.0),
_typeMatch(false),
_matchDistance(ConfigOptions().getPoiPolygonMatchDistance()),
_reviewDistance(ConfigOptions().getPoiPolygonMatchReviewDistance()),
_nameScoreThreshold(ConfigOptions().getPoiPolygonMatchNameThreshold()),
_typeScoreThreshold(ConfigOptions().getPoiPolygonMatchTypeThreshold()),
_areaNeighborIds(areaNeighborIds),
_poiNeighborIds(poiNeighborIds)
{
  _calculateMatch(eid1, eid2);
}

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold,
                                 shared_ptr<const PoiPolygonRfClassifier> rf,
                                 double matchDistance, double reviewDistance,
                                 double nameScoreThreshold, double typeScoreThreshold) :
Match(threshold),
_eid1(eid1),
_eid2(eid2),
_rf(rf),
_badGeomCount(0),
_map(map),
_distance(-1.0),
_nameScore(-1.0),
_nameMatch(false),
_exactNameMatch(false),
_typeScore(-1.0),
_typeMatch(false),
_matchDistance(matchDistance),
_reviewDistance(reviewDistance),
_nameScoreThreshold(nameScoreThreshold),
_typeScoreThreshold(typeScoreThreshold)
{
  _calculateMatch(eid1, eid2);
}

//TODO: define a poi poly poly category??

bool PoiPolygonMatch::isPoly(const Element& e)
{
  const Tags& tags = e.getTags();
  //check this first, b/c some of these excluded features may have multiple type vals
  const bool inABuildingOrPoiCategory =
    OsmSchema::getInstance().getCategories(tags).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  /*if (tags.get("uuid") == _testUuid)
  {
    LOG_VARD(inABuildingOrPoiCategory);
    LOG_VARD(e.toString());
  }*/
  if (/*!inABuildingOrPoiCategory &&*/
      (tags.get("barrier").toLower() == "fence"
      || tags.get("landuse").toLower() == "grass"
      || tags.get("natural").toLower() == "tree_row"
      || tags.get("natural").toLower() == "scrub"
      || tags.get("highway").toLower() == "residential"))
  {
    return false;
  }
  return OsmSchema::getInstance().isArea(tags, e.getElementType()) &&
         (inABuildingOrPoiCategory || tags.getNames().size() > 0);
}

//TODO: define a poi poly poi category??

bool PoiPolygonMatch::isPoi(const Element& e)
{
  /*if (Log::getInstance().getLevel() == Log::Debug && e.getTags().get("uuid") == _testUuid)
  {
    LOG_DEBUG("poi candidate:");
    LOG_VARD(e.getTags().get("uuid"));
    LOG_VARD(e.getTags().getNames().size());
    LOG_VARD(e.getTags().getNames());
    LOG_VARD(OsmSchema::getInstance().getCategories(e.getTags()).toString());
    LOG_VARD(OsmSchema::getInstance().getCategories(e.getTags()).intersects(
               OsmSchemaCategory::building() | OsmSchemaCategory::poi()));
    LOG_VARD(
      OsmSchema::getInstance().getCategories(e.getTags()).intersects(OsmSchemaCategory::building()));
    LOG_VARD(
      OsmSchema::getInstance().getCategories(e.getTags()).intersects(OsmSchemaCategory::poi()));
    LOG_VARD(e.getTags());
  }*/

  //TODO: I haven't figure out a way to bypass hgispoi defining these as poi's yet...need to fix.
  //TODO: replace logic in rule applier with commented out ones
  const Tags& tags = e.getTags();
  if (tags.get("natural").toLower() == "tree"
      || tags.get("amenity").toLower() == "drinking_water"
      || tags.get("amenity").toLower() == "bench"
      || tags.contains("traffic_sign")
      || tags.get("amenity").toLower() == "recycling")
  {
    return false;
  }

  return e.getElementType() == ElementType::Node &&
         (OsmSchema::getInstance().getCategories(e.getTags()).intersects(
           OsmSchemaCategory::building() | OsmSchemaCategory::poi()) ||
          e.getTags().getNames().size() > 0);
}

bool PoiPolygonMatch::isArea(const Element& e)
{
  const Tags& tags = e.getTags();
  return isPoly(e) && !OsmSchema::getInstance().isBuilding(tags, e.getElementType());
}

void PoiPolygonMatch::_calculateMatch(const ElementId& eid1, const ElementId& eid2)
{
  _class.setMiss();

  ConstElementPtr e1 = _map->getElement(eid1);
  ConstElementPtr e2 = _map->getElement(eid2);

  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(_eid1);
    LOG_VARD(e1->getTags().get("uuid"));
    LOG_VARD(e1->getTags());
    LOG_VARD(_eid2);
    LOG_VARD(e2->getTags().get("uuid"));
    LOG_VARD(e2->getTags());
  }

  ConstElementPtr poi, poly;
  bool e1IsPoi = false;
  if (isPoi(*e1) && isPoly(*e2))
  {
    _poiEid = eid1;
    _polyEid = eid2;
    poi = e1;
    poly = e2;
    e1IsPoi = true;
  }
  else if (isPoi(*e2) && isPoly(*e1))
  {
    _poiEid = eid2;
    _polyEid = eid1;
    poi = e2;
    poly = e1;
  }
  else
  {
    LOG_WARN(e1->toString());
    LOG_WARN(e2->toString());
    throw IllegalArgumentException("Expected a POI & polygon, got: " + eid1.toString() + " " +
                                   eid2.toString());
  }

  //GEOMETRY CONVERSION

  //TODO: temp suppress "unable to connect all ways..." message here?
  shared_ptr<Geometry> polyGeom;
  try
  {
    polyGeom = ElementConverter(_map).convertToGeometry(poly);
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back to warn?
      /*LOG_WARN*/LOG_DEBUG(
        "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
        "geometry: " << poly->toString() << "\n" << e.what());
      _badGeomCount++;
    }
    return;
  }
  //may need a better way to handle this...(already tried using isValid())
  if (QString::fromStdString(polyGeom->toString()).toUpper().contains("EMPTY"))
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back to warn?
      /*LOG_WARN*/LOG_DEBUG("Invalid polygon passed to PoiPolygonMatchCreator: " << polyGeom->toString());
      _badGeomCount++;
    }
    return;
  }
  shared_ptr<Geometry> poiGeom;
  try
  {
    poiGeom = ElementConverter(_map).convertToGeometry(poi);
    _distance = polyGeom->distance(poiGeom.get());
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back to warn?
      /*LOG_WARN*/LOG_DEBUG(
        "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
        "geometry: " << poi->toString() << "\n" << e.what());
      _badGeomCount++;
    }
    return;
  }

  int evidence = 0;

  //DISTANCE

  // calculate the 2 sigma for the distance between the two objects
  const double sigma1 = e1->getCircularError() / 2.0;
  const double sigma2 = e1->getCircularError() / 2.0;
  const double ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;
  const double reviewDistancePlusCe = _reviewDistance + ce;
  const bool closeMatch = _distance <= reviewDistancePlusCe;
  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(closeMatch);
    LOG_VARD(_distance);
    LOG_VARD(_matchDistance);
    LOG_VARD(_reviewDistance);
    LOG_VARD(reviewDistancePlusCe);
    LOG_VARD(ce);
    LOG_VARD(e1->getCircularError());
    LOG_VARD(e2->getCircularError());
  }
  const bool printMatchDistanceTruth = ConfigOptions().getPoiPolygonPrintMatchDistanceTruth();
  //close match is a requirement, regardless of the evidence count
  if (!closeMatch && !printMatchDistanceTruth)
  {
    //don't exit early here if printing truths, b/c we need to calculate type match for that first
    //before exiting
    return;
  }
  PoiPolygonMatchDistanceCalculator distanceCalc(
    _matchDistance, _reviewDistance, _map->getElement(_polyEid)->getTags());
  _matchDistance =
    max(
      distanceCalc.getMatchDistanceForType(_t1BestKvp),
      distanceCalc.getMatchDistanceForType(_t2BestKvp));
  _reviewDistance =
    max(
      distanceCalc.getReviewDistanceForType(_t1BestKvp),
      distanceCalc.getReviewDistanceForType(_t2BestKvp));
  evidence += _distance <= _matchDistance ? 2 : 0;

  //TYPE

  PoiPolygonTypeMatch typeScorer(_typeScoreThreshold, _testUuid);
  _typeScore = typeScorer.getTypeScore(poi, poly, _t1BestKvp, _t2BestKvp);
  if (poi->getTags().get("historic") == "monument")
  {
    //monuments can represent just about any poi type, so lowering this some to account for that
    _typeScoreThreshold = 0.3; //TODO: move to config
  }
  _typeMatch = _typeScore >= _typeScoreThreshold;
  evidence += _typeMatch ? 1 : 0;
  //TODO: re-enable
  if (/*Log::getInstance().getLevel() == Log::Debug*/printMatchDistanceTruth)
  {
    //output feature distances for all feature types which fell within the match threshold
    const QString ref2 = poi->getTags().get("REF2");
    const QString review = poi->getTags().get("REVIEW");
    if (ref2 == poly->getTags().get("REF1").split(";")[0])
    {
      if (e1IsPoi)
      {
        _poiMatchRefIdsToDistances.insert(_t1BestKvp, _distance);
        _polyMatchRefIdsToDistances.insert(_t2BestKvp, _distance);
      }
      else
      {
        _poiMatchRefIdsToDistances.insert(_t2BestKvp, _distance);
        _polyMatchRefIdsToDistances.insert(_t1BestKvp, _distance);
      }
    }
    else if (review == poly->getTags().get("REF1").split(";")[0])
    {
      if (e1IsPoi)
      {
        _poiReviewRefIdsToDistances.insert(_t1BestKvp, _distance);
        _polyReviewRefIdsToDistances.insert(_t2BestKvp, _distance);
      }
      else
      {
        _poiReviewRefIdsToDistances.insert(_t2BestKvp, _distance);
        _polyReviewRefIdsToDistances.insert(_t1BestKvp, _distance);
      }
    }
  }

  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(_typeScore);
    LOG_VARD(_typeMatch);
    LOG_VARD(_t1BestKvp);
    LOG_VARD(_t2BestKvp);
  }
  if (!closeMatch) //second chance to exit early if printing match truths
  {
    return;
  }

  //NAME

  PoiPolygonNameMatch nameScorer(_nameScoreThreshold, _testUuid);
  _nameScore = nameScorer.getNameScore(poi, poly);
  _nameMatch = _nameScore >= _nameScoreThreshold;
  _exactNameMatch = nameScorer.getExactNameScore(poi, poly) == 1.0;
  evidence += _nameMatch ? 1 : 0;
  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(_nameMatch);
    LOG_VARD(_exactNameMatch);
    LOG_VARD(_nameScore);
  }

  //ADDRESS

  //no point in calc'ing the address match if we already have a match from the other evidence
  //TODO: make threshold levels constants
  bool addressMatch = false;
  if (evidence <= 3)
  {
    addressMatch = PoiPolygonAddressMatch(_map, _testUuid).calculateMatch(poly, poi);
  }
  else
  {
    LOG_DEBUG("Skipped address matching.");
  }
  evidence += addressMatch ? 1 : 0;
  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(addressMatch);
  }

  //CLASSIFICATION

  if (evidence >= 3)
  {
    _class.setMatch();
  }
  else if (evidence >= 1)
  {
    _class.setReview();
  }

  //REVIEW REDUCTION

  MatchClassification externalMatchClass;
  //TODO: pass in nameScorer and typeScorer here to clean this constructor up
  const bool reviewReductionRuleTriggered =
    PoiPolygonReviewReducer(
      _map, _areaNeighborIds, _poiNeighborIds, _distance, _nameScore, _nameMatch, _exactNameMatch,
      _typeScoreThreshold, _typeScore, _typeMatch, _matchDistance, polyGeom, poiGeom, _testUuid)
      .triggersRule(poi, poly, externalMatchClass);
  if (reviewReductionRuleTriggered)
  {
    //This could just be set to miss, except there is one case in ReviewReducer where a review is
    //returned instead of a miss....may need to refactor that out.
    _class = externalMatchClass;
  }

  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(_class);
    LOG_DEBUG("**************************");
  }
}

void PoiPolygonMatch::printMatchDistanceInfo()
{
  _printMatchDistanceInfo("POI Match", _poiMatchRefIdsToDistances);
  _printMatchDistanceInfo("Poly Match", _polyMatchRefIdsToDistances);
  _printMatchDistanceInfo("POI Review", _poiReviewRefIdsToDistances);
  _printMatchDistanceInfo("Poly Review", _polyReviewRefIdsToDistances);
}

void PoiPolygonMatch::resetMatchDistanceInfo()
{
  _poiMatchRefIdsToDistances.clear();
  _polyMatchRefIdsToDistances.clear();
  _poiReviewRefIdsToDistances.clear();
  _polyReviewRefIdsToDistances.clear();
}

void PoiPolygonMatch::_printMatchDistanceInfo(const QString matchType,
                                              const QMultiMap<QString, double>& distanceInfo)
{
  foreach (QString type, distanceInfo.uniqueKeys())
  {
    if (!type.trimmed().isEmpty())
    {
      double maxDistance = 0.0;
      double minimumDistance = 99999;
      double averageDistance = 0.0;
      int numberOfEntries = 0;

      QList<double> distances = distanceInfo.values(type);
      qSort(distances.begin(), distances.end());
      double sumDist = 0.0;
      QString distancesStr = "";
      for (QList<double>::const_iterator itr = distances.begin(); itr != distances.end(); ++itr)
      {
        const double dist = *itr;
        maxDistance = max(maxDistance, dist);
        minimumDistance = min(minimumDistance, dist);
        sumDist += dist;
        distancesStr += QString::number(dist) + ", ";
        numberOfEntries++;
      }
      distancesStr.chop(2);
      averageDistance = sumDist / (double)distances.size();

      //TODO: change back to info or debug later
      LOG_ERROR(matchType.toUpper() << " distance info for type: " << type);
      LOG_VARE(maxDistance);
      LOG_VARE(minimumDistance);
      LOG_VARE(averageDistance);
      LOG_VARE(numberOfEntries);
      LOG_VARE(distancesStr);
      LOG_ERROR("**************************");
    }
  }
}

set< pair<ElementId, ElementId> > PoiPolygonMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_poiEid, _polyEid));
  return result;
}

map<QString, double> PoiPolygonMatch::getFeatures(const shared_ptr<const OsmMap>& m) const
{
  return _rf->getFeatures(m, _eid1, _eid2);
}

//TODO: add back in more description
QString PoiPolygonMatch::toString() const
{
  return QString("PoiPolygonMatch %1 %2 P: %3").arg(_poiEid.toString()).
    arg(_polyEid.toString()).arg(_class.toString());
}

}

