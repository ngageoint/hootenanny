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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "BuildingMatchCreator.h"

// hoot
#include <hoot/core/algorithms/extractors/OverlapExtractor.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/conflate/polygon/BuildingRfClassifier.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/visitors/SpatialIndexer.h>

// Standard
#include <fstream>
#include <functional>

// tgs
#include <tgs/RandomForest/RandomForest.h>
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>

//Qt

#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, BuildingMatchCreator)

using namespace Tgs;

/**
 * Searches the specified map for any building match potentials.
 */
class BuildingMatchVisitor : public ConstElementVisitor
{
public:

  BuildingMatchVisitor(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& result,
                       ElementCriterionPtr filter = ElementCriterionPtr()) :
  _map(map),
  _result(result),
  _filter(filter)
  {
  }

  /**
   * @param matchStatus If the element's status matches this status then it is checked for a match.
   */
  BuildingMatchVisitor(const ConstOsmMapPtr& map,
    std::vector<ConstMatchPtr>& result, std::shared_ptr<BuildingRfClassifier> rf,
    ConstMatchThresholdPtr threshold, ElementCriterionPtr filter = ElementCriterionPtr(),
    Status matchStatus = Status::Invalid) :
    _map(map),
    _result(result),
    _rf(rf),
    _mt(threshold),
    _filter(filter),
    _matchStatus(matchStatus)
  {
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
    _maxGroupSize = 0;
    _numElementsVisited = 0;
    _numMatchCandidatesVisited = 0;
    ConfigOptions opts;
    _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
    _searchRadius = opts.getSearchRadiusBuilding();
    _memoryCheckUpdateInterval = opts.getMemoryUsageCheckerInterval();
  }

