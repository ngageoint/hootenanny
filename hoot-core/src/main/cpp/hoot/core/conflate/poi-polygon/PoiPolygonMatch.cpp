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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonMatch.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Log.h>

#include "extractors/PoiPolygonTypeScoreExtractor.h"
#include "extractors/PoiPolygonNameScoreExtractor.h"
#include "PoiPolygonDistance.h"
#include "extractors/PoiPolygonAddressScoreExtractor.h"
#include "PoiPolygonReviewReducer.h"
#include "PoiPolygonAdvancedMatcher.h"
#include "PoiPolygonDistanceTruthRecorder.h"
#include "extractors/PoiPolygonDistanceExtractor.h"
#include "extractors/PoiPolygonAlphaShapeDistanceExtractor.h"

using namespace std;

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, ConstMatchThresholdPtr threshold,
                                 boost::shared_ptr<const PoiPolygonRfClassifier> rf,
                                 const set<ElementId>& polyNeighborIds,
                                 const set<ElementId>& poiNeighborIds) :
Match(threshold),
_map(map),
_matchEvidenceThreshold(3),
_reviewEvidenceThreshold(1),
_closeMatch(false),
_typeScore(-1.0),
_nameScore(-1.0),
_addressScore(-1.0),
_polyNeighborIds(polyNeighborIds),
_poiNeighborIds(poiNeighborIds),
_rf(rf)
{
}

void PoiPolygonMatch::setMatchDistanceThreshold(double distance)
{
  if (distance < 0.0 || distance > 5000.0)  //upper limit is arbitrary
  {
    throw IllegalArgumentException(
      QString("Invalid POI/Polygon match distance configuration option value: ") + distance +
      QString(".  0 <= value <= 5000"));
  }
  _matchDistanceThreshold = distance;
}

void PoiPolygonMatch::setReviewDistanceThreshold(double distance)
{
  if (distance < 0.0 || distance > 5000.0)  //upper limit is arbitrary
  {
    throw IllegalArgumentException(
      QString("Invalid POI/Polygon review distance configuration option value: ") + distance +
      QString(".  0 <= value <= 5000"));
  }
  _reviewDistanceThreshold = distance;
}

void PoiPolygonMatch::setNameScoreThreshold(double threshold)
{
  if (threshold < 0.01 || threshold > 1.0)
  {
    throw IllegalArgumentException(
      QString("Invalid POI/Polygon name score threshold configuration option value: ") + threshold +
      QString(".  0.01 <= value <= 1.0"));
  }
  _nameScoreThreshold = threshold;
}

void PoiPolygonMatch::setTypeScoreThreshold(double threshold)
{
  if (threshold < 0.01 || threshold > 1.0)
  {
    throw IllegalArgumentException(
      QString("Invalid POI/Polygon type score threshold configuration option value: ") + threshold +
      QString(".  0.01 <= value <= 1.0"));
  }
  _typeScoreThreshold = threshold;
}

void PoiPolygonMatch::setReviewIfMatchedTypes(const QStringList& types)
{
  for (int i = 0; i < types.size(); i++)
  {
    const QString kvp = types[i];
    if (kvp.trimmed().isEmpty() || !kvp.contains("="))
    {
      throw IllegalArgumentException(
        QString("Invalid POI/Polygon review if matched type configuration option value: ") + kvp);
    }
    const QStringList kvpList = kvp.split("=");
    if (kvpList.size() != 2 || kvpList[0].trimmed().isEmpty() || kvpList[1].trimmed().isEmpty())
    {
      throw IllegalArgumentException(
        QString("Invalid POI/Polygon review if matched type configuration option value: ") + kvp);
    }
  }
  _reviewIfMatchedTypes = types;
}

void PoiPolygonMatch::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setMatchDistanceThreshold(config.getPoiPolygonMatchDistanceThreshold());
  setReviewDistanceThreshold(config.getPoiPolygonReviewDistanceThreshold());
  setNameScoreThreshold(config.getPoiPolygonNameScoreThreshold());
  setTypeScoreThreshold(config.getPoiPolygonTypeScoreThreshold());
  setReviewIfMatchedTypes(config.getPoiPolygonReviewIfMatchedTypes());
  setEnableAdvancedMatching(config.getPoiPolygonEnableAdvancedMatching());
  setEnableReviewReduction(config.getPoiPolygonEnableReviewReduction());
  const int matchEvidenceThreshold = config.getPoiPolygonMatchEvidenceThreshold();
  if (matchEvidenceThreshold < 1 || matchEvidenceThreshold > 4)
  {
    throw HootException(
      "Invalid value for POI/Polygon match evidence threshold: " +
      QString::number(matchEvidenceThreshold) + ".  Valid values are 1 to 4.");
  }
  setMatchEvidenceThreshold(matchEvidenceThreshold);
  LOG_VART(_matchEvidenceThreshold);
  const int reviewEvidenceThreshold = config.getPoiPolygonReviewEvidenceThreshold();
  if (reviewEvidenceThreshold < 0 || reviewEvidenceThreshold > matchEvidenceThreshold - 1)
  {
    throw HootException(
      "Invalid value for POI/Polygon review evidence threshold: " +
      QString::number(reviewEvidenceThreshold) + ".  Valid values are 0 to " +
      QString::number(matchEvidenceThreshold - 1) + ".");
  }
  setReviewEvidenceThreshold(reviewEvidenceThreshold);
  LOG_VART(_reviewEvidenceThreshold);
}

