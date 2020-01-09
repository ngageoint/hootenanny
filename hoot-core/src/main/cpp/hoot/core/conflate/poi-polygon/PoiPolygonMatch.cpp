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
#include "PoiPolygonMatch.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonDistanceTruthRecorder.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonReviewReducer.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonAlphaShapeDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonDistanceExtractor.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/MultiUseBuildingCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchema.h>

// Qt
#include <QElapsedTimer>
#include <QStringBuilder>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Match, PoiPolygonMatch)

QString PoiPolygonMatch::_matchName = "POI to Polygon";
std::shared_ptr<ToEnglishTranslator> PoiPolygonMatch::_translator;

long PoiPolygonMatch::matchesProcessed = 0;
long PoiPolygonMatch::distanceMatches = 0;
long PoiPolygonMatch::typeMatches = 0;
long PoiPolygonMatch::noTypeFoundCount = 0;
long PoiPolygonMatch::nameMatches = 0;
long PoiPolygonMatch::namesProcessed = 0;
long PoiPolygonMatch::nameMatchCandidates = 0;
long PoiPolygonMatch::addressesProcessed = 0;
long PoiPolygonMatch::addressMatches = 0;
long PoiPolygonMatch::addressMatchCandidates = 0;
long PoiPolygonMatch::phoneNumberMatches = 0;
long PoiPolygonMatch::phoneNumbersProcesed = 0;
long PoiPolygonMatch::phoneNumberMatchCandidates = 0;
long PoiPolygonMatch::convexPolyDistanceMatches = 0;
long PoiPolygonMatch::numReviewReductions = 0;

PoiPolygonMatch::PoiPolygonMatch() :
Match()
{
}

PoiPolygonMatch::PoiPolygonMatch(ConstMatchThresholdPtr threshold) :
Match(threshold)
{
}

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, ConstMatchThresholdPtr threshold,
                                 std::shared_ptr<const PoiPolygonRfClassifier> rf,
                                 PoiPolygonInfoCachePtr infoCache,
                                 const set<ElementId>& polyNeighborIds) :
Match(threshold),
_map(map),
_e1IsPoi(false),
_matchEvidenceThreshold(3),
_reviewEvidenceThreshold(1),
_distance(-1.0),
_matchDistanceThreshold(-1.0),
_reviewDistanceThreshold(-1.0),
_reviewDistancePlusCe(-1.0),
_closeDistanceMatch(false),
_typeScore(-1.0),
_typeScoreThreshold(-1.0),
_reviewIfMatchedTypes(QStringList()),
_nameScore(-1.0),
_nameScoreThreshold(-1.0),
_addressScore(-1.0),
//leaving this false by default due to libpostals startup time
_addressMatchEnabled(false),
_phoneNumberScore(-1.0),
_phoneNumberMatchEnabled(true),
_polyNeighborIds(polyNeighborIds),
_enableReviewReduction(true),
_disableSameSourceConflation(false),
_disableSameSourceConflationMatchTagKeyPrefixOnly(true),
_sourceTagKey(""),
_reviewMultiUseBuildings(false),
_rf(rf),
_explainText(""),
_infoCache(infoCache),
_timingThreshold(1000000)    //nanoseconds
{
  LOG_VART(_infoCache.get());

  _typeScorer.reset(new PoiPolygonTypeScoreExtractor(_infoCache));
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
    QString kvp = types[i];

    //As a UI workaround, we're allowing the format "key,value" to be used instead of "key=value".
    kvp.replace(",", "=");

    LOG_VART(kvp);
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
    _reviewIfMatchedTypes.append(kvp);
  }
  LOG_VART(_reviewIfMatchedTypes);
}

