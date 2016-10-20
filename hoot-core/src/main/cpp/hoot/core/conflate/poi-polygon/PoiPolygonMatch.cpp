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
/*#include "PoiPolygonTypeMatch.h"
#include "PoiPolygonNameMatch.h"
#include "PoiPolygonAddressMatch.h"
#include "PoiPolygonMatchDistanceCalculator.h"*/
#include "PoiPolygonEvidenceScorer.h"

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

  try
  {
    _distance = polyGeom->distance(poiGeom.get());
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back to warn?
      /*LOG_WARN*/LOG_DEBUG(
        "Calculating the distance between the POI " << poi->toString() << "\n and polygon " <<
        poly->toString() << "\n caused topology exception on conversion to a " << e.what());
      _badGeomCount++;
    }
    return;
  }

  //CLASSIFICATION

  PoiPolygonEvidenceScorer evidenceScorer(
    _matchDistance, _reviewDistance, _distance, _typeScoreThreshold, _nameScoreThreshold, _map,
    _testUuid);
  const int evidence = evidenceScorer.calculateEvidence(poi, poly);
  if (evidence >= 3)
  {
    _class.setMatch();
  }
  else if (evidence >= 1)
  {
    _class.setReview();
  }

  //REVIEW REDUCTION

  if (evidence > 0)
  {
    MatchClassification externalMatchClass;
    //TODO: pass in evidence scorer here to clean this constructor up ??
    const bool reviewReductionRuleTriggered =
      PoiPolygonReviewReducer(
        _map, _areaNeighborIds, _poiNeighborIds, _distance, evidenceScorer.getNameScore(),
        evidenceScorer.getNameMatch(), evidenceScorer.getExactNameMatch(), _typeScoreThreshold,
        evidenceScorer.getTypeScore(), evidenceScorer.getTypeMatch(), _matchDistance, polyGeom,
        poiGeom, evidence, _testUuid)
        .triggersRule(poi, poly, externalMatchClass);
    if (reviewReductionRuleTriggered)
    {
      //This could just be set to miss, except there is one case in ReviewReducer where a review is
      //returned instead of a miss....may need to refactor that out.
      _class = externalMatchClass;
    }
  }

  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(_class);
    LOG_DEBUG("**************************");
  }

  //DISTANCE TRUTHS

  if (/*Log::getInstance().getLevel() == Log::Debug*/
      ConfigOptions().getPoiPolygonPrintMatchDistanceTruth())
  {
    //output feature distances for all feature types which fell within the match threshold
    const QString ref2 = poi->getTags().get("REF2");
    const QString review = poi->getTags().get("REVIEW");
    if (ref2 == poly->getTags().get("REF1").split(";")[0])
    {
      if (e1IsPoi)
      {
        _poiMatchRefIdsToDistances.insert(evidenceScorer.getType1BestKvp(), _distance);
        _polyMatchRefIdsToDistances.insert(evidenceScorer.getType2BestKvp(), _distance);
      }
      else
      {
        _poiMatchRefIdsToDistances.insert(evidenceScorer.getType2BestKvp(), _distance);
        _polyMatchRefIdsToDistances.insert(evidenceScorer.getType1BestKvp(), _distance);
      }
    }
    else if (review == poly->getTags().get("REF1").split(";")[0])
    {
      if (e1IsPoi)
      {
        _poiReviewRefIdsToDistances.insert(evidenceScorer.getType1BestKvp(), _distance);
        _polyReviewRefIdsToDistances.insert(evidenceScorer.getType2BestKvp(), _distance);
      }
      else
      {
        _poiReviewRefIdsToDistances.insert(evidenceScorer.getType2BestKvp(), _distance);
        _polyReviewRefIdsToDistances.insert(evidenceScorer.getType1BestKvp(), _distance);
      }
    }
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

