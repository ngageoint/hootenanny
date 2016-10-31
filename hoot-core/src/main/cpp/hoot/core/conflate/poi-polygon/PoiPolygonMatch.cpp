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
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/AlphaShapeGenerator.h>

#include "PoiPolygonTypeScoreExtractor.h"
#include "PoiPolygonNameScoreExtractor.h"
#include "PoiPolygonDistance.h"
#include "PoiPolygonAddressScoreExtractor.h"
#include "PoiPolygonCustomMatchRules.h"
#include "PoiPolygonDistanceTruthRecorder.h"
#include "PoiPolygonDistanceExtractor.h"
#include "PoiPolygonAlphaShapeDistanceExtractor.h"

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

const unsigned int PoiPolygonMatch::MATCH_EVIDENCE_THRESHOLD = 3;
const unsigned int PoiPolygonMatch::REVIEW_EVIDENCE_THRESHOLD = 1;

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
_typeScore(-1.0),
_matchDistance(ConfigOptions().getPoiPolygonMatchDistanceThreshold()),
_reviewDistance(ConfigOptions().getPoiPolygonReviewDistanceThreshold()),
_nameScoreThreshold(ConfigOptions().getPoiPolygonMatchNameThreshold()),
_typeScoreThreshold(ConfigOptions().getPoiPolygonMatchTypeThreshold())
{
  _calculateMatch(eid1, eid2);
  //_calculateMatchWeka(eid1, eid2);
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
_typeScore(-1.0),
_matchDistance(ConfigOptions().getPoiPolygonMatchDistanceThreshold()),
_reviewDistance(ConfigOptions().getPoiPolygonReviewDistanceThreshold()),
_nameScoreThreshold(ConfigOptions().getPoiPolygonMatchNameThreshold()),
_typeScoreThreshold(ConfigOptions().getPoiPolygonMatchTypeThreshold()),
_polyNeighborIds(polyNeighborIds),
_poiNeighborIds(poiNeighborIds)
{
  _calculateMatch(eid1, eid2);
  //_calculateMatchWeka(eid1, eid2);
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
_typeScore(-1.0),
_matchDistance(matchDistance),
_reviewDistance(reviewDistance),
_nameScoreThreshold(nameScoreThreshold),
_typeScoreThreshold(typeScoreThreshold)
{
  _calculateMatch(eid1, eid2);
  //_calculateMatchWeka(eid1, eid2);
}

//TODO: define a poi poly poly category??

bool PoiPolygonMatch::isPoly(const Element& e)
{
  const Tags& tags = e.getTags();
  //check this first, b/c some of these excluded features may have multiple type vals
  const bool inABuildingOrPoiCategory =
    OsmSchema::getInstance().getCategories(tags).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  if (tags.get("barrier").toLower() == "fence"
      || tags.get("landuse").toLower() == "grass"
      || tags.get("natural").toLower() == "tree_row"
      || tags.get("natural").toLower() == "scrub"
      || tags.get("highway").toLower() == "residential")
  {
    return false;
  }
  return OsmSchema::getInstance().isArea(tags, e.getElementType()) &&
         (inABuildingOrPoiCategory || tags.getNames().size() > 0);
}

bool PoiPolygonMatch::isPoi(const Element& e)
{
  //TODO: I haven't figure out a way to bypass hgispoi defining these as poi's yet...need to fix
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
  return isPoly(e) && !OsmSchema::getInstance().isBuilding(e.getTags(), e.getElementType());
}

void PoiPolygonMatch::_categorizeElementsByGeometryType(const ElementId& eid1,
                                                        const ElementId& eid2)
{
  ConstElementPtr e1 = _map->getElement(eid1);
  ConstElementPtr e2 = _map->getElement(eid2);

  LOG_VART(_eid1);
  LOG_VART(e1->getTags().get("uuid"));
  LOG_VART(e1->getTags());
  LOG_VART(_eid2);
  LOG_VART(e2->getTags().get("uuid"));
  LOG_VART(e2->getTags());

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
}

void PoiPolygonMatch::_calculateMatchWeka(const ElementId& eid1, const ElementId& eid2)
{
  _class.setMiss();

  _categorizeElementsByGeometryType(eid1, eid2);

  try
  {
    const double distance = PoiPolygonDistanceExtractor().extract(*_map, _poi, _poly);
    const double convexPolydistance =
      PoiPolygonAlphaShapeDistanceExtractor().extract(*_map, _poi, _poly);
    const double nameScore = PoiPolygonNameScoreExtractor().extract(*_map, _poi, _poly);
    const double typeScore = PoiPolygonTypeScoreExtractor().extract(*_map, _poi, _poly);
    PoiPolygonAddressScoreExtractor addressScoreExtractor;
    addressScoreExtractor.setExactAddressMatching(false);
    const double addressScore = addressScoreExtractor.extract(*_map, _poi, _poly);

    //TODO: finish

  }
  catch (const geos::util::TopologyException& e)
  {
    if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
    {
      LOG_WARN(
        "Feature(s) passed to PoiPolygonMatchCreator caused topology exception on conversion "
        "to a geometry: " << _poly->toString() << "\n" << _poi->toString() << "\n" << e.what());
      _badGeomCount++;
    }
    return;
  }



  LOG_VART(_class);
  LOG_TRACE("**************************");
}

void PoiPolygonMatch::_calculateMatch(const ElementId& eid1, const ElementId& eid2)
{
  _class.setMiss();

  _categorizeElementsByGeometryType(eid1, eid2);

  const unsigned int evidence = _calculateEvidence(_poi, _poly);
  if (evidence >= MATCH_EVIDENCE_THRESHOLD)
  {
    _class.setMatch();
  }
  else if (evidence >= REVIEW_EVIDENCE_THRESHOLD)
  {
    _class.setReview();
  }

  LOG_VART(_class);
  LOG_TRACE("**************************");
}

unsigned int PoiPolygonMatch::_getDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  _distance = PoiPolygonDistanceExtractor().extract(*_map, poi, poly);
  if (_distance == -1.0)
  {
    _closeMatch = false;
    return 0;
  }

  //search radius taken from PoiPolygonMatchCreator
  PoiPolygonDistance distanceCalc(
    _matchDistance, _reviewDistance, poly->getTags(),
    poi->getCircularError() + ConfigOptions().getPoiPolygonReviewDistanceThreshold());
//  _matchDistance =
//    max(
//      distanceCalc.getMatchDistanceForType(_t1BestKvp),
//      distanceCalc.getMatchDistanceForType(_t2BestKvp));
  _reviewDistance =
    max(
      distanceCalc.getReviewDistanceForType(PoiPolygonTypeScoreExtractor::t1BestKvp),
      distanceCalc.getReviewDistanceForType(PoiPolygonTypeScoreExtractor::t2BestKvp));
  /*if (poi->getTags().get("station") != "light_rail" &&
      poi->getTags().get("amenity") != "fuel")
  {
    distanceCalc.modifyMatchDistanceForPolyDensity(_matchDistance);
    distanceCalc.modifyReviewDistanceForPolyDensity(_reviewDistance);
  }*/

  // calculate the 2 sigma for the distance between the two objects
  const double sigma1 = poi->getCircularError() / 2.0;
  const double sigma2 = poly->getCircularError() / 2.0;
  const double ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;
  const double reviewDistancePlusCe = _reviewDistance + ce;
  _closeMatch = _distance <= reviewDistancePlusCe;
  //close match is a requirement, regardless of the evidence count
  if (!_closeMatch)
  {
    return 0;
  }
  unsigned int evidence = _distance <= _matchDistance ? 2 : 0;

  LOG_VART(_matchDistance);
  LOG_VART(_reviewDistance);
  LOG_VART(poi->getCircularError());
  LOG_VART(poly->getCircularError());
  LOG_VART(reviewDistancePlusCe);
  LOG_VART(ce);
  LOG_VART(_distance);
  LOG_VART(_closeMatch);

  return evidence;
}

unsigned int PoiPolygonMatch::_getConvexPolyDistanceEvidence(ConstElementPtr poi,
                                                             ConstElementPtr poly)
{
  unsigned int evidence = 0;
  //don't really need to put a distance == -1.0 check here for now, since we're assuming
  //PoiPolygonDistanceExtractor will always be run before this one
  const double alphaShapeDist =
    PoiPolygonAlphaShapeDistanceExtractor().extract(*_map, poi, poly);
  evidence += alphaShapeDist <= _matchDistance ? 2 : 0;
  LOG_VART(alphaShapeDist);
  return evidence;
}

unsigned int PoiPolygonMatch::_getTypeEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  PoiPolygonTypeScoreExtractor typeScorer;
  typeScorer.setDistance(_distance);
  _typeScore = typeScorer.extract(*_map, poi, poly);
  if (poi->getTags().get("historic") == "monument")
  {
    //monuments can represent just about any poi type, so lowering this some to account for that
    _typeScoreThreshold = 0.3; //TODO: move to config
  }
  const bool typeMatch = _typeScore >= _typeScoreThreshold;
  unsigned int evidence = typeMatch ? 1 : 0;

  LOG_VART(_typeScore);
  LOG_VART(typeMatch);
  LOG_VART(_t1BestKvp);
  LOG_VART(_t2BestKvp);

  return evidence;
}

