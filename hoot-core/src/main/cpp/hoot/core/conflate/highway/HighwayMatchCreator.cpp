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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "HighwayMatchCreator.h"

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcherFactory.h>
#include <hoot/core/conflate/highway/HighwayClassifier.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/schema/TagAncestorDifferencer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MemoryUsageChecker.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/visitors/SpatialIndexer.h>

// Standard
#include <fstream>

// Boost
#include <boost/bind.hpp>

// tgs
#include <tgs/RandomForest/RandomForest.h>
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>

// Qt
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, HighwayMatchCreator)

/**
 * Searches the specified map for any highway match potentials.
 */
class HighwayMatchVisitor : public ConstElementVisitor
{
public:

  HighwayMatchVisitor(const ConstOsmMapPtr& map, vector<ConstMatchPtr>& result,
                      ElementCriterionPtr filter = ElementCriterionPtr())
  : _map(map),
    _result(result),
    _filter(filter)
  {
  }

  HighwayMatchVisitor(const ConstOsmMapPtr& map, vector<ConstMatchPtr>& result,
                      std::shared_ptr<HighwayClassifier> classifier,
                      std::shared_ptr<HighwayClassifier> medianClassifier,
                      std::shared_ptr<SublineStringMatcher> sublineMatcher, ConstMatchThresholdPtr threshold,
                      std::shared_ptr<TagAncestorDifferencer> tagAncestorDiff,
                      ElementCriterionPtr filter = ElementCriterionPtr())
  : _map(map),
    _result(result),
    _classifier(classifier),
    _medianClassifier(medianClassifier),
    _sublineMatcher(sublineMatcher),
    _threshold(threshold),
    _neighborCountMax(-1),
    _neighborCountSum(0),
    _elementsEvaluated(0),
    _searchRadius(ConfigOptions().getSearchRadiusHighway()),
    _tagAncestorDiff(tagAncestorDiff),
    _filter(filter),
    _numElementsVisited(0),
    _numMatchCandidatesVisited(0),
    _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval()),
    _memoryCheckUpdateInterval(ConfigOptions().getMemoryUsageCheckerInterval())
  {
  }

  ~HighwayMatchVisitor()
  {
    LOG_TRACE("neighbor counts, max: " << _neighborCountMax << " mean: " <<
              (double)_neighborCountSum / (double)_elementsEvaluated);
  }

  void checkForMatch(const std::shared_ptr<const Element>& e)
  {
    LOG_VART(e->getElementId());

    // Find other nearby candidates.
    std::shared_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(getSearchRadius(e));
    set<ElementId> neighbors =
      SpatialIndexer::findNeighbors(*env, getIndex(), _indexToEid, getMap());
    ElementId elementBeingMatched(e->getElementType(), e->getId());

    _elementsEvaluated++;
    int neighborCount = 0;

    const bool medianMatchEnabled = _medianClassifier.get();
    for (const auto& elementId : neighbors)
    {
      if (elementBeingMatched != elementId)
      {
        const std::shared_ptr<const Element>& neighbor = _map->getElement(elementId);

        std::shared_ptr<HighwayClassifier> classifier;

        // Highway median to multiple divided road matching is separate matching routine from
        // regular road conflate. Check to see if the conditions for it are satisfied, and if so,
        // employ a different classifier.
        bool medianTagFound = false;
        if (medianMatchEnabled)
        {
          // neighbor always is a secondary feature (unknown2).
          const QString kvp =
            neighbor->getTags().getFirstMatchingKvp(
              ConfigOptions().getHighwayMedianIdentifyingTags());
          medianTagFound = !kvp.isEmpty();
          LOG_VART(medianTagFound);
          if (medianTagFound)
            classifier = _medianClassifier;
        }
        if (!medianTagFound)
          classifier = _classifier;

        // Score each candidate.
        std::shared_ptr<HighwayMatch> match =
          createMatch(_map, classifier, _sublineMatcher, _threshold, _tagAncestorDiff, e, neighbor);
        if (match)
        {
          _result.push_back(match);
          neighborCount++;
        }
      }
    }

    _neighborCountSum += neighborCount;
    _neighborCountMax = std::max(_neighborCountMax, neighborCount);
  }

  static std::shared_ptr<HighwayMatch> createMatch(const ConstOsmMapPtr& map,
                                                   std::shared_ptr<HighwayClassifier> classifier,
                                                   std::shared_ptr<SublineStringMatcher> sublineMatcher,
                                                   ConstMatchThresholdPtr threshold,
                                                   std::shared_ptr<TagAncestorDifferencer> tagAncestorDiff,
                                                   ConstElementPtr e1, ConstElementPtr e2)
  {
    std::shared_ptr<HighwayMatch> result;

    HighwayCriterion highwayCrit(map);
    if (e1 && e2 && e1->getStatus() != e2->getStatus() && e2->isUnknown() &&
        highwayCrit.isSatisfied(e1) && highwayCrit.isSatisfied(e2))
    {
      double maxEnumDiff = ConfigOptions().getHighwayMaxEnumDiff();

      // Don't want sidewalks matching with roads. Tried very hard to make this work in the schema
      // instead of adding this but could not due to the many interrelations between road types.
      // There should be a way to do it in the schema, however. Until that can be figured out, we
      // may find other minor highway types that will be proven to need this treatment as well.
      if (Tags::onlyOneContainsKvp(e1->getTags(), e2->getTags(), "footway=sidewalk"))
      {
        // This value still may need some tweaking after testing against additional datasets. Basing
        // this off of the default diff value rather than giving it its own config opt, since we've
        // never made a change to the default opt value to date.
        maxEnumDiff -= 0.2;
      }

      LOG_TRACE(
        "Tag diff between: " << e1->getElementId() << " and " << e2->getElementId() << ": " <<
        tagAncestorDiff->diff(map, e1, e2) << "; max diff: " << maxEnumDiff);
      if (tagAncestorDiff->diff(map, e1, e2) <= maxEnumDiff)
      {
        // score each candidate and push it on the result vector
        result =
          std::make_shared<HighwayMatch>(
            classifier, sublineMatcher, map, e1->getElementId(), e2->getElementId(), threshold);
        // if we're confident this is a miss
        if (result->getType() == MatchType::Miss)
          result.reset();
      }
    }
    return result;
  }

  Meters getSearchRadius(const std::shared_ptr<const Element>& e) const
  {
    Meters searchRadius;
    if (_searchRadius >= 0)
      searchRadius = _searchRadius;
    else
      searchRadius = e->getCircularError();
    LOG_VART(searchRadius);
    return searchRadius;
  }

  void visit(const ConstElementPtr& e) override
  {
    if (e->getStatus() == Status::Unknown1 && isMatchCandidate(e))
    {
      checkForMatch(e);
      _numMatchCandidatesVisited++;
      if (_numMatchCandidatesVisited % (_taskStatusUpdateInterval * 10) == 0)
      {
        PROGRESS_DEBUG(
          "\tProcessed " << StringUtils::formatLargeNumber(_numMatchCandidatesVisited) <<
          " match candidates / " <<
          StringUtils::formatLargeNumber(_map->getWayCount() + _map->getRelationCount()) <<
          " elements.");
      }
    }
    _numElementsVisited++;
    if (_numElementsVisited % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_STATUS(
        "\tProcessed " << StringUtils::formatLargeNumber(_numElementsVisited) << " of " <<
        StringUtils::formatLargeNumber(_map->getWayCount() + _map->getRelationCount()) <<
        " elements.");
    }
    if (_numElementsVisited % _memoryCheckUpdateInterval == 0)
      MemoryUsageChecker::getInstance().check();
  }

  bool isMatchCandidate(ConstElementPtr element) const
  {
    // The special tag is currently only used by roundabout processing to mark temporary features.
    if (element->getTags().contains(MetadataTags::HootSpecial()) ||
        (_filter && !_filter->isSatisfied(element)))
    {
      return false;
    }
    return HighwayCriterion(_map).isSatisfied(element);
  }

  std::shared_ptr<HilbertRTree>& getIndex()
  {
    if (!_index)
    {
      LOG_INFO("Creating highway feature index...");

      // No tuning was done, just copied these settings from OsmMapIndex. 10 children - 368 - see
      // #3054
      _index = std::make_shared<HilbertRTree>(std::make_shared<MemoryPageStore>(728), 2);

      // Only index elements satisfy isMatchCandidate(e)
      std::function<bool (ConstElementPtr e)> f =
        std::bind(&HighwayMatchVisitor::isMatchCandidate, this, placeholders::_1);
      std::shared_ptr<ArbitraryCriterion> pCrit = std::make_shared<ArbitraryCriterion>(f);

      SpatialIndexer v(
        _index, _indexToEid, pCrit,
        std::bind(&HighwayMatchVisitor::getSearchRadius, this, placeholders::_1), getMap());

      getMap()->visitRo(v);
      v.finalizeIndex();

      LOG_DEBUG(
        "Highway feature index created with " << StringUtils::formatLargeNumber(v.getSize()) <<
        " elements.");
    }

    return _index;
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  ConstOsmMapPtr getMap() const { return _map; }
  long getNumMatchCandidatesFound() const { return _numMatchCandidatesVisited; }

private:

  const ConstOsmMapPtr& _map;
  vector<ConstMatchPtr>& _result;
  set<ElementId> _empty;

  std::shared_ptr<HighwayClassifier> _classifier;
  std::shared_ptr<HighwayClassifier> _medianClassifier;
  std::shared_ptr<SublineStringMatcher> _sublineMatcher;
  ConstMatchThresholdPtr _threshold;

  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  Meters _searchRadius;

  std::shared_ptr<TagAncestorDifferencer> _tagAncestorDiff;

  ElementCriterionPtr _filter;

  // used for finding neighbors
  std::shared_ptr<HilbertRTree> _index;
  deque<ElementId> _indexToEid;

  long _numElementsVisited;
  long _numMatchCandidatesVisited;
  int _taskStatusUpdateInterval;
  int _memoryCheckUpdateInterval;
};