void PoiPolygonMatch::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  setMatchDistanceThreshold(config.getPoiPolygonMatchDistanceThreshold());
  setReviewDistanceThreshold(config.getPoiPolygonReviewDistanceThreshold());
  setNameScoreThreshold(config.getPoiPolygonNameScoreThreshold());
  setTypeScoreThreshold(config.getPoiPolygonTypeScoreThreshold());

  setReviewIfMatchedTypes(config.getPoiPolygonReviewIfMatchedTypes());
  setDisableSameSourceConflation(config.getPoiPolygonDisableSameSourceConflation());
  setDisableSameSourceConflationMatchTagKeyPrefixOnly(
    config.getPoiPolygonDisableSameSourceConflationMatchTagKeyPrefixOnly());
  setSourceTagKey(config.getPoiPolygonSourceTagKey());

  setReviewMultiUseBuildings(config.getPoiPolygonReviewMultiuseBuildings());

  setEnableReviewReduction(config.getPoiPolygonEnableReviewReduction());

  const int matchEvidenceThreshold = config.getPoiPolygonMatchEvidenceThreshold();
  if (matchEvidenceThreshold < 1 || matchEvidenceThreshold > 4)
  {
    throw HootException(
      "Invalid value for POI/Polygon match evidence threshold: " +
      QString::number(matchEvidenceThreshold) + ".  Valid values are 1 to 4.");
  }
  setMatchEvidenceThreshold(matchEvidenceThreshold);
  if (_matchEvidenceThreshold == 1)
  {
    // reviews are effectively turned off if match thresh = 1
    _reviewEvidenceThreshold = 0;
  }
  else
  {
    const int reviewEvidenceThreshold = config.getPoiPolygonReviewEvidenceThreshold();
    if (reviewEvidenceThreshold < 1 || reviewEvidenceThreshold > 3)
    {
      throw HootException(
        "Invalid value for POI/Polygon review evidence threshold: " +
        QString::number(reviewEvidenceThreshold) + ".  Valid values are 1 to 3.");
    }
    setReviewEvidenceThreshold(reviewEvidenceThreshold);
  }
  if (_reviewEvidenceThreshold >= _matchEvidenceThreshold)
  {
    throw HootException(
      "Value for POI/Polygon review evidence threshold: " +
      QString::number(_reviewEvidenceThreshold) +
      " is greater than or equal to value for POI/Polyon match evidence threshold: " +
      QString::number(_matchEvidenceThreshold) + ".");
  }
  LOG_VART(_matchEvidenceThreshold);
  LOG_VART(_reviewEvidenceThreshold);

  _addressMatchEnabled = config.getPoiPolygonAddressMatchEnabled();
  if (_addressMatchEnabled)
  {
    _addressScorer.setConfiguration(conf);
  }
  if (!_typeScorer)
  {
    _typeScorer.reset(new PoiPolygonTypeScoreExtractor(_infoCache));
  }
  _typeScorer->setConfiguration(conf);
  _nameScorer.setConfiguration(conf);
  _phoneNumberMatchEnabled = config.getPoiPolygonPhoneNumberMatchEnabled();
  if (_phoneNumberMatchEnabled)
  {
    _phoneNumberScorer.setConfiguration(conf);
  }
}

void PoiPolygonMatch::_categorizeElementsByGeometryType()
{
  ConstElementPtr e1 = _map->getElement(_eid1);
  ConstElementPtr e2 = _map->getElement(_eid2);

  LOG_VART(_eid1);
  LOG_VART(e1->getTags().get("uuid"));
  LOG_VART(e1->getTags());
  LOG_VART(_eid2);
  LOG_VART(e2->getTags().get("uuid"));
  LOG_VART(e2->getTags());

  if (_poiCrit.isSatisfied(e1) && _polyCrit.isSatisfied(e2))
  {
    assert(e1->getElementType() == ElementType::Node);
    _poi = std::dynamic_pointer_cast<const Node>(e1);
    _poly = e2;
    _e1IsPoi = true;
  }
  else if (_poiCrit.isSatisfied(e2) && _polyCrit.isSatisfied(e1))
  {
    assert(e2->getElementType() == ElementType::Node);
    _poi = std::dynamic_pointer_cast<const Node>(e2);
    _poly = e1;
    _e1IsPoi = false;
  }
  else
  {
    LOG_VART(e1->toString());
    LOG_VART(e2->toString());
    throw IllegalArgumentException("Expected a POI & polygon, got: " + _eid1.toString() + " " +
                                   _eid2.toString());
  }
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
    const QString kvp = it.key() % "=" % it.value();
    if (_reviewIfMatchedTypes.contains(kvp))
    {
      LOG_TRACE("Matched type for review: " << kvp);
      return true;
    }
  }
  return false;
}