bool PoiPolygonMatch::isPoly(const Element& e)
{
  const Tags& tags = e.getTags();
  //types we don't care about at all - see #1172 as to why this can't be handled in the schema
  //files
  if (tags.get("barrier").toLower() == "fence"
      || tags.get("landuse").toLower() == "grass"
      || tags.get("natural").toLower() == "tree_row"
      || tags.get("natural").toLower() == "scrub"
      || tags.get("highway").toLower() == "residential")
  {
    return false;
  }
  const bool inABuildingOrPoiCategory =
    OsmSchema::getInstance().getCategories(tags).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  //isArea includes building too
  const bool isPoly =
    OsmSchema::getInstance().isArea(tags, e.getElementType()) &&
      (inABuildingOrPoiCategory || tags.getNames().size() > 0);
  LOG_VART(e);
  LOG_VART(isPoly);
  return isPoly;
}

bool PoiPolygonMatch::isPoi(const Element& e)
{
  const Tags& tags = e.getTags();
  //types we don't care about at all - see #1172 as to why this can't be handled in the schema
  //files
  if (tags.get("natural").toLower() == "tree"
      || tags.get("amenity").toLower() == "drinking_water"
      || tags.get("amenity").toLower() == "bench"
      || tags.contains("traffic_sign")
      || tags.get("amenity").toLower() == "recycling")
  {
    return false;
  }
  const bool inABuildingOrPoiCategory =
    OsmSchema::getInstance().getCategories(tags).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  bool isPoi =
    e.getElementType() == ElementType::Node &&
      (inABuildingOrPoiCategory || tags.getNames().size() > 0);

  if (!isPoi && e.getElementType() == ElementType::Node &&
      ConfigOptions().getPoiPolygonPromotePointsWithAddressesToPois() &&
      PoiPolygonAddressScoreExtractor::hasAddress(e))
  {
    isPoi = true;
  }

  LOG_VART(e);
  LOG_VART(isPoi);
  return isPoi;
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
    LOG_VART(e1->toString());
    LOG_VART(e2->toString());
    throw IllegalArgumentException("Expected a POI & polygon, got: " + eid1.toString() + " " +
                                   eid2.toString());
  }
}

//Weka didn't help any with improving this matching.  Leaving this method here in case anyone
//wants to explore in weka with this again.
void PoiPolygonMatch::calculateMatchWeka(const ElementId& /*eid1*/, const ElementId& /*eid2*/)
{
//  _class.setMiss();

//  _categorizeElementsByGeometryType(eid1, eid2);

//  try
//  {
//    const double distance = PoiPolygonDistanceExtractor().extract(*_map, _poi, _poly);
//    //const double convexPolydistance =
//      //PoiPolygonAlphaShapeDistanceExtractor().extract(*_map, _poi, _poly);
//    const double nameScore = PoiPolygonNameScoreExtractor().extract(*_map, _poi, _poly);
//    const double typeScore = PoiPolygonTypeScoreExtractor().extract(*_map, _poi, _poly);
//    PoiPolygonAddressScoreExtractor addressScoreExtractor;
//    addressScoreExtractor.setExactAddressMatching(false);
//    const double addressScore = addressScoreExtractor.extract(*_map, _poi, _poly);

//    /*if (distance <= 10.698997)
//    {
//      if (typeScore > 0.576)
//      {
//        if (nameScore > 0.782878)
//        {
//          if (typeScore <= 0.8)
//          {
//            if (typeScore <= 0.64)
//            {
//              _class.setMatch();
//            }
//          }
//          else
//          {
//            if (distance <= 0.000371)
//            {
//              _class.setMatch();
//            }
//          }
//        }
//      }
//    }*/
//  }
//  catch (const geos::util::TopologyException& e)
//  {
//    //if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
//    //{
//      LOG_WARN(
//        "Feature(s) passed to PoiPolygonMatchCreator caused topology exception on conversion "
//        "to a geometry: " << _poly->toString() << "\n" << _poi->toString() << "\n" << e.what());
//      //_badGeomCount++;
//    //}
//    return;
//  }

//  LOG_VART(_class);
//  LOG_TRACE("**************************");
}

