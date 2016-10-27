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

#include "PoiPolygonTypeMatch.h"
#include "PoiPolygonNameMatch.h"
#include "PoiPolygonEvidenceScorer.h"

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

QString PoiPolygonMatch::_testUuid = "{1c3e97f5-7430-5a1f-b030-9ce1e7038682}";
QMultiMap<QString, double> PoiPolygonMatch::_poiMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_polyMatchRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_poiReviewRefIdsToDistances;
QMultiMap<QString, double> PoiPolygonMatch::_polyReviewRefIdsToDistances;

const unsigned int PoiPolygonMatch::MATCH_EVIDENCE_THRESHOLD = 3;
const unsigned int PoiPolygonMatch::REVIEW_EVIDENCE_THRESHOLD = 1;

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
                                 const set<ElementId>& polyNeighborIds = set<ElementId>(),
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
_polyNeighborIds(polyNeighborIds),
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

void PoiPolygonMatch::_separateElementsByGeometryType(const ElementId& eid1, const ElementId& eid2)
{
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

  _e1IsPoi = false;
  if (isPoi(*e1) && isPoly(*e2))
  {
    _poi = e1;
    _poly = e2;
    _e1IsPoi = true;
  }
  else if (isPoi(*e2) && isPoly(*e1))
  {
    _poi = e2;
    _poly = e1;
  }
  else
  {
    LOG_WARN(e1->toString());
    LOG_WARN(e2->toString());
    throw IllegalArgumentException("Expected a POI & polygon, got: " + eid1.toString() + " " +
                                   eid2.toString());
  }

  _testFeatureFound =
    _poly->getTags().get("uuid") == _testUuid || _poi->getTags().get("uuid") == _testUuid;
}

bool PoiPolygonMatch::_parseGeometries()
{
  //TODO: temp suppress "unable to connect all ways..." message here?
  try
  {
    _polyGeom = ElementConverter(_map).convertToGeometry(_poly);
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back to warn?
      /*LOG_WARN*/LOG_DEBUG(
        "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
        "geometry: " << _poly->toString() << "\n" << e.what());
      _badGeomCount++;
    }
    return false;
  }
  //may need a better way to handle this...(already tried using isValid())
  if (QString::fromStdString(_polyGeom->toString()).toUpper().contains("EMPTY"))
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back to warn?
      /*LOG_WARN*/LOG_DEBUG("Invalid polygon passed to PoiPolygonMatchCreator: " << _polyGeom->toString());
      _badGeomCount++;
    }
    return false;
  }

  try
  {
    _poiGeom = ElementConverter(_map).convertToGeometry(_poi);
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back to warn?
      /*LOG_WARN*/LOG_DEBUG(
        "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
        "geometry: " << _poi->toString() << "\n" << e.what());
      _badGeomCount++;
    }
    return false;
  }

  try
  {
    _distance = _polyGeom->distance(_poiGeom.get());
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      //TODO: change back to warn?
      /*LOG_WARN*/LOG_DEBUG(
        "Calculating the distance between the POI " << _poi->toString() << "\n and polygon " <<
        _poly->toString() << "\n caused topology exception on conversion to a " << e.what());
      _badGeomCount++;
    }
    return false;
  }

  return true;
}

void PoiPolygonMatch::_calculateMatch(const ElementId& eid1, const ElementId& eid2)
{
  _class.setMiss();

  _separateElementsByGeometryType(eid1, eid2);
  if (!_parseGeometries())
  {
    return;
  }

  PoiPolygonEvidenceScorer evidenceScorer(
    _matchDistance, _reviewDistance, _distance, _typeScoreThreshold, _nameScoreThreshold,
    MATCH_EVIDENCE_THRESHOLD, _poiGeom, _polyGeom, _map, _polyNeighborIds, _poiNeighborIds,
    _testUuid);
  const unsigned int evidence = evidenceScorer.calculateEvidence(_poi, _poly);
  if (_testFeatureFound)
  {
    LOG_VARD(evidence);
  }
  if (evidence >= MATCH_EVIDENCE_THRESHOLD)
  {
    _class.setMatch();
  }
  else if (evidence >= REVIEW_EVIDENCE_THRESHOLD)
  {
    _class.setReview();
  }

  if (_testFeatureFound)
  {
    LOG_VARD(_class);
    LOG_DEBUG("**************************");
  }

  if (ConfigOptions().getPoiPolygonPrintMatchDistanceTruth())
  {
    _recordDistanceTruth(evidenceScorer);
  }
}

void PoiPolygonMatch::_recordDistanceTruth(const PoiPolygonEvidenceScorer& evidenceScorer)
{
  //output feature distances for all feature types which fell within the match threshold
  const QString ref2 = _poi->getTags().get("REF2");
  const QString review = _poi->getTags().get("REVIEW");
  if (ref2 == _poly->getTags().get("REF1").split(";")[0])
  {
    if (_e1IsPoi)
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
  else if (review == _poly->getTags().get("REF1").split(";")[0])
  {
    if (_e1IsPoi)
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
      LOG_INFO(matchType.toUpper() << " distance info for type: " << type);
      LOG_VAR(maxDistance);
      LOG_VAR(minimumDistance);
      LOG_VAR(averageDistance);
      LOG_VAR(numberOfEntries);
      LOG_VAR(distancesStr);
      LOG_INFO("**************************");
    }
  }
}

set< pair<ElementId, ElementId> > PoiPolygonMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_poi->getElementId(), _poly->getElementId()));
  return result;
}

map<QString, double> PoiPolygonMatch::getFeatures(const shared_ptr<const OsmMap>& m) const
{
  return _rf->getFeatures(m, _eid1, _eid2);
}

//TODO: add back in more description
QString PoiPolygonMatch::toString() const
{
  return QString("PoiPolygonMatch %1 %2 P: %3").arg(_poi->getElementId().toString()).
    arg(_poly->getElementId().toString()).arg(_class.toString());
}

}