bool PoiPolygonMatch::_inputFeaturesHaveSameSource() const
{
  LOG_VART(_disableSameSourceConflationMatchTagKeyPrefixOnly)

  const QString e1SourceVal = _map->getElement(_eid1)->getTags().get(_sourceTagKey).trimmed();
  const QString e2SourceVal = _map->getElement(_eid2)->getTags().get(_sourceTagKey).trimmed();
  LOG_VART(e1SourceVal);
  LOG_VART(e2SourceVal);

  if (e1SourceVal.isEmpty() || e2SourceVal.isEmpty())
  {
    LOG_TRACE("Both sources empty.  No feature source match.");
    return false;
  }
  else if (_disableSameSourceConflationMatchTagKeyPrefixOnly)
  {
    //using ':' as a hardcoded source prefix val delimiter since it seems to be a common OSM
    //convention
    if (!e1SourceVal.contains(":") || !e2SourceVal.contains(":"))
    {
      LOG_TRACE(
        "Source prefix match enabled and at least one feature has no source prefix.  No feature source match.");
      return false;
    }
    else
    {
      const QString e1SourceValPrefix = e1SourceVal.split(":")[0].trimmed();
      const QString e2SourceValPrefix = e2SourceVal.split(":")[0].trimmed();
      LOG_VART(e1SourceValPrefix);
      LOG_VART(e2SourceValPrefix);
      if (e1SourceValPrefix.toLower() == e2SourceValPrefix.toLower())
      {
        LOG_TRACE("Feature source prefixes match.");
        return true;
      }
    }
  }
  else if (e1SourceVal.toLower() == e2SourceVal.toLower())
  {
    LOG_TRACE("Feature sources have an exact match.");
    return true;
  }

  LOG_TRACE("No feature source match.");
  return false;
}

bool PoiPolygonMatch::_skipForReviewTypeDebugging() const
{
  if (!PoiPolygonSchema::hasSpecificType(_poi) || !PoiPolygonSchema::hasSpecificType(_poly))
  {
    return true;
  }

  QStringList reviewTypeIgnoreList;
  if (_poi->getTags().hasAnyKvp(reviewTypeIgnoreList) ||
      _poly->getTags().hasAnyKvp(reviewTypeIgnoreList))
  {
    return true;
  }
  return false;
}