HighwayMatchCreator::HighwayMatchCreator()
  : _classifier(Factory::getInstance().constructObject<HighwayClassifier>(
                  ConfigOptions().getConflateMatchHighwayClassifier())),
    _sublineMatcher(SublineStringMatcherFactory::getMatcher(CreatorDescription::BaseFeatureType::Highway)),
    _tagAncestorDiff(std::make_shared<TagAncestorDifferencer>("highway"))
{
  // Enable/disable road median matching and validate associated configuration options.
  setRunMedianMatching(
    ConfigOptions().getHighwayMedianToDualHighwayMatch(),
    ConfigOptions().getHighwayMedianIdentifyingTags(),
    ConfigOptions().getHighwayMedianToDualHighwayTransferKeys());
}

void HighwayMatchCreator::setRunMedianMatching(const bool runMatching, const QStringList& identifyingTags,
                                               const QStringList& transferKeys)
{
  if (runMatching)
  {
    QStringList identifyingTagsTemp = identifyingTags;
    StringUtils::removeEmptyStrings(identifyingTagsTemp);
    if (identifyingTagsTemp.empty())
    {
      throw IllegalArgumentException(
        QString("No road median identifying tags specified in %1.").arg(ConfigOptions::getHighwayMedianIdentifyingTagsKey()));
    }

    QStringList transferKeysTemp = transferKeys;
    StringUtils::removeEmptyStrings(transferKeysTemp);
    if (transferKeysTemp.empty())
    {
      throw IllegalArgumentException(
        QString("No road median transfer tag keys specified in %1.").arg(ConfigOptions::getHighwayMedianToDualHighwayTransferKeysKey()));
    }

    _medianClassifier =
      Factory::getInstance().constructObject<HighwayClassifier>(ConfigOptions().getConflateMatchHighwayMedianClassifier());

    LOG_STATUS("Running highway road median custom conflation workflow...");
  }
  else
  {
    // HighwayMatchVisitor determines whether to run the median matching routine based on whether
    // this variable is initialized.
    _medianClassifier.reset();
  }
}

