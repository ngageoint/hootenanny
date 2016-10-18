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

#include "PoiPolygonRuleApplier.h"
#include "PoiPolygonScorer.h"
#include "PoiPolygonAddressMatch.h"

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

QString PoiPolygonMatch::_testUuid = "{00e22c8e-155b-5de4-a0fa-3e98ef220d01}";
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
  ConstElementPtr e1 = _map->getElement(eid1);
  ConstElementPtr e2 = _map->getElement(eid2);

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

  shared_ptr<Geometry> polyGeom;
  try
  {
    polyGeom = ElementConverter(_map).convertToGeometry(poly);
  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      LOG_WARN(
        "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
        "geometry: " << poly->toString() << "\n" << e.what());
      _badGeomCount++;
    }
    _class.setMiss();
    return;
  }
  //may need a better way to handle this...(already tried using isValid())
  if (QString::fromStdString(polyGeom->toString()).toUpper().contains("EMPTY"))
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      LOG_WARN("Invalid polygon passed to PoiPolygonMatchCreator: " << polyGeom->toString());
      _badGeomCount++;
    }
    _class.setMiss();
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
      LOG_WARN(
        "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
        "geometry: " << poi->toString() << "\n" << e.what());
      _badGeomCount++;
    }
    _class.setMiss();
    return;
  }

  PoiPolygonScorer scorer(_nameScoreThreshold, _typeScoreThreshold, _testUuid);

  _nameScore = scorer.getNameScore(poi, poly);
  _nameMatch = _nameScore >= _nameScoreThreshold;
  _exactNameMatch = scorer.getExactNameScore(poi, poly) == 1.0;

  _typeScore = scorer.getTypeScore(poi, poly, _t1BestKvp, _t2BestKvp);
  if (poi->getTags().get("historic") == "monument")
  {
    _typeMatch = _typeScore >= 0.3; //TODO: move to constant
  }
  else
  {
    _typeMatch = _typeScore >= _typeScoreThreshold;
  }
  //const bool exactTypeMatch = typeScore == 1.0;

  double ce = -1.0;
  double reviewDistancePlusCe = -1.0;
  bool closeMatch = false;
  bool addressMatch = false;
  int evidence = -1;

  MatchClassification externalMatchClass;
  const bool externalRuleTriggered =
    PoiPolygonRuleApplier(
      _map, _areaNeighborIds, _poiNeighborIds, _distance, _nameScore, _nameMatch, _exactNameMatch,
      _typeScoreThreshold, _typeScore, _typeMatch, _matchDistance, polyGeom, poiGeom, _testUuid)
      .applyRules(poi, poly, externalMatchClass);
  if (!externalRuleTriggered)
  {
    // calculate the 2 sigma for the distance between the two objects
    const double sigma1 = e1->getCircularError() / 2.0;
    const double sigma2 = e1->getCircularError() / 2.0;
    ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;

    addressMatch = PoiPolygonAddressMatch(_map, _testUuid).calculateMatch(poly, poi);

    _matchDistance =
      max(_getMatchDistanceForType(_t1BestKvp), _getMatchDistanceForType(_t2BestKvp));
    _reviewDistance =
      max(_getReviewDistanceForType(_t1BestKvp), _getReviewDistanceForType(_t2BestKvp));
    reviewDistancePlusCe = _reviewDistance + ce;
    closeMatch = _distance <= reviewDistancePlusCe;

    evidence = 0;
    evidence += _typeMatch ? 1 : 0;
    evidence += _nameMatch ? 1 : 0;
    evidence += addressMatch ? 1 : 0;
    evidence += _distance <= _matchDistance ? 2 : 0;

    if (!closeMatch)
    {
      _class.setMiss();
    }
    else if (evidence >= 3)
    {
      _class.setMatch();
    }
    else if (evidence >= 1)
    {
      _class.setReview();
    }
    else
    {
      _class.setMiss();
    }
  }
  else
  {
    _class = externalMatchClass;
  }

  //if (Log::getInstance().getLevel() == Log::Debug)
  //{
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
  //}

  if (Log::getInstance().getLevel() == Log::Debug &&
      (e1->getTags().get("uuid") == _testUuid || e2->getTags().get("uuid") == _testUuid))
  {
    LOG_VARD(_eid1);
    LOG_VARD(e1->getTags().get("uuid"));
    LOG_VARD(e1->getTags());
    LOG_VARD(_eid2);
    LOG_VARD(e2->getTags().get("uuid"));
    LOG_VARD(e2->getTags());
    LOG_VARD(_typeScore);
    LOG_VARD(_typeMatch);
    LOG_VARD(_nameMatch);
    LOG_VARD(_exactNameMatch);
    LOG_VARD(_nameScore);
    LOG_VARD(addressMatch);
    LOG_VARD(closeMatch);
    LOG_VARD(_distance);
    LOG_VARD(_matchDistance);
    LOG_VARD(_reviewDistance);
    LOG_VARD(reviewDistancePlusCe);
    LOG_VARD(ce);
    LOG_VARD(e1->getCircularError());
    LOG_VARD(e2->getCircularError());
    LOG_VARD(_t1BestKvp);
    LOG_VARD(_t2BestKvp);
    LOG_VARD(evidence);
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

      LOG_DEBUG(matchType.toUpper() << " distance info for type: " << type);
      LOG_VARD(maxDistance);
      LOG_VARD(minimumDistance);
      LOG_VARD(averageDistance);
      LOG_VARD(numberOfEntries);
      LOG_VARD(distancesStr);
      LOG_DEBUG("**************************");
    }
  }
}

double PoiPolygonMatch::_getMatchDistanceForType(const QString /*typeKvp*/) const
{
  //dataset c
  /*if (typeKvp == "amenity=clinic")
  {
    return 10.0;
  }
  else if (typeKvp == "amenity=school" || typeKvp == "amenity=kindergarten")
  {
    return 20.0;
  }
  else if (typeKvp == "leisure=park")
  {
    return 10.0;
  }*/

  return _matchDistance;
}

double PoiPolygonMatch::_getReviewDistanceForType(const QString typeKvp) const
{
  const Tags polyTags = _map->getElement(_polyEid)->getTags();
  if (typeKvp == "leisure=park")
  {
    return 25.0;
  }
  else if ((typeKvp == "station=light_rail" || typeKvp == "railway=platform") &&
           (polyTags.get("subway") == "yes" || polyTags.get("tunnel") == "yes"))
  {
    return 150.0;
  }
  /*else if (typeKvp == "leisure=playground")
  {
    return 20.0;
  }
  else if (typeKvp == "sport=basketball")
  {
    return 50.0;
  }*/

  return _reviewDistance;
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