bool PoiPolygonMatch::_featureHasReviewIfMatchedType(ConstElementPtr element) const
{
  if (_reviewIfMatchedTypes.isEmpty())
  {
    return false;
  }

  const Tags& tags = element->getTags();
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    if (_reviewIfMatchedTypes.contains(it.key() + "=" + it.value()))
    {
      return true;
    }
  }
  return false;
}

void PoiPolygonMatch::calculateMatch(const ElementId& eid1, const ElementId& eid2)
{  
  _class.setMiss();

  _categorizeElementsByGeometryType(eid1, eid2);

  //allow for auto marking features with certain types for review if they get matched
  const bool foundReviewIfMatchedType =
    _featureHasReviewIfMatchedType(_poi) || _featureHasReviewIfMatchedType(_poly);
  LOG_VART(foundReviewIfMatchedType);

  unsigned int evidence = _calculateEvidence(_poi, _poly);

  //no point in trying to reduce reviews if we're still at a miss here
  if (_enableReviewReduction && evidence >= _reviewEvidenceThreshold)
  {
    PoiPolygonReviewReducer reviewReducer(
      _map, _polyNeighborIds, _poiNeighborIds, _distance, _nameScoreThreshold,
      _nameScore >= _nameScoreThreshold, _nameScore == 1.0, _typeScore,
      _typeScore >= _typeScoreThreshold, _matchDistanceThreshold);
    if (reviewReducer.triggersRule(_poi, _poly))
    {
      evidence = 0;
    }
  }

  if (evidence >= _matchEvidenceThreshold)
  {
    if (!foundReviewIfMatchedType)
    {
      _class.setMatch();
    }
    else
    {
      _class.setReview();
    }
  }
  else if (evidence >= _reviewEvidenceThreshold)
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
    _matchDistanceThreshold, _reviewDistanceThreshold, poly->getTags(),
    poi->getCircularError() + ConfigOptions().getPoiPolygonReviewDistanceThreshold());
  //type based match distance changes didn't have any positive effect experimentally; leaving it
  //commented out here in case there is need for further examination
//  _matchDistanceThreshold =
//    max(
//      distanceCalc.getMatchDistanceForType(_t1BestKvp),
//      distanceCalc.getMatchDistanceForType(_t2BestKvp));
  _reviewDistanceThreshold =
    max(
      distanceCalc.getReviewDistanceForType(_poi->getTags()),
      distanceCalc.getReviewDistanceForType(_poly->getTags()));
  //density based distance changes didn't have any positive effect experimentally; leaving it
  //commented out here in case there is need for further examination
  /*if (poi->getTags().get("station") != "light_rail" &&
      poi->getTags().get("amenity") != "fuel")
  {
    distanceCalc.modifyMatchDistanceForPolyDensity(_matchDistanceThreshold);
    distanceCalc.modifyReviewDistanceForPolyDensity(_reviewDistanceThreshold);
  }*/

  // calculate the 2 sigma for the distance between the two objects
  const double poiSigma = poi->getCircularError() / 2.0;
  const double polySigma = poly->getCircularError() / 2.0;
  const double sigma = sqrt(poiSigma * poiSigma + polySigma * polySigma);
  const double combinedCircularError2Sigma = sigma * 2;
  const double reviewDistancePlusCe = _reviewDistanceThreshold + combinedCircularError2Sigma;
  _closeMatch = _distance <= reviewDistancePlusCe;
  //close match is a requirement for any matching, regardless of the final total evidence count
  if (!_closeMatch)
  {
    return 0;
  }

  LOG_VART(_matchDistanceThreshold);
  LOG_VART(_reviewDistanceThreshold);
  LOG_VART(poi->getCircularError());
  LOG_VART(poly->getCircularError());
  LOG_VART(reviewDistancePlusCe);
  LOG_VART(combinedCircularError2Sigma);
  LOG_VART(_distance);
  LOG_VART(_closeMatch);

  return _distance <= _matchDistanceThreshold ? 2u : 0u;
}

unsigned int PoiPolygonMatch::_getConvexPolyDistanceEvidence(ConstElementPtr poi,
                                                             ConstElementPtr poly)
{
  //don't really need to put a distance == -1.0 check here for now, since we're assuming
  //PoiPolygonDistanceExtractor will always be run before this one
  const double alphaShapeDist =
    PoiPolygonAlphaShapeDistanceExtractor().extract(*_map, poi, poly);
  LOG_VART(alphaShapeDist);
  return alphaShapeDist <= _matchDistanceThreshold ? 2u : 0u;
}