  ~BuildingMatchVisitor()
  {
    LOG_TRACE("neighbor counts, max: " << _neighborCountMax << " mean: " <<
              (double)_neighborCountSum / (double)_elementsEvaluated);
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void checkForMatch(const std::shared_ptr<const Element>& e)
  {
    LOG_VART(e->getElementId());

    std::shared_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(e->getCircularError());

    // find other nearby candidates
    std::set<ElementId> neighbors =
      SpatialIndexer::findNeighbors(*env, getIndex(), _indexToEid, getMap());

    ElementId from(e->getElementType(), e->getId());

    _elementsEvaluated++;
    int neighborCount = 0;

    std::vector<MatchPtr> tempMatches;
    for (std::set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      const ElementId neighborId = *it;
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
    {
      _markNonOneToOneMatchesAsReview(tempMatches);
    }
    _adjustForOverlappingAdjoiningBuildingMatches(tempMatches);

    for (std::vector<MatchPtr>::const_iterator it = tempMatches.begin(); it != tempMatches.end();
         ++it)
    {
      _result.push_back(*it);
    }

    _neighborCountSum += neighborCount;
    _neighborCountMax = std::max(_neighborCountMax, neighborCount);
  }

  std::shared_ptr<BuildingMatch> createMatch(ElementId eid1, ElementId eid2) const
  {
    return std::make_shared<BuildingMatch>(_map, _rf, eid1, eid2, _mt);
  }

  static bool isRelated(ConstElementPtr e1, ConstElementPtr e2)
  {
    BuildingCriterion buildingCrit;
    if (e1->getStatus() != e2->getStatus() && e1->isUnknown() && e2->isUnknown() &&
        buildingCrit.isSatisfied(e1) && buildingCrit.isSatisfied(e2))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  Meters getSearchRadius(const std::shared_ptr<const Element>& e) const
  {
    Meters searchRadius;
    if (_searchRadius >= 0)
    {
      searchRadius = _searchRadius;
    }
    else
    {
      searchRadius = e->getCircularError();
    }
    LOG_VART(searchRadius);
    return searchRadius;
  }

  void visit(const ConstElementPtr& e) override
  {
    if (e->getStatus() == _matchStatus && isMatchCandidate(e))
    {
      checkForMatch(e);

      _numMatchCandidatesVisited++;
      if (_numMatchCandidatesVisited % (_taskStatusUpdateInterval * 100) == 0)
      {
        PROGRESS_DEBUG(
          "Processed " << StringUtils::formatLargeNumber(_numMatchCandidatesVisited) <<
          " match candidates / " <<
          StringUtils::formatLargeNumber(_map->getWayCount() + _map->getRelationCount()) <<
          " elements.");
      }
    }

    _numElementsVisited++;
    if (_numElementsVisited % (_taskStatusUpdateInterval * 100) == 0)
    {
      PROGRESS_STATUS(
        "Processed " << StringUtils::formatLargeNumber(_numElementsVisited) << " of " <<
        StringUtils::formatLargeNumber(_map->getWayCount() + _map->getRelationCount()) <<
        " elements.");
    }
    if (_numElementsVisited % _memoryCheckUpdateInterval == 0)
    {
      MemoryUsageChecker::getInstance().check();
    }
  }

  bool isMatchCandidate(ConstElementPtr element) const
  {
    if (_filter && !_filter->isSatisfied(element))
    {
      return false;
    }
    return BuildingCriterion().isSatisfied(element);
  }

  std::shared_ptr<HilbertRTree>& getIndex()
  {
    if (!_index)
    {
      LOG_INFO("Creating building feature index...");

      // No tuning was done, I just copied these settings from OsmMapIndex.
      // 10 children - 368 - see #3054
      _index = std::make_shared<HilbertRTree>(std::make_shared<MemoryPageStore>(728), 2);

      // Only index elements that isMatchCandidate(e)
      std::function<bool (ConstElementPtr e)> f =
        std::bind(&BuildingMatchVisitor::isMatchCandidate, this, placeholders::_1);
      std::shared_ptr<ArbitraryCriterion> pCrit = std::make_shared<ArbitraryCriterion>(f);

      // Instantiate our visitor
      SpatialIndexer v(_index,
                       _indexToEid,
                       pCrit,
                       std::bind(&BuildingMatchVisitor::getSearchRadius, this, placeholders::_1),
                       getMap());

      getMap()->visitRo(v);
      v.finalizeIndex();

      LOG_DEBUG(
        "Building feature index created with " << StringUtils::formatLargeNumber(v.getSize()) <<
        " elements.");
    }

    return _index;
  }

  ConstOsmMapPtr getMap() const { return _map; }

  long getNumMatchCandidatesFound() const { return _numMatchCandidatesVisited; }

private:

  const ConstOsmMapPtr& _map;
  std::vector<ConstMatchPtr>& _result;
  std::set<ElementId> _empty;
  std::shared_ptr<BuildingRfClassifier> _rf;
  ConstMatchThresholdPtr _mt;
  ElementCriterionPtr _filter;
  Status _matchStatus;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  Meters _searchRadius;

  // Used for finding neighbors
  std::shared_ptr<HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  long _numElementsVisited;
  long _numMatchCandidatesVisited;
  int _taskStatusUpdateInterval;
  int _memoryCheckUpdateInterval;

  void _markNonOneToOneMatchesAsReview(std::vector<MatchPtr>& matches) const
  {
    for (std::vector<MatchPtr>::iterator it = matches.begin(); it != matches.end(); ++it)
    {
      MatchPtr match = *it;
      // Not proud of this, but not sure what else to do at this point w/o having to change the
      // Match interface.
      MatchClassification& matchClass =
        const_cast<MatchClassification&>(match->getClassification());
      matchClass.setReview();
      match->setExplain("Match involved in multiple building relationships.");
    }
  }

  void _adjustForOverlappingAdjoiningBuildingMatches(std::vector<MatchPtr>& matches) const
  {
    // If we have matches or reviews between adjoining houses (building=terrace; townhouses and
    // the like), check for many to one relationships. From the many to one, keep only the match
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

    for (std::vector<MatchPtr>::const_iterator matchItr = matches.begin();
         matchItr != matches.end(); ++matchItr)
    {
      MatchPtr match = *matchItr;
      LOG_VART(match->getType());
      assert(match->getType() != MatchType::Miss);

      std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
      LOG_VART(matchPairs.size());
      assert(matchPairs.size() == 1);
      std::pair<ElementId, ElementId> matchPair = *matchPairs.begin();

      ConstElementPtr element1 = _map->getElement(matchPair.first);
      ConstElementPtr element2 = _map->getElement(matchPair.second);

      const QString adjoiningBuildingKvp = "building=terrace";
      if (element1->getElementType() == ElementType::Way &&
          element2->getElementType() == ElementType::Way &&
          (OsmSchema::getInstance().score(adjoiningBuildingKvp, element1->getTags()) >=
             tagScoreThreshold ||
           OsmSchema::getInstance().score(adjoiningBuildingKvp, element2->getTags()) >=
             tagScoreThreshold))
      {
        LOG_TRACE(
          "one or both is adjoining building: " << element1->getElementId() << ", " <<
          element2->getElementId());
        adjoiningBuildingEncountered = true;

        const double overlap = OverlapExtractor().extract(*_map, element1, element2);
        if (!highestOverlapScores.contains(element1->getElementId()) ||
            overlap > highestOverlapScores[element1->getElementId()])
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
      for (QMap<ElementId, MatchPtr>::const_iterator modifiedMatchItr = highestOverlapMatches.begin();
           modifiedMatchItr != highestOverlapMatches.end(); ++modifiedMatchItr)
      {
        modifiedMatches.push_back(modifiedMatchItr.value());
      }
      matches = modifiedMatches;
    }
    highestOverlapMatches.clear();
    LOG_VART(matches);
  }
};

BuildingMatchCreator::BuildingMatchCreator() :
_conflateMatchBuildingModel(ConfigOptions().getConflateMatchBuildingModel())
{
}

MatchPtr BuildingMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1,
                                           ElementId eid2)
{
  std::shared_ptr<BuildingMatch> result;
  if (eid1.getType() != ElementType::Node && eid2.getType() != ElementType::Node)
  {
    ConstElementPtr e1 = map->getElement(eid1);
    ConstElementPtr e2 = map->getElement(eid2);

    if (BuildingMatchVisitor::isRelated(e1, e2))
    {
      // score each candidate and push it on the result vector
      result = std::make_shared<BuildingMatch>(map, _getRf(), eid1, eid2, getMatchThreshold());
    }
  }
  return result;
}

void BuildingMatchCreator::createMatches(const ConstOsmMapPtr& map,
                                         std::vector<ConstMatchPtr>& matches,
                                         ConstMatchThresholdPtr threshold)
{
  QElapsedTimer timer;
  timer.start();

  // The parent does some initialization we need.
  MatchCreator::createMatches(map, matches, threshold);

  QString searchRadiusStr;
  const double searchRadius = ConfigOptions().getSearchRadiusBuilding();
  if (searchRadius < 0)
  {
    searchRadiusStr = "within a feature dependent search radius";
  }
  else
  {
    searchRadiusStr =
      "within a search radius of " + QString::number(searchRadius, 'g', 2) + " meters";
  }
  LOG_INFO("Looking for matches with: " << className() << " " << searchRadiusStr << "...");
  LOG_VARD(*threshold);
  const int matchesSizeBefore = matches.size();

  BuildingMatchVisitor v(map, matches, _getRf(), threshold, _filter, Status::Unknown1);
  map->visitWaysRo(v);
  map->visitRelationsRo(v);
  const int matchesSizeAfter = matches.size();

  LOG_INFO(
    "Found " << StringUtils::formatLargeNumber(v.getNumMatchCandidatesFound()) <<
    " building match candidates and " <<
    StringUtils::formatLargeNumber(matchesSizeAfter - matchesSizeBefore) <<
    " total matches in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

std::vector<CreatorDescription> BuildingMatchCreator::getAllCreators() const
{
  std::vector<CreatorDescription> result;
  result.emplace_back(
    className(),
    "Generates matchers that match buildings",
    CreatorDescription::Building,
    false);
  return result;
}

std::shared_ptr<BuildingRfClassifier> BuildingMatchCreator::_getRf()
{
  if (!_rf)
  {
    QString path = ConfPath::search(_conflateMatchBuildingModel);
    LOG_DEBUG("Loading model from: " << path);

    QFile file(path.toLatin1().data());
    if (!file.open(QIODevice::ReadOnly))
    {
      throw HootException("Error opening file: " + path);
    }
    QDomDocument doc("");
    if (!doc.setContent(&file))
    {
      file.close();
      throw HootException("Error opening file: " + path);
    }
    file.close();

    _rf = std::make_shared<BuildingRfClassifier>();
    QDomElement docRoot = doc.elementsByTagName("RandomForest").at(0).toElement();
    _rf->import(docRoot);
  }

  return _rf;
}

bool BuildingMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map)
{
  std::vector<ConstMatchPtr> matches;
  return BuildingMatchVisitor(map, matches, _filter).isMatchCandidate(element);
}

std::shared_ptr<MatchThreshold> BuildingMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    LOG_VART(ConfigOptions().getBuildingMatchThreshold());
    _matchThreshold =
      std::make_shared<MatchThreshold>(
        ConfigOptions().getBuildingMatchThreshold(), ConfigOptions().getBuildingMissThreshold(),
        ConfigOptions().getBuildingReviewThreshold());
  }
  return _matchThreshold;
}

QStringList BuildingMatchCreator::getCriteria() const
{
  return QStringList(BuildingCriterion::className());
}

}
