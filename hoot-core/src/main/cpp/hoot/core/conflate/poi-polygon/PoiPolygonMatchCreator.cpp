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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonMatchCreator.h"

// hoot
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/poi-polygon/PoiPolygonMatchVisitor.h>

// Std
#include <float.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, PoiPolygonMatchCreator)

MatchPtr PoiPolygonMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1,
                                             ElementId eid2)
{
  if (!_infoCache)
  {
    LOG_DEBUG("Initializing info cache...");
    _infoCache.reset(new PoiPolygonInfoCache(map));
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
  QElapsedTimer timer;
  timer.start();

  // The parent does some initialization we need.
  MatchCreator::createMatches(map, matches, threshold);

  QString searchRadiusStr;
  const double additionalDistance = ConfigOptions().getPoiPolygonAdditionalSearchDistance();
  if (additionalDistance <= 0)
  {
    searchRadiusStr = "within a feature dependent search radius";
  }
  else
  {
    searchRadiusStr =
      "within a feature dependent search radius plus an additional distance of ";
    if (additionalDistance < 1000)
    {
      searchRadiusStr += QString::number(additionalDistance, 'g', 3) + " meters";
    }
    else
    {
      searchRadiusStr += QString::number(additionalDistance / 1000.0, 'g', 3) + " kilometers";
    }
  }
  LOG_INFO("Looking for matches with: " << className() << " " << searchRadiusStr << "...");
  LOG_VARD(*threshold);
  const int matchesSizeBefore = matches.size();

  if (!_infoCache)
  {
    LOG_DEBUG("Initializing info cache...");
    _infoCache.reset(new PoiPolygonInfoCache(map));
    _infoCache->setConfiguration(conf());
  }

  PoiPolygonMatch::resetMatchDistanceInfo();

  PoiPolygonMatchVisitor matchVis(map, matches, threshold, _getRf(), _infoCache, _filter);
  map->visitNodesRo(matchVis);
  const int matchesSizeAfter = matches.size();

  LOG_INFO(
    "Found " << StringUtils::formatLargeNumber(matchVis.getNumMatchCandidatesFound()) <<
    " POI to Polygon match candidate features and " <<
    StringUtils::formatLargeNumber(matchesSizeAfter - matchesSizeBefore) << " total matches in: " <<
    StringUtils::millisecondsToDhms(timer.elapsed()) << ".");

  // If we're only keeping matches/reviews with the closest distances between features, then let's
  // weed out the ones that aren't as close to each other.
  int numMatchesRemoved = 0;
  if (ConfigOptions().getPoiPolygonKeepClosestMatchesOnly())
  {
    timer.restart();
    const int startingMatchCount = matches.size();
    numMatchesRemoved = _retainClosestDistanceMatchesOnly(matches, map);
    LOG_INFO(
      "Discarded " << StringUtils::formatLargeNumber(numMatchesRemoved) <<
      " non-closest  " << StringUtils::formatLargeNumber(startingMatchCount) <<
      " / total matches in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
  }

  if (conf().getBool(ConfigOptions::getPoiPolygonCalculateMatchDistanceTruthKey()))
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
    StringUtils::formatLargeNumber(PoiPolygonMatch::addressMatches) << " of " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::addressMatchCandidates) <<
    " candidate matches.  " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::addressesProcessed) <<
    " total addresses processed.");
  LOG_DEBUG(
    "POI/Polygon phone number matches: " <<
    StringUtils::formatLargeNumber(PoiPolygonMatch::phoneNumberMatches) << " of " <<
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
  if (ConfigOptions().getPoiPolygonKeepClosestMatchesOnly())
  {
    LOG_DEBUG(
      "Number of matches removed due to option enabled to only keeping closest matches: " <<
      StringUtils::formatLargeNumber(numMatchesRemoved));
  }
  _infoCache->printCacheInfo();
  _infoCache->clear();
}