unsigned int PoiPolygonMatch::_getNameEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  _nameScore = PoiPolygonNameScoreExtractor().extract(*_map, poi, poly);
  const bool nameMatch = _nameScore >= _nameScoreThreshold;
  unsigned int evidence = nameMatch ? 1 : 0;

  LOG_VART(nameMatch);
  LOG_VART(_nameScore);

  return evidence;
}

unsigned int PoiPolygonMatch::_getAddressEvidence(ConstElementPtr poi,
                                                           ConstElementPtr poly)
{
  const double addressScore = PoiPolygonAddressScoreExtractor().extract(*_map, poi, poly);
  //TODO: move score threshold to config
  const bool addressMatch = addressScore == 1.0;
  LOG_VART(addressMatch);
  return addressMatch ? 1 : 0;
}

unsigned int PoiPolygonMatch::_calculateEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  unsigned int evidence = 0;

  //need to get type evidence first, b/c the best type kvp can influence the behavior of the
  //distance matching
  evidence += _getTypeEvidence(poi, poly);

  evidence += _getDistanceEvidence(poi, poly);

  //close match is a requirement, regardless of the evidence count
  if (!_closeMatch)
  {
    //don't exit early here if printing truths, b/c we need to calculate type match for that first
    //before exiting
    return 0;
  }

  evidence += _getNameEvidence(poi, poly);

  //no point in calc'ing the address match if we already have a match from the other evidence
  //LOG_VARD(_matchEvidenceThreshold);
  if (evidence < MATCH_EVIDENCE_THRESHOLD)
  {
    if (ConfigOptions().getPoiPolygonEnableAddressMatching())
    {
      evidence += _getAddressEvidence(poi, poly);
    }
    //TODO: move values to config
    if (evidence < MATCH_EVIDENCE_THRESHOLD && _distance <= 35.0 &&
        poi->getTags().get("amenity") == "school" && OsmSchema::getInstance().isBuilding(poly))
    {
      evidence += _getConvexPolyDistanceEvidence(poi, poly);
    }
  }

  if (evidence == 0)
  {
    if (_nameScore >= 0.4 && _typeScore >= 0.6) //TODO: move values to config
    {
      evidence++;
    }
    else if (ConfigOptions().getPoiPolygonEnableCustomMatchRules())
    {
      PoiPolygonCustomMatchRules matchRules(_map, _polyNeighborIds, _poiNeighborIds, _distance);
      matchRules.collectInfo(poi, poly);
      if (matchRules.ruleTriggered())
      {
        evidence++;
      }
    }
  }

  LOG_VART(evidence);
  return evidence;
}

void PoiPolygonMatch::printMatchDistanceInfo()
{
  PoiPolygonDistanceTruthRecorder::printMatchDistanceInfo();
}

void PoiPolygonMatch::resetMatchDistanceInfo()
{
  PoiPolygonDistanceTruthRecorder::resetMatchDistanceInfo();
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

