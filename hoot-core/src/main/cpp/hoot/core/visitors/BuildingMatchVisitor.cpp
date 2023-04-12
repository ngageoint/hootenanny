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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

#include "BuildingMatchVisitor.h"

// hoot
#include <hoot/core/algorithms/extractors/OverlapExtractor.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/visitors/SpatialIndexer.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

//  Don't register this visitor with the factory
//HOOT_FACTORY_REGISTER(ElementVisitor, BuildingMatchVisitor)

BuildingMatchVisitor::BuildingMatchVisitor(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result,
                                           ElementCriterionPtr filter)
  : _map(map),
    _result(result),
    _rf(),
    _filter(filter),
    _matchStatus(),
    _neighborCountMax(-1),
    _neighborCountSum(0),
    _elementsEvaluated(0),
    _maxGroupSize(0),
    _searchRadius(ConfigOptions().getSearchRadiusBuilding()),
    _index(),
    _indexToEid(),
    _numElementsVisited(0),
    _numMatchCandidatesVisited(0),
    _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
    _memoryCheckUpdateInterval(ConfigOptions().getMemoryUsageCheckerInterval())
{
}

BuildingMatchVisitor::BuildingMatchVisitor(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result,
                                           std::shared_ptr<BuildingRfClassifier> rf, ConstMatchThresholdPtr threshold,
                                           ElementCriterionPtr filter, Status matchStatus)
  : _map(map),
    _result(result),
    _rf(rf),
    _mt(threshold),
    _filter(filter),
    _matchStatus(matchStatus),
    _neighborCountMax(-1),
    _neighborCountSum(0),
    _elementsEvaluated(0),
    _maxGroupSize(0),
    _searchRadius(ConfigOptions().getSearchRadiusBuilding()),
    _index(),
    _indexToEid(),
    _numElementsVisited(0),
    _numMatchCandidatesVisited(0),
    _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
    _memoryCheckUpdateInterval(ConfigOptions().getMemoryUsageCheckerInterval())
{
}

BuildingMatchVisitor::~BuildingMatchVisitor()
{
  LOG_TRACE("neighbor counts, max: " << _neighborCountMax << " mean: " << (double)_neighborCountSum / (double)_elementsEvaluated);
}

void BuildingMatchVisitor::checkForMatch(const std::shared_ptr<const Element>& e)
{
  LOG_VART(e->getElementId());

  std::shared_ptr<Envelope> env(e->getEnvelope(_map));
  env->expandBy(e->getCircularError());

  // find other nearby candidates
  std::set<ElementId> neighbors = SpatialIndexer::findNeighbors(*env, getIndex(), _indexToEid, getMap());

  ElementId from(e->getElementType(), e->getId());

  _elementsEvaluated++;
  int neighborCount = 0;

  std::vector<MatchPtr> tempMatches;
  for (const auto& neighborId : neighbors)
  {
    if (from != neighborId)
    {
      const std::shared_ptr<const Element>& neighbor = _map->getElement(neighborId);
      if (isRelated(neighbor, e))
      {
        // score each candidate and push it on the result vector
        std::shared_ptr<BuildingMatch> match = createMatch(from, neighborId);
        // if we're confident this is a miss
        if (match->getType() != MatchType::Miss)
        {
          tempMatches.push_back(match);
          neighborCount++;
        }
      }
    }
  }
  LOG_VART(neighborCount);

  if (neighborCount > 1 && ConfigOptions().getBuildingReviewMatchesOtherThanOneToOne())
    _markNonOneToOneMatchesAsReview(tempMatches);

  _adjustForOverlappingAdjoiningBuildingMatches(tempMatches);

  for (const auto& m : tempMatches)
    _result.push_back(m);

  _neighborCountSum += neighborCount;
  _neighborCountMax = std::max(_neighborCountMax, neighborCount);
}

std::shared_ptr<BuildingMatch> BuildingMatchVisitor::createMatch(ElementId eid1, ElementId eid2) const
{
  return std::make_shared<BuildingMatch>(_map, _rf, eid1, eid2, _mt);
}

bool BuildingMatchVisitor::isRelated(ConstElementPtr e1, ConstElementPtr e2)
{
  BuildingCriterion buildingCrit;
  return (e1->getStatus() != e2->getStatus() && e1->isUnknown() && e2->isUnknown() && buildingCrit.isSatisfied(e1) && buildingCrit.isSatisfied(e2));
}

Meters BuildingMatchVisitor::getSearchRadius(const std::shared_ptr<const Element>& e) const
{
  Meters searchRadius;
  if (_searchRadius >= 0)
    searchRadius = _searchRadius;
  else
    searchRadius = e->getCircularError();

  LOG_VART(searchRadius);
  return searchRadius;
}

void BuildingMatchVisitor::visit(const ConstElementPtr& e)
{
  if (e->getStatus() == _matchStatus && isMatchCandidate(e))
  {
    checkForMatch(e);

    _numMatchCandidatesVisited++;
    if (_numMatchCandidatesVisited % (_taskStatusUpdateInterval * 100) == 0)
    {
      PROGRESS_DEBUG(
        "\tProcessed " << StringUtils::formatLargeNumber(_numMatchCandidatesVisited) <<
        " match candidates / " <<
        StringUtils::formatLargeNumber(_map->getWayCount() + _map->getRelationCount()) <<
        " elements.");
    }
  }

  _numElementsVisited++;
  if (_numElementsVisited % (_taskStatusUpdateInterval * 100) == 0)
  {
    PROGRESS_STATUS(
      "\tProcessed " << StringUtils::formatLargeNumber(_numElementsVisited) << " of " <<
      StringUtils::formatLargeNumber(_map->getWayCount() + _map->getRelationCount()) <<
      " elements.");
  }
  if (_numElementsVisited % _memoryCheckUpdateInterval == 0)
    MemoryUsageChecker::getInstance().check();
}

