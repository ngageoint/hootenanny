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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwayMatchCreator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/subline-matching/MaximalNearestSublineMatcher.h>
#include <hoot/core/algorithms/subline-matching/MaximalSublineStringMatcher.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/visitors/SpatialIndexer.h>
#include <hoot/core/conflate/highway/HighwayClassifier.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/schema/TagAncestorDifferencer.h>
#include <hoot/core/util/StringUtils.h>

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
                      ElementCriterionPtr filter = ElementCriterionPtr()) :
  _map(map),
  _result(result),
  _filter(filter)
  {
  }

  /**
   * @param matchStatus If the element's status matches this status then it is checked for a match.
   *
   * This constructor has gotten a little out of hand.
   */
  HighwayMatchVisitor(const ConstOsmMapPtr& map,
    vector<ConstMatchPtr>& result, std::shared_ptr<HighwayClassifier> c,
    std::shared_ptr<SublineStringMatcher> sublineMatcher, Status matchStatus,
    ConstMatchThresholdPtr threshold,
    std::shared_ptr<TagAncestorDifferencer> tagAncestorDiff,
    ElementCriterionPtr filter = ElementCriterionPtr()):
    _map(map),
    _result(result),
    _c(c),
    _sublineMatcher(sublineMatcher),
    _matchStatus(matchStatus),
    _threshold(threshold),
    _tagAncestorDiff(tagAncestorDiff),
    _filter(filter)
  {
    ConfigOptions opts = ConfigOptions();
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
    _searchRadius = opts.getSearchRadiusHighway();
    _numElementsVisited = 0;
    _numMatchCandidatesVisited = 0;
    _taskStatusUpdateInterval = opts.getTaskStatusUpdateInterval();
  }

  ~HighwayMatchVisitor()
  {
    LOG_TRACE("neighbor counts, max: " << _neighborCountMax << " mean: " <<
              (double)_neighborCountSum / (double)_elementsEvaluated);
  }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }

  void checkForMatch(const std::shared_ptr<const Element>& e)
  {
    LOG_VART(e->getElementId());
    //LOG_VART(e);

    std::shared_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(getSearchRadius(e));

    // find other nearby candidates
    set<ElementId> neighbors =
      SpatialIndexer::findNeighbors(*env, getIndex(), _indexToEid, getMap());

    ElementId from(e->getElementType(), e->getId());

    _elementsEvaluated++;
    int neighborCount = 0;

    for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      if (from != *it)
      {
        const std::shared_ptr<const Element>& n = _map->getElement(*it);
        // score each candidate and push it on the result vector
        std::shared_ptr<HighwayMatch> match =
          createMatch(_map, _c, _sublineMatcher, _threshold, _tagAncestorDiff, e, n);
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
    if (e1 && e2 &&
        e1->getStatus() != e2->getStatus() && e2->isUnknown() &&
        highwayCrit.isSatisfied(e1) && highwayCrit.isSatisfied(e2) &&
        tagAncestorDiff->diff(map, e1, e2) <= ConfigOptions().getHighwayMaxEnumDiff())
    {
      // score each candidate and push it on the result vector
      result.reset(
        new HighwayMatch(
          classifier, sublineMatcher, map, e1->getElementId(), e2->getElementId(), threshold));
      // if we're confident this is a miss
      if (result->getType() == MatchType::Miss)
      {
        result.reset();
      }
    }

    return result;
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

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getStatus() == _matchStatus && isMatchCandidate(e))
    {
      checkForMatch(e);

      _numMatchCandidatesVisited++;
      if (_numMatchCandidatesVisited % (_taskStatusUpdateInterval * 10) == 0)
      {
        PROGRESS_DEBUG(
          "Processed " << _numMatchCandidatesVisited << " match candidates / " <<
          _map->getElementCount() << " total elements.");
      }
    }

    _numElementsVisited++;
    if (_numElementsVisited % (_taskStatusUpdateInterval * 10) == 0)
    {
      PROGRESS_INFO(
        "Processed " << StringUtils::formatLargeNumber(_numElementsVisited) << " / " <<
        StringUtils::formatLargeNumber(_map->getElementCount()) << " elements.");
    }
  }

  bool isMatchCandidate(ConstElementPtr element)
  {
    if (_filter && !_filter->isSatisfied(element))
    {
      return false;
    }
    return HighwayCriterion(_map).isSatisfied(element);
  }

  std::shared_ptr<HilbertRTree>& getIndex()
  {
    if (!_index)
    {
      LOG_STATUS("Creating highway feature index...");

      // No tuning was done, I just copied these settings from OsmMapIndex.
      // 10 children - 368 - see #3054
      std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
      _index.reset(new HilbertRTree(mps, 2));

      // Only index elements satisfy isMatchCandidate(e)
      std::function<bool (ConstElementPtr e)> f =
        std::bind(&HighwayMatchVisitor::isMatchCandidate, this, placeholders::_1);
      std::shared_ptr<ArbitraryCriterion> pCrit(new ArbitraryCriterion(f));

      // Instantiate our visitor
      SpatialIndexer v(
        _index, _indexToEid, pCrit,
        std::bind(&HighwayMatchVisitor::getSearchRadius, this, placeholders::_1), getMap());

      getMap()->visitRo(v);
      v.finalizeIndex();

      LOG_STATUS(
        "Highway feature index created with " << StringUtils::formatLargeNumber(v.getSize()) <<
        " elements.");
    }

    return _index;
  }

  ConstOsmMapPtr getMap() { return _map; }

  long getNumMatchCandidatesFound() const { return _numMatchCandidatesVisited; }