void PoiPolygonMatch::calculateMatch(const ElementId& eid1, const ElementId& eid2)
{  
  //for testing only
//  ConstElementPtr e1 = _map->getElement(eid1);
//  ConstElementPtr e2 = _map->getElement(eid2);
//  const bool oneElementIsRelation =
//    e1->getElementType() == ElementType::Relation ||
//    e2->getElementType() == ElementType::Relation;
  //QElapsedTimer timer;
  matchesProcessed++;
  _explainText = "";
  _class.setMiss();
  _eid1 = eid1;
  _eid2 = eid2;

  //if the options was activated to not conflate features with the same source tag, then exit
  //out with a miss now
  if (_disableSameSourceConflation && _inputFeaturesHaveSameSource())
  {
    return;
  }

  _categorizeElementsByGeometryType();

  //FOR REDUCING REVIEWS DURING REVIEW TYPE DEBUGGING ONLY!
//  if (_skipForReviewTypeDebugging())
//  {
//    return;
//  }

  //allow for auto marking features with certain types for review if they get matched
  //timer.restart();
  const bool foundReviewIfMatchedType =
    _featureHasReviewIfMatchedType(_poi) || _featureHasReviewIfMatchedType(_poly);
  LOG_VART(foundReviewIfMatchedType);
//  if (timer.nsecsElapsed() > _timingThreshold)
//  {
//    LOG_DEBUG("foundReviewIfMatchedType: " << timer.nsecsElapsed());
//  }

  //timer.restart();
  unsigned int evidence = _calculateEvidence(_poi, _poly);
  LOG_VART(evidence);
//  if (timer.elapsed() > _timingThreshold)
//  {
//    LOG_DEBUG("_calculateEvidence: " << timer.elapsed());
//  }

  bool runReviewReduction = _enableReviewReduction;
  if (// No point in trying to reduce reviews if we're still at a miss here. Also, if the review
      // threshold = 0, that means we don't want any reviews at all...so no point in trying to
      //reduce them.
      (_reviewEvidenceThreshold > 0 && evidence < _reviewEvidenceThreshold) ||
      // If the match threshold = 1, then we throw out all reviews and no reason to try and reduce
      // any.
      (_matchEvidenceThreshold == 1 && evidence < _matchEvidenceThreshold))
  {
    runReviewReduction = false;
  }
  LOG_VART(runReviewReduction);
  if (runReviewReduction)
  {
    //timer.restart();
    // this constructor has gotten a little out of hand...
    PoiPolygonReviewReducer reviewReducer(
      _map, _polyNeighborIds, _distance, _nameScoreThreshold, _nameScore,
      _nameScore >= _nameScoreThreshold, _nameScore == 1.0, _typeScoreThreshold, _typeScore,
      _typeScore >= _typeScoreThreshold, _matchDistanceThreshold, _addressScore == 1.0,
      _addressMatchEnabled, _infoCache);
    reviewReducer.setConfiguration(conf());
    if (reviewReducer.triggersRule(_poi, _poly))
    {
      evidence = 0;
      // TODO: b/c this is a miss, don't think it will actually get added to the output anywhere...
      _explainText =
        "Match score automatically dropped by review reduction rule: " +
        reviewReducer.getTriggeredRuleDescription();
    }
    numReviewReductions++;
//    if (timer.elapsed() > _timingThreshold)
//    {
//      LOG_DEBUG("PoiPolygonReviewReducer: " << timer.elapsed());
//    }
  }
  LOG_VART(evidence);

  if (evidence >= _matchEvidenceThreshold)
  {
    if (!foundReviewIfMatchedType)
    {
      LOG_VART(_reviewMultiUseBuildings);
      LOG_VART(MultiUseBuildingCriterion().isSatisfied(_poly));
      //only do the multi-use check on the poly
      if (_reviewMultiUseBuildings &&
          _infoCache->hasCriterion(
            _poly, QString::fromStdString(MultiUseBuildingCriterion::className())))
      {
        _class.setReview();
        _explainText = "Match involves a multi-use building.";
      }
      else
      {
        _class.setMatch();
      }
    }
    else// if (oneElementIsRelation) //for testing only
    {
      _class.setReview();
      _explainText =
        "Feature contains tag specified for review from list: " % _reviewIfMatchedTypes.join(";");
    }
  }
  else if (evidence >= _reviewEvidenceThreshold && _reviewEvidenceThreshold > 0)
           //&& oneElementIsRelation) //for testing only
  {
    _class.setReview();

    if (_explainText.isEmpty())
    {
      // TODO: move this somewhere else?...or start values out at 0.0?
      if (_typeScore < 0.0)
      {
        _typeScore = 0.0;
      }
      if (_nameScore < 0.0)
      {
        _nameScore = 0.0;
      }
      if (_addressScore < 0.0)
      {
        _addressScore = 0.0;
      }
      const QString typeMatchStr = _typeScore >= _typeScoreThreshold ? "yes" : "no";
      const QString nameMatchStr = _nameScore >= _nameScoreThreshold ? "yes" : "no";
      const QString addressMatchStr = _addressScore >= 1.0 ? "yes" : "no";
      const QString distanceMatchStr = _distance <= _matchDistanceThreshold ? "yes" : "no";
      _explainText =
        QString("Similarity score: %1; less than match threshold: %2; meets review threshold: %3. distance: %4 (%5m; score: 2/2), type: %6 (score: %7/1), name: %8 (score: %9/1), address: %10 (score: %11/1). Max distance allowed for match: %12m, review: %13m.")
          .arg(evidence)
          .arg(_matchEvidenceThreshold)
          .arg(_reviewEvidenceThreshold)
          .arg(distanceMatchStr)
          .arg(round(_distance))
          .arg(typeMatchStr)
          .arg(QString::number(_typeScore))
          .arg(nameMatchStr)
          .arg(QString::number(_nameScore))
          .arg(addressMatchStr)
          .arg(QString::number(_addressScore))
          .arg(_matchDistanceThreshold)
          .arg(_reviewDistancePlusCe);
    }
  }
  else
  {
    _explainText = "";
  }

  LOG_TRACE("eid1: " << eid1 << ", eid2: " << eid2 << ", class: " << _class);
  LOG_TRACE("**************************");
}