int PoiPolygonMatchCreator::_retainClosestDistanceMatchesOnly(
  std::vector<ConstMatchPtr>& matches, const ConstOsmMapPtr& map)
{
  LOG_INFO("Discarding non-closest matches...");

  const bool debug = false; // leave this false by default
  const ElementId testElementId1 = ElementId(ElementType::Node, -1893344);
  const ElementId testElementId2 = ElementId(ElementType::Way, -276936);

  int numRemoved = 0;
  // look for overlapping matches separately for POI and poly matches
  numRemoved += _retainClosestDistanceMatchesOnlyByType(matches, map, true);
  LOG_DEBUG("Removed " << StringUtils::formatLargeNumber(numRemoved) << " POI matches.");

  if (debug)
  {
    const bool containsMatch = _containsMatch(testElementId1, testElementId2, matches);
    QString containstStr = containsMatch ? "contains" : "does not contain";
    LOG_DEBUG(
      "Matches after POI match removal " << containstStr << " elements: " << testElementId1 <<
      ", " << testElementId2);
    int numMatchesContaining = _numMatchesContainingElement(testElementId1, matches);
    LOG_DEBUG(numMatchesContaining << " contain " << testElementId1);
    numMatchesContaining = _numMatchesContainingElement(testElementId2, matches);
    LOG_DEBUG(numMatchesContaining << " contain " << testElementId2);
  }

  numRemoved += _retainClosestDistanceMatchesOnlyByType(matches, map, false);
  LOG_DEBUG("Removed " << StringUtils::formatLargeNumber(numRemoved) << " poly matches.");

  if (debug)
  {
    const bool containsMatch = _containsMatch(testElementId1, testElementId2, matches);
    QString containstStr = containsMatch ? "contains" : "does not contain";
    LOG_DEBUG(
      "Matches after poly match removal " << containstStr << " elements: " << testElementId1 <<
      ", " << testElementId2);
    int numMatchesContaining = _numMatchesContainingElement(testElementId1, matches);
    LOG_DEBUG(numMatchesContaining << " matches contain " << testElementId1);
    numMatchesContaining = _numMatchesContainingElement(testElementId2, matches);
    LOG_DEBUG(numMatchesContaining << " matches contain " << testElementId2);
  }

  return numRemoved;
}

bool PoiPolygonMatchCreator::_containsMatch(
  const ElementId& elementId1, const ElementId& elementId2,
  const std::vector<ConstMatchPtr>& matches) const
{
  // for debugging only
  for (std::vector<ConstMatchPtr>::const_iterator matchItr = matches.begin();
       matchItr != matches.end(); ++matchItr)
  {
    ConstMatchPtr match = *matchItr;
    if (match->getType() != MatchType::Miss)
    {
      if (match->getMatchPairs().size() != 1)
      {
        LOG_VART(match->getMatchPairs().size());
      }
      assert(match->getMatchPairs().size() == 1);
      std::pair<ElementId, ElementId> matchElementIds = *(match->getMatchPairs()).begin();
      if ((matchElementIds.first == elementId1 && matchElementIds.second == elementId2) ||
          (matchElementIds.first == elementId2 && matchElementIds.second == elementId1))
      {
        return true;
      }
    }
  }
  return false;
}

int PoiPolygonMatchCreator::_numMatchesContainingElement(
  const ElementId& elementId, const std::vector<ConstMatchPtr>& matches) const
{
  // for debugging only
  int numMatchesContainingId = 0;
  for (std::vector<ConstMatchPtr>::const_iterator matchItr = matches.begin();
       matchItr != matches.end(); ++matchItr)
  {
    ConstMatchPtr match = *matchItr;
    if (match->getType() != MatchType::Miss)
    {
      if (match->getMatchPairs().size() != 1)
      {
        LOG_VART(match->getMatchPairs().size());
      }
      assert(match->getMatchPairs().size() == 1);
      std::pair<ElementId, ElementId> matchElementIds = *(match->getMatchPairs()).begin();
      if (matchElementIds.first == elementId || matchElementIds.second == elementId)
      {
        numMatchesContainingId++;
      }    }
  }
  return numMatchesContainingId;
}

QMultiMap<ElementId, ConstMatchPtr> PoiPolygonMatchCreator::_indexMatchesById(
  const std::vector<ConstMatchPtr>& matches, const QString& matchTypeStr)
{
  LOG_DEBUG(
    "Indexing " << StringUtils::formatLargeNumber(matches.size()) << " " << matchTypeStr <<
    " matches...");
  const bool processPois = matchTypeStr == "POI";
  QMultiMap<ElementId, ConstMatchPtr> matchesById;

  for (std::vector<ConstMatchPtr>::const_iterator matchItr = matches.begin();
       matchItr != matches.end(); ++matchItr)
  {
    ConstMatchPtr match = *matchItr;
    //LOG_VART(match);
    if (match->getType() != MatchType::Miss)
    {
      if (match->getMatchPairs().size() != 1)
      {
        LOG_VART(match->getMatchPairs().size());
      }
      assert(match->getMatchPairs().size() == 1);
      std::pair<ElementId, ElementId> matchElementIds = *(match->getMatchPairs()).begin();
      if (processPois)
      {
        if (matchElementIds.first.getType() == ElementType::Node)
        {
          matchesById.insertMulti(matchElementIds.first, match);
        }
        else
        {
          matchesById.insertMulti(matchElementIds.second, match);
        }
      }
      else
      {
        if (matchElementIds.first.getType() == ElementType::Way ||
            matchElementIds.first.getType() == ElementType::Relation)
        {
          matchesById.insertMulti(matchElementIds.first, match);
        }
        else
        {
          matchesById.insertMulti(matchElementIds.second, match);
        }
      }
    }
  }

  return matchesById;
}