unsigned int PoiPolygonMatch::_getTypeEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  PoiPolygonTypeScoreExtractor typeScorer;
  typeScorer.setFeatureDistance(_distance);
  typeScorer.setTypeScoreThreshold(_typeScoreThreshold);
  _typeScore = typeScorer.extract(*_map, poi, poly);
  //if (poi->getTags().get("historic") == "monument")
  //{
    //monuments can represent just about any poi type, so lowering this some to account for that
//    _typeScoreThreshold = _typeScoreThreshold * 0.42; //determined experimentally
    //_typeScoreThreshold = 0.3; //this could be moved to a config
  //}
  const bool typeMatch = _typeScore >= _typeScoreThreshold;
  LOG_VART(typeMatch);
  LOG_VART(PoiPolygonTypeScoreExtractor::poiBestKvp);
  LOG_VART(PoiPolygonTypeScoreExtractor::polyBestKvp);
  return typeMatch ? 1u : 0u;
}

unsigned int PoiPolygonMatch::_getNameEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  PoiPolygonNameScoreExtractor nameScorer;
  nameScorer.setNameScoreThreshold(_nameScoreThreshold);
  _nameScore = nameScorer.extract(*_map, poi, poly);
  const bool nameMatch = _nameScore >= _nameScoreThreshold;
  LOG_VART(nameMatch);
  return nameMatch ? 1u : 0u;
}

unsigned int PoiPolygonMatch::_getAddressEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  _addressScore = PoiPolygonAddressScoreExtractor().extract(*_map, poi, poly);
  const bool addressMatch = _addressScore == 1.0;
  LOG_VART(addressMatch);
  return addressMatch ? 1u : 0u;
}

unsigned int PoiPolygonMatch::_calculateEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  unsigned int evidence = 0;

  evidence += _getDistanceEvidence(poi, poly);
  //see comment in _getDistanceEvidence
  if (!_closeMatch)
  {
    return 0;
  }

  //The operations from here are on down are roughly ordered by increasing runtime complexity.

  evidence += _getNameEvidence(poi, poly);
  //if we already have a match, no point in doing more calculations
  if (_reviewIfMatchedTypes.isEmpty() && evidence >= _matchEvidenceThreshold)
  {
    return evidence;
  }

  evidence += _getTypeEvidence(poi, poly);
  if (evidence >= _matchEvidenceThreshold)
  {
    return evidence;
  }

  evidence += _getAddressEvidence(poi, poly);
  if (evidence >= _matchEvidenceThreshold)
  {
    return evidence;
  }

  //We only want to run this if the previous match distance calculation was too large.
  //Tightening up the requirements for running the convex poly calculation here to improve
  //runtime.  These requirements can possibly be removed at some point in the future, if proven
  //necessary.  The school requirement definitely seems too type specific (this type of evidence
  //has actually only been found with school pois in one test dataset so far), but when
  //removing it scores dropped for other datasets.  So, more investigation needs to be done to
  //clean the school restriction up (see #1173).
  if (evidence == 0 && _distance <= 35.0 && poi->getTags().get("amenity") == "school" &&
      OsmSchema::getInstance().isBuilding(poly))
  {
    evidence += _getConvexPolyDistanceEvidence(poi, poly);
    if (evidence >= _matchEvidenceThreshold)
    {
      return evidence;
    }
  }

  /*if (evidence == 0)
  {
    //This is kind of a consolation prize...if there was no match, but name and type score have
    //moderately high values, then make it a match.
    if (_nameScore >= 0.4 && _typeScore >= 0.6) //determined experimentally
    {
      evidence++;
    }
  }*/

  //no point in trying to increase evidence if we're already at a match
  if (_enableAdvancedMatching && evidence < _matchEvidenceThreshold)
  {
    PoiPolygonAdvancedMatcher advancedMatcher(
      _map, _polyNeighborIds, _poiNeighborIds, _distance);
    if (advancedMatcher.triggersRule(_poi, _poly))
    {
      evidence++;
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

map<QString, double> PoiPolygonMatch::getFeatures(const ConstOsmMapPtr& m) const
{
  return _rf->getFeatures(m, _eid1, _eid2);
}

QString PoiPolygonMatch::toString() const
{
  return
    QString("PoiPolygonMatch %1 %2 P: %3, distance: %4, close match: %5, type score: %6, name score: %7, address score: %8")
      .arg(_poi->getElementId().toString())
      .arg(_poly->getElementId().toString())
      .arg(_class.toString())
      .arg(_distance)
      .arg(_closeMatch)
      .arg(_typeScore)
      .arg(_nameScore)
      .arg(_addressScore);
}

}