MatchType PoiPolygonMatch::getType() const
{
  return _threshold->getType(_class);
}

unsigned int PoiPolygonMatch::_getDistanceEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  LOG_TRACE("Retrieving distance evidence...");

  _distance = PoiPolygonDistanceExtractor(_infoCache).extract(*_map, poi, poly);
  if (_distance == -1.0)
  {
    _closeDistanceMatch = false;
    _explainText = "Error calculating the distance between features.";
    return 0;
  }

  _reviewDistanceThreshold =
    max(
      _infoCache->getReviewDistance(_poi, poly->getTags(), _reviewDistanceThreshold),
      _infoCache->getReviewDistance(_poly, poly->getTags(), _reviewDistanceThreshold));

  //Tried type and density based match distance changes here too, but they didn't have any positive
  //effect.

  // calculate the 2 sigma for the distance between the two objects
  const double poiSigma = poi->getCircularError() / 2.0;
  const double polySigma = poly->getCircularError() / 2.0;
  const double sigma = sqrt(poiSigma * poiSigma + polySigma * polySigma);
  const double combinedCircularError2Sigma = sigma * 2;
  _reviewDistancePlusCe = _reviewDistanceThreshold + combinedCircularError2Sigma;
  _closeDistanceMatch = _distance <= _reviewDistancePlusCe;
  //close distance match is a requirement for any matching, regardless of the final total evidence
  //count
  if (!_closeDistanceMatch)
  {
    _explainText =
      "The distance between the features is more than the configured review distance plus circular error.";
    return 0;
  }

  LOG_VART(_matchDistanceThreshold);
  LOG_VART(_reviewDistanceThreshold);
  LOG_VART(poi->getCircularError());
  LOG_VART(poly->getCircularError());
  LOG_VART(_reviewDistancePlusCe);
  LOG_VART(combinedCircularError2Sigma);
  LOG_VART(_distance);
  LOG_VART(_closeDistanceMatch);

  //Tried weighting distance less when both POI's had specific types, but it hasn't helped so far.
  return _distance <= _matchDistanceThreshold ? 2u : 0u;
}

unsigned int PoiPolygonMatch::_getConvexPolyDistanceEvidence(ConstElementPtr poi,
                                                             ConstElementPtr poly)
{
  LOG_TRACE("Retrieving convex poly distance evidence...");

  //don't really need to put a distance == -1.0 check here for now, since we're assuming
  //PoiPolygonDistanceExtractor will always be run before this one
  const double alphaShapeDist =
    PoiPolygonAlphaShapeDistanceExtractor().extract(*_map, poi, poly);
  LOG_VART(alphaShapeDist);
  const bool convexPolyDistMatch = alphaShapeDist <= _matchDistanceThreshold;
  if (convexPolyDistMatch)
  {
    convexPolyDistanceMatches++;
  }
  return convexPolyDistMatch ? 2u : 0u;
}