QMap<ElementId, QList<ConstMatchPtr>> PoiPolygonMatchCreator::_getOverlappingMatches(
  const QMultiMap<ElementId, ConstMatchPtr>& matchesById, const QString& matchTypeStr)
{
  LOG_DEBUG(
    "Finding overlapping " << matchTypeStr << " matches out of " <<
    StringUtils::formatLargeNumber(matchesById.size()) << " indexed matches...");
  QMap<ElementId, QList<ConstMatchPtr>> overlappingMatches;

  const QList<ElementId> ids = matchesById.keys();
  for (QList<ElementId>::const_iterator idItr = ids.begin(); idItr != ids.end(); ++idItr)
  {
    //LOG_VART(*idItr);
    const QList<ConstMatchPtr> matches = matchesById.values(*idItr);
    //LOG_VART(matches.size());
    if (matches.size() > 1)
    {
      //LOG_TRACE("Found overlapping matches: " << matches);
      LOG_TRACE("Found overlapping matches of size: " << matches.size());
      overlappingMatches[*idItr] = matches;
    }
  }

  return overlappingMatches;
}

std::vector<ConstMatchPtr> PoiPolygonMatchCreator::_filterOutNonClosestMatches(
  const QMap<ElementId, QList<ConstMatchPtr>>& overlappingMatches,
  const std::vector<ConstMatchPtr>& allMatches, const ConstOsmMapPtr& map,
  const QString& matchTypeStr)
{
  LOG_DEBUG(
    "Filtering out non-closest " << matchTypeStr << " matches out of " <<
     StringUtils::formatLargeNumber(overlappingMatches.size()) << " overlapping matches...");
  QList<ConstMatchPtr> matchesToRemove;
  for (QMap<ElementId, QList<ConstMatchPtr>>::const_iterator matchesItr = overlappingMatches.begin();
       matchesItr != overlappingMatches.end(); ++matchesItr)
  {
    LOG_TRACE("****************************************");
    ElementId sharedElementId = matchesItr.key();
    LOG_VART(sharedElementId);
    ConstElementPtr sharedElement = map->getElement(sharedElementId);

    ConstMatchPtr closestMatch;
    double smallestDistance = DBL_MAX;
    QList<ConstMatchPtr> matchesWithSharedId = matchesItr.value();
    for (QList<ConstMatchPtr>::const_iterator matchesItr2 = matchesWithSharedId.begin();
         matchesItr2 != matchesWithSharedId.end(); ++matchesItr2)
    {
      ConstMatchPtr overlappingMatch = *matchesItr2;
      LOG_VART(overlappingMatch->getName());
      LOG_VART(overlappingMatch);

      if (overlappingMatch->getName() == PoiPolygonMatch::getPoiPolygonMatchName())
      {
        std::pair<ElementId, ElementId> matchElementIds =
          *(overlappingMatch->getMatchPairs()).begin();
        ElementId comparisonElementId;
        if (matchElementIds.first == sharedElementId)
        {
          comparisonElementId = matchElementIds.second;
        }
        else
        {
          comparisonElementId = matchElementIds.first;
        }
        LOG_VART(comparisonElementId);

        int pointCount = 0;
        if (sharedElementId.getType() == ElementType::Node)
        {
          pointCount++;
        }
        if (comparisonElementId.getType() == ElementType::Node)
        {
          pointCount++;
        }
        if (pointCount != 1)
        {
          throw IllegalArgumentException(
            "POI/Polygon match does not contain exactly one POI: " + overlappingMatch->toString());
        }

        int polyCount = 0;
        if (sharedElementId.getType() == ElementType::Way ||
            sharedElementId.getType() == ElementType::Relation)
        {
          polyCount++;
        }
        if (comparisonElementId.getType() == ElementType::Way ||
            comparisonElementId.getType() == ElementType::Relation)
        {
          polyCount++;
        }
        if (polyCount != 1)
        {
          throw IllegalArgumentException(
            "POI/Polygon match does not contain exactly one Polygon: " +
            overlappingMatch->toString());
        }

        ConstElementPtr comparisonElement = map->getElement(comparisonElementId);

        ConstNodePtr point;
        bool sharedWasPoint = false;
        if (sharedElementId.getType() == ElementType::Node)
        {
          point = std::dynamic_pointer_cast<const Node>(sharedElement);
          sharedWasPoint = true;
        }
        else
        {
          point = std::dynamic_pointer_cast<const Node>(comparisonElement);
        }

        double distance = DBL_MAX;
        if (sharedWasPoint)
        {
          distance = _infoCache->getDistance(comparisonElement, point);
        }
        else
        {
          distance = _infoCache->getDistance(sharedElement, point);
        }
        LOG_VART(distance);
        if (distance != -1.0 && distance < smallestDistance)
        {
          smallestDistance = distance;
          LOG_TRACE(
            "smallest distance: " << smallestDistance << ", " << sharedElementId << ";" <<
            comparisonElementId);
          closestMatch = overlappingMatch;
        }
      }
    }

    if (closestMatch)
    {
      LOG_TRACE("Keeping closest match: " << closestMatch << "...");
      for (QList<ConstMatchPtr>::const_iterator matchItr = matchesWithSharedId.begin();
           matchItr != matchesWithSharedId.end(); ++matchItr)
      {
        ConstMatchPtr match = *matchItr;
        if (match != closestMatch)
        {
          matchesToRemove.append(match);
        }
      }
    }

    LOG_VART(matchesToRemove.size());
  }
  LOG_DEBUG("Removing " <<  StringUtils::formatLargeNumber(matchesToRemove.size()) << " matches.");

  std::vector<ConstMatchPtr> matchesToKeep;
  for (std::vector<ConstMatchPtr>::const_iterator matchItr = allMatches.begin();
       matchItr != allMatches.end(); ++matchItr)
  {
    ConstMatchPtr match = *matchItr;
    if (!matchesToRemove.contains(match))
    {
      matchesToKeep.push_back(match);
    }
  }
  LOG_VART(matchesToKeep.size());
  return matchesToKeep;
}