bool BuildingMatchVisitor::isMatchCandidate(ConstElementPtr element) const
{
  if (_filter && !_filter->isSatisfied(element))
    return false;

  return BuildingCriterion().isSatisfied(element);
}

std::shared_ptr<HilbertRTree>& BuildingMatchVisitor::getIndex()
{
  if (!_index)
  {
    LOG_INFO("Creating building feature index...");

    // No tuning was done, just copied these settings from OsmMapIndex. 10 children - 368 - see
    // #3054
    _index = std::make_shared<HilbertRTree>(std::make_shared<MemoryPageStore>(728), 2);

    // Only index elements that isMatchCandidate(e)
    std::function<bool (ConstElementPtr e)> f = std::bind(&BuildingMatchVisitor::isMatchCandidate, this, placeholders::_1);
    std::shared_ptr<ArbitraryCriterion> pCrit = std::make_shared<ArbitraryCriterion>(f);

    // Instantiate our visitor
    SpatialIndexer v(_index, _indexToEid, pCrit, std::bind(&BuildingMatchVisitor::getSearchRadius, this, placeholders::_1), getMap());

    getMap()->visitRo(v);
    v.finalizeIndex();

    LOG_DEBUG("Building feature index created with " << StringUtils::formatLargeNumber(v.getSize()) << " elements.");
  }

  return _index;
}

void BuildingMatchVisitor::_markNonOneToOneMatchesAsReview(const std::vector<MatchPtr>& matches) const
{
  for (const auto& match : matches)
  {
    // Not proud of this, but not sure what else to do at this point w/o having to change the
    // Match interface.
    MatchClassification& matchClass =
      const_cast<MatchClassification&>(match->getClassification());
    matchClass.setReview();
    match->setExplain("Match involved in multiple building relationships.");
  }
}

void BuildingMatchVisitor::_adjustForOverlappingAdjoiningBuildingMatches(std::vector<MatchPtr>& matches) const
{
  // If we have matches or reviews between adjoining houses (building=terrace; townhouses and
  // the like), check for many-to-one relationships. From the many-to-one, keep only the match
  // with the highest overlap. Convert all others to misses by removing the matches completely.
  //
  // The argument could be made that this overlap check could be done for all buildings, not just
  // adjoining buildings, to reduce bad matches/reviews. Not sure how much havoc that might wreak,
  // but maybe worth trying.

  LOG_VART(matches);

  QMap<ElementId, double> highestOverlapScores;
  QMap<ElementId, MatchPtr> highestOverlapMatches;
  const double tagScoreThreshold = ConfigOptions().getBuildingAdjoiningTagScoreThreshold();
  bool adjoiningBuildingEncountered = false;

  for (const auto& match : matches)
  {
    LOG_VART(match->getType());
    assert(match->getType() != MatchType::Miss);

    std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
    LOG_VART(matchPairs.size());
    assert(matchPairs.size() == 1);
    std::pair<ElementId, ElementId> matchPair = *matchPairs.begin();

    ConstElementPtr element1 = _map->getElement(matchPair.first);
    ConstElementPtr element2 = _map->getElement(matchPair.second);

    const QString adjoiningBuildingKvp = "building=terrace";
    if (element1->getElementType() == ElementType::Way && element2->getElementType() == ElementType::Way &&
        (OsmSchema::getInstance().score(adjoiningBuildingKvp, element1->getTags()) >= tagScoreThreshold ||
         OsmSchema::getInstance().score(adjoiningBuildingKvp, element2->getTags()) >= tagScoreThreshold))
    {
      LOG_TRACE(
        "one or both is adjoining building: " << element1->getElementId() << ", " <<
        element2->getElementId());
      adjoiningBuildingEncountered = true;

      const double overlap = OverlapExtractor().extract(*_map, element1, element2);
      if (!highestOverlapScores.contains(element1->getElementId()) || overlap > highestOverlapScores[element1->getElementId()])
      {
        highestOverlapScores[element1->getElementId()] = overlap;
        highestOverlapMatches[element1->getElementId()] = match;
        LOG_TRACE(
          "Updating highest overlap score: " << overlap << " for ref: " <<
          element1->getElementId() << ", sec: " << element2->getElementId());
      }
      else
      {
        LOG_TRACE(
          "Dropping match with lower overlap score: " << overlap <<
          " compared to highest overlap score: " <<
          highestOverlapScores[element1->getElementId()] << " for ref: " <<
          element1->getElementId() << ", sec: " << element2->getElementId());
      }
    }
  }
  highestOverlapScores.clear();
  LOG_VART(adjoiningBuildingEncountered);

  if (adjoiningBuildingEncountered)
  {
    std::vector<MatchPtr> modifiedMatches;
    for (auto it = highestOverlapMatches.constBegin(); it != highestOverlapMatches.constEnd(); ++it)
      modifiedMatches.push_back(it.value());
    matches = std::move(modifiedMatches);
  }
  highestOverlapMatches.clear();
  LOG_VART(matches);
}

}