unsigned int PoiPolygonMatch::_getTypeEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  LOG_TRACE("Retrieving type evidence...");

  _typeScorer->setFeatureDistance(_distance);
  _typeScorer->setTypeScoreThreshold(_typeScoreThreshold);
  _typeScore = _typeScorer->extract(*_map, poi, poly);
  const bool typeMatch = _typeScore >= _typeScoreThreshold;
  if (typeMatch)
  {
    typeMatches++;
  }
  if (_typeScorer->getNoTypeFound())
  {
    noTypeFoundCount++;
  }

  if (_typeScorer->getFailedMatchRequirements().size() > 0)
  {
    QString failedMatchTypes;
    for (int i = 0; i < _typeScorer->getFailedMatchRequirements().size(); i++)
    {
      failedMatchTypes += _typeScorer->getFailedMatchRequirements().at(i) % ", ";
    }
    failedMatchTypes.chop(2);

    QString explainBase = "Failed custom match requirements for: ";
    if (_explainText.isEmpty())
    {
      _explainText = explainBase % failedMatchTypes;
    }
    else
    {
      _explainText += ";" % explainBase % failedMatchTypes;
    }
  }

  LOG_VART(typeMatch);

  return typeMatch ? 1u : 0u;
}

unsigned int PoiPolygonMatch::_getNameEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  LOG_TRACE("Retrieving name evidence...");

  _nameScorer.setNameScoreThreshold(_nameScoreThreshold);
  _nameScore = _nameScorer.extract(*_map, poi, poly);
  const bool nameMatch = _nameScore >= _nameScoreThreshold;
  LOG_VART(nameMatch);
  if (nameMatch)
  {
    nameMatches++;
  }
  namesProcessed += _nameScorer.getNamesProcessed();
  if (_nameScorer.getMatchAttemptMade())
  {
    nameMatchCandidates++;
  }
  return nameMatch ? 1u : 0u;
}

unsigned int PoiPolygonMatch::_getAddressEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  LOG_TRACE("Retrieving address evidence...");

  _addressScore = _addressScorer.extract(*_map, poi, poly);
  const bool addressMatch = _addressScore == 1.0;
  LOG_VART(addressMatch);
  if (addressMatch)
  {
    addressMatches++;
  }
  addressesProcessed += _addressScorer.getAddressesProcessed();
  if (_addressScorer.getMatchAttemptMade())
  {
    addressMatchCandidates++;
  }
  return addressMatch ? 1u : 0u;
}

unsigned int PoiPolygonMatch::_getPhoneNumberEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  LOG_TRACE("Retrieving phone number evidence...");

  _phoneNumberScore = _phoneNumberScorer.extract(*_map, poi, poly);
  const bool phoneNumberMatch = _phoneNumberScore == 1.0;
  LOG_VART(phoneNumberMatch);
  if (phoneNumberMatch)
  {
    phoneNumberMatches++;
  }
  phoneNumbersProcesed += _phoneNumberScorer.getPhoneNumbersProcessed();
  if (_phoneNumberScorer.getMatchAttemptMade())
  {
    phoneNumberMatchCandidates++;
  }
  return phoneNumberMatch ? 1u : 0u;
}

unsigned int PoiPolygonMatch::_calculateEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  //LOG_VART(poi);
  //LOG_VART(poly);

  //QElapsedTimer timer;

  unsigned int evidence = 0;

  //timer.restart();
  evidence += _getDistanceEvidence(poi, poly);
//  if (timer.nsecsElapsed() > _timingThreshold)
//  {
//    LOG_DEBUG("_getDistanceEvidence: " << timer.nsecsElapsed());
//  }
  //see comment in _getDistanceEvidence
  if (!_closeDistanceMatch)
  {
    return 0;
  }
  distanceMatches++;

  //The operations from here are on down are roughly ordered by increasing runtime complexity.

  //timer.restart();
  evidence += _getNameEvidence(poi, poly);
//  if (timer.nsecsElapsed() > _timingThreshold)
//  {
//    LOG_DEBUG("_getNameEvidence: " << timer.nsecsElapsed());
//  }
  // Used to allow for kicking out of the method once enough evidence was accumulated for a match
  // as a runtime optimization.  However, that results in incomplete scoring information passed to
  // the review reducer, so have since disabled. It also causes some regression tests to fail.
