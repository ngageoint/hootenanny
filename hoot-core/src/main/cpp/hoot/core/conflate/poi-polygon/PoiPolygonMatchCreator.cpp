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
#include "PoiPolygonMatchCreator.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/visitors/poi-polygon/PoiPolygonMatchVisitor.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, PoiPolygonMatchCreator)

PoiPolygonMatchCreator::PoiPolygonMatchCreator()
{
}

MatchPtr PoiPolygonMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1,
                                           ElementId eid2)
{
  if (!_infoCache)
  {
    LOG_TRACE("Initializing info cache...");
    _infoCache.reset(new PoiPolygonCache(map));
  }

  std::shared_ptr<PoiPolygonMatch> result;

  if (eid1.getType() != eid2.getType())
  {
    ConstElementPtr e1 = map->getElement(eid1);
    ConstElementPtr e2 = map->getElement(eid2);

    const bool foundPoi = _poiCrit.isSatisfied(e1) || _poiCrit.isSatisfied(e2);
    const bool foundPoly = _polyCrit.isSatisfied(e1) || _polyCrit.isSatisfied(e2);
    if (foundPoi && foundPoly)
    {
      result.reset(new PoiPolygonMatch(map, getMatchThreshold(), _getRf(), _infoCache));
      result->setConfiguration(conf());
      result->calculateMatch(eid1, eid2);
    }
  }

  return result;
}

void PoiPolygonMatchCreator::createMatches(const ConstOsmMapPtr& map,
                                           std::vector<ConstMatchPtr>& matches,
                                           ConstMatchThresholdPtr threshold)
{
  LOG_INFO("Looking for matches with: " << className() << "...");
  LOG_VARD(*threshold);

  if (!_infoCache)
  {
    LOG_TRACE("Initializing info cache...");
    _infoCache.reset(new PoiPolygonCache(map));
  }

  PoiPolygonMatch::resetMatchDistanceInfo();

  QElapsedTimer timer;
  timer.start();
  PoiPolygonMatchVisitor v(map, matches, threshold, _getRf(), _infoCache, _filter);
  map->visitRo(v);
  LOG_INFO(
    "Found " << StringUtils::formatLargeNumber(v.getNumMatchCandidatesFound()) <<
    " POI to Polygon match candidates in: " << StringUtils::millisecondsToDhms(timer.elapsed()) <<
    ".");

  if (conf().getBool(ConfigOptions::getPoiPolygonPrintMatchDistanceTruthKey()))
  {
    PoiPolygonMatch::printMatchDistanceInfo();
  }
  LOG_DEBUG(
    "POI/Polygon total match pair candidates processed: " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::matchesProcessed));
  LOG_DEBUG(
    "POI/Polygon distance matches: " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::distanceMatches));
  LOG_DEBUG(
    "POI/Polygon type matches: " << StringUtils::formatLargeNumber(PoiPolygonMatch::typeMatches));
  LOG_DEBUG(
    "POI/Polygon match pairs with no relevant type: " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::noTypeFoundCount));
  LOG_DEBUG(
    "POI/Polygon name matches: " << StringUtils::formatLargeNumber(PoiPolygonMatch::nameMatches) <<
    " / " << StringUtils::formatLargeNumber(PoiPolygonMatch::nameMatchCandidates) <<
    " match candidates.  " << StringUtils::formatLargeNumber(PoiPolygonMatch::namesProcessed) <<
    " total names processed.");
  LOG_DEBUG(
    "POI/Polygon address matches: " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::addressMatches) << " / " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::addressMatchCandidates) <<
    " candidate matches.  " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::addressesProcessed) <<
    " total addresses processed.");
  LOG_DEBUG(
    "POI/Polygon phone number matches: " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::phoneNumberMatches) << " / " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::phoneNumberMatchCandidates) <<
    " candidate matches.  " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::phoneNumbersProcesed) <<
    " total phone numbers processed.");
  LOG_DEBUG(
    "POI/Polygon convex polygon distance matches: " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::convexPolyDistanceMatches));
  LOG_DEBUG(
    "POI/Polygon review reductions: " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::numReviewReductions));
  _infoCache->printCacheInfo();
}

std::vector<CreatorDescription> PoiPolygonMatchCreator::getAllCreators() const
{
  std::vector<CreatorDescription> result;
  result.push_back(
    CreatorDescription(
      className(),
      "Generates matchers that match POIs to polygons",
      //this match creator has two conflatable types, so arbitrarily just picking one of them as
      //the base feature type; stats class will handle the logic to deal with both poi and polygon
      //input types
      CreatorDescription::Polygon,
      false));
  return result;
}

bool PoiPolygonMatchCreator::isMatchCandidate(ConstElementPtr element,
                                              const ConstOsmMapPtr& /*map*/)
{
  return
    element->isUnknown() && (_poiCrit.isSatisfied(element) || _polyCrit.isSatisfied(element));
}

std::shared_ptr<MatchThreshold> PoiPolygonMatchCreator::getMatchThreshold()
{
  //  Since the POI to Poly matcher is an additive model, all thresholds are 1.0
  if (!_matchThreshold.get())
    _matchThreshold.reset(new MatchThreshold(1.0, 1.0, 1.0));
  return _matchThreshold;
}

std::shared_ptr<PoiPolygonRfClassifier> PoiPolygonMatchCreator::_getRf()
{
  if (!_rf)
  {
    _rf.reset(new PoiPolygonRfClassifier());
  }
  return _rf;
}

}