MatchPtr HighwayMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2)
{
  return
    HighwayMatchVisitor::createMatch(
      map, _classifier, _sublineMatcher, getMatchThreshold(), _tagAncestorDiff,
      map->getElement(eid1), map->getElement(eid2));
}

void HighwayMatchCreator::createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
                                        ConstMatchThresholdPtr threshold)
{
  QElapsedTimer timer;
  timer.start();

  // The parent does some initialization we need.
  MatchCreator::createMatches(map, matches, threshold);

  QString searchRadiusStr;
  const double searchRadius = ConfigOptions().getSearchRadiusHighway();
  if (searchRadius < 0)
    searchRadiusStr = "within a feature dependent search radius";
  else
    searchRadiusStr = "within a search radius of " + QString::number(searchRadius, 'g', 2) + " meters";

  LOG_INFO("Looking for matches with: " << className() << " " << searchRadiusStr << "...");
  LOG_VARD(*threshold);
  const int matchesSizeBefore = static_cast<int>(matches.size());

  HighwayMatchVisitor v(map, matches, _classifier, _medianClassifier, _sublineMatcher, threshold,
                        _tagAncestorDiff, _filter);
  map->visitWaysRo(v);
  map->visitRelationsRo(v);
  const int matchesSizeAfter = static_cast<int>(matches.size());

  LOG_STATUS(
    "\tFound " << StringUtils::formatLargeNumber(v.getNumMatchCandidatesFound()) <<
    " highway match candidates and " <<
    StringUtils::formatLargeNumber(matchesSizeAfter - matchesSizeBefore) <<
    " total matches in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

vector<CreatorDescription> HighwayMatchCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.emplace_back(
    className(),
    "Generates matchers that match roads with the 2nd Generation (Unifying) Algorithm",
    CreatorDescription::Highway,
    false);
  return result;
}

bool HighwayMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map)
{
  vector<ConstMatchPtr> matches;
  return HighwayMatchVisitor(map, matches, _filter).isMatchCandidate(element);
}

std::shared_ptr<MatchThreshold> HighwayMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    _matchThreshold =
      std::make_shared<MatchThreshold>(
        ConfigOptions().getHighwayMatchThreshold(),
        ConfigOptions().getHighwayMissThreshold(),
        ConfigOptions().getHighwayReviewThreshold());
  }
  return _matchThreshold;
}

QStringList HighwayMatchCreator::getCriteria() const
{
  return QStringList(HighwayCriterion::className());
}

}