private:

  const ConstOsmMapPtr& _map;
  vector<ConstMatchPtr>& _result;
  set<ElementId> _empty;
  std::shared_ptr<HighwayClassifier> _c;
  std::shared_ptr<SublineStringMatcher> _sublineMatcher;
  Status _matchStatus;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  Meters _searchRadius;
  ConstMatchThresholdPtr _threshold;
  std::shared_ptr<TagAncestorDifferencer> _tagAncestorDiff;
  double _highwayMaxEnumDiff;
  ElementCriterionPtr _filter;

  // Used for finding neighbors
  std::shared_ptr<HilbertRTree> _index;
  deque<ElementId> _indexToEid;

  long _numElementsVisited;
  long _numMatchCandidatesVisited;
  int _taskStatusUpdateInterval;
};

HighwayMatchCreator::HighwayMatchCreator()
{
  _classifier.reset(
    Factory::getInstance().constructObject<HighwayClassifier>(
      ConfigOptions().getConflateMatchHighwayClassifier()));
  _sublineMatcher.reset(
    Factory::getInstance().constructObject<SublineStringMatcher>(
      ConfigOptions().getHighwaySublineStringMatcher()));

  _tagAncestorDiff = std::shared_ptr<TagAncestorDifferencer>(new TagAncestorDifferencer("highway"));

  Settings settings = conf();
  settings.set("way.matcher.max.angle", ConfigOptions().getHighwayMatcherMaxAngle());
  settings.set("way.subline.matcher", ConfigOptions().getHighwaySublineMatcher());
  settings.set("way.matcher.heading.delta", ConfigOptions().getHighwayMatcherHeadingDelta());
  _sublineMatcher->setConfiguration(settings);
}

MatchPtr HighwayMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2)
{
  return HighwayMatchVisitor::createMatch(map, _classifier, _sublineMatcher, getMatchThreshold(),
    _tagAncestorDiff, map->getElement(eid1), map->getElement(eid2));
}

void HighwayMatchCreator::createMatches(
  const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches, ConstMatchThresholdPtr threshold)
{
  QElapsedTimer timer;
  timer.start();

  QString searchRadiusStr;
  const double searchRadius = ConfigOptions().getSearchRadiusHighway();
  if (searchRadius < 0)
  {
    searchRadiusStr = "within a feature dependent search radius";
  }
  else
  {
    searchRadiusStr =
      "within a search radius of " + QString::number(searchRadius, 'g', 2) + " meters";
  }
  LOG_STATUS("Looking for matches with: " << className() << " " << searchRadiusStr << "...");
  LOG_VARD(*threshold);
  const int matchesSizeBefore = matches.size();

  HighwayMatchVisitor v(
    map, matches, _classifier, _sublineMatcher, Status::Unknown1, threshold, _tagAncestorDiff,
    _filter);
  map->visitWaysRo(v);
  map->visitRelationsRo(v);
  const int matchesSizeAfter = matches.size();

  LOG_STATUS(
    "Found " << StringUtils::formatLargeNumber(v.getNumMatchCandidatesFound()) <<
    " highway match candidates and " <<
    StringUtils::formatLargeNumber(matchesSizeAfter - matchesSizeBefore) <<
    " total matches in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

vector<CreatorDescription> HighwayMatchCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.push_back(
    CreatorDescription(
      className(),
      "Generates matchers that match roads with the 2nd Generation (Unifying) Algorithm",
      CreatorDescription::Highway, false));
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
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getHighwayMatchThreshold(), config.getHighwayMissThreshold(),
                         config.getHighwayReviewThreshold()));
  }
  return _matchThreshold;
}

}