int PoiPolygonMatchCreator::_retainClosestDistanceMatchesOnlyByType(
  std::vector<ConstMatchPtr>& matches, const ConstOsmMapPtr& map, const bool processPois)
{
  QString matchTypeStr = "Polygon";
  if (processPois)
  {
    matchTypeStr = "POI";
  }
  const int startingMatchesSize = matches.size();
  LOG_DEBUG(
    "Discarding non-closest " << matchTypeStr << " matches (out of " <<
    StringUtils::formatLargeNumber(matches.size()) << " remaining)...");

  // index matches by involved element ID
  const QMultiMap<ElementId, ConstMatchPtr> matchesById = _indexMatchesById(matches, matchTypeStr);

  // find matches sharing the same element ID
  const QMap<ElementId, QList<ConstMatchPtr>> overlappingMatches =
    _getOverlappingMatches(matchesById, matchTypeStr);

  // for each overlapping match, find the match in the group with the closest distance between
  // features and throw out the rest
  matches = _filterOutNonClosestMatches(overlappingMatches, matches, map, matchTypeStr);

  const int diff = startingMatchesSize - matches.size();
  LOG_VARD(diff);
  return diff;
}

std::vector<CreatorDescription> PoiPolygonMatchCreator::getAllCreators() const
{
  std::vector<CreatorDescription> result;
  result.emplace_back(
    className(),
    "Generates matchers that match POIs to polygons",
    //this match creator has two conflatable types, so arbitrarily just picking one of them as
    //the base feature type; stats class will handle the logic to deal with both poi and polygon
    //input types
    CreatorDescription::Polygon,
    false);
  return result;
}

bool PoiPolygonMatchCreator::isMatchCandidate(
  ConstElementPtr element, const ConstOsmMapPtr& /*map*/)
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

QStringList PoiPolygonMatchCreator::getCriteria() const
{
  QStringList criteria;
  criteria.append(PoiPolygonPoiCriterion::className());
  criteria.append(PoiPolygonPolyCriterion::className());
  return criteria;
}

}