//  if (_reviewIfMatchedTypes.isEmpty() && evidence >= _matchEvidenceThreshold)
//  {
//    return evidence;
//  }
  //timer.restart();
  evidence += _getTypeEvidence(poi, poly);
//  if (timer.nsecsElapsed() > _timingThreshold)
//  {
//    LOG_DEBUG("_getTypeEvidence: " << timer.nsecsElapsed());
//  }
//  if (evidence >= _matchEvidenceThreshold)
//  {
//    return evidence;
//  }
  if (_addressMatchEnabled)
  {
    //timer.restart();
    evidence += _getAddressEvidence(poi, poly);
//    if (timer.nsecsElapsed() > _timingThreshold)
//    {
//      LOG_DEBUG("_getAddressEvidence: " << timer.nsecsElapsed());
//    }
//    if (evidence >= _matchEvidenceThreshold)
//    {
//      return evidence;
//    }
  }
  if (_phoneNumberMatchEnabled)
  {
    //timer.restart();
    evidence += _getPhoneNumberEvidence(poi, poly);
//    if (timer.nsecsElapsed() > _timingThreshold)
//    {
//      LOG_DEBUG("_getPhoneNumberEvidence: " << timer.nsecsElapsed());
//    }
//    if (evidence >= _matchEvidenceThreshold)
//    {
//      return evidence;
//    }
  }

  //We only want to run this if the previous match distance calculation was too large.
  //Tightening up the requirements for running the convex poly calculation here to improve
  //runtime.  These requirements can possibly be removed at some point in the future, if proven
  //necessary.  The school requirement definitely seems too type specific (this type of evidence
  //has actually only been found with school pois in one test dataset so far), but when
  //removing it scores dropped for other datasets...so not changing it for now.
  //timer.restart();
  if (evidence == 0 && _distance <= 35.0 && _infoCache->isType(poi, "school") &&
      _infoCache->hasCriterion(poly, QString::fromStdString(BuildingCriterion::className())))
  {
    evidence += _getConvexPolyDistanceEvidence(poi, poly);
//    if (evidence >= _matchEvidenceThreshold)
//    {
//      return evidence;
//    }
  }
//  if (timer.nsecsElapsed() > _timingThreshold)
//  {
//    LOG_DEBUG("_getConvexPolyDistanceEvidence: " << timer.nsecsElapsed());
//  }

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

set<pair<ElementId, ElementId>> PoiPolygonMatch::getMatchPairs() const
{
  set<pair<ElementId, ElementId>> result;
  // arbitrarily adding poi id first for consistency
  if (_e1IsPoi)
  {
    result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
  }
  else
  {
    result.insert(pair<ElementId, ElementId>(_eid2, _eid1));
  }
  return result;
}

map<QString, double> PoiPolygonMatch::getFeatures(const ConstOsmMapPtr& m) const
{
  return _rf->getFeatures(m, _eid1, _eid2);
}

QString PoiPolygonMatch::toString() const
{
  QString str =
    "PoiPolygonMatch: POI: %1, Poly: %2, P: %3, distance: %4, close match: %5, type score: %6, ";
  str += "name score: %7, address score: %8";
  // arbitrarily adding poi id first for consistency
  if (_e1IsPoi)
  {
    return
      str
        .arg(_eid1.toString())
        .arg(_eid2.toString())
        .arg(_class.toString())
        .arg(_distance)
        .arg(_closeDistanceMatch)
        .arg(_typeScore)
        .arg(_nameScore)
        .arg(_addressScore);
  }
  // _poi and _poly may be null during certain tests
  else
  {
    return
      str
        .arg(_eid2.toString())
        .arg(_eid1.toString())
        .arg(_class.toString())
        .arg(_distance)
        .arg(_closeDistanceMatch)
        .arg(_typeScore)
        .arg(_nameScore)
        .arg(_addressScore);
  }
}

}

