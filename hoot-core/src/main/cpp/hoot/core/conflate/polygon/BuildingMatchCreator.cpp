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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingMatchCreator.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/conflate/polygon/BuildingRfClassifier.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/IndexElementsVisitor.h>

// Standard
#include <fstream>

// Boost
#include <boost/bind.hpp>
#include <boost/function.hpp>

// tgs
#include <tgs/RandomForest/RandomForest.h>
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>

//Qt
#include <QFile>

using namespace geos::geom;

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

  BuildingMatchVisitor(const ConstOsmMapPtr& map, std::vector<const Match*>& result,
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
    std::vector<const Match*>& result, boost::shared_ptr<BuildingRfClassifier> rf,
    ConstMatchThresholdPtr threshold, ElementCriterionPtr filter = ElementCriterionPtr(),
    Status matchStatus = Status::Invalid, bool reviewMatchesOtherThanOneToOne = false) :
    _map(map),
    _result(result),
    _rf(rf),
    _mt(threshold),
    _filter(filter),
    _matchStatus(matchStatus),
    _reviewMatchesOtherThanOneToOne(reviewMatchesOtherThanOneToOne)
  {
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
    _maxGroupSize = 0;
    _numElementsVisited = 0;
    _numMatchCandidatesVisited = 0;
    _taskStatusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  }

  ~BuildingMatchVisitor()
  {
    LOG_TRACE("neighbor counts, max: " << _neighborCountMax << " mean: " <<
              (double)_neighborCountSum / (double)_elementsEvaluated);
  }

  virtual QString getDescription() const { return ""; }

  void checkForMatch(const boost::shared_ptr<const Element>& e)
  {
    boost::shared_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(e->getCircularError());

    // find other nearby candidates
    std::set<ElementId> neighbors = IndexElementsVisitor::findNeighbors(*env,
                                                                        getIndex(),
                                                                        _indexToEid,
                                                                        getMap());
    ElementId from(e->getElementType(), e->getId());

    _elementsEvaluated++;
    int neighborCount = 0;

    std::vector<Match*> tempMatches;
    for (std::set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      if (from != *it)
      {
        const boost::shared_ptr<const Element>& n = _map->getElement(*it);
        if (isRelated(n, e))
        {
          // score each candidate and push it on the result vector
          BuildingMatch* m = createMatch(from, *it);
          // if we're confident this is a miss
          if (m->getType() == MatchType::Miss)
          {
            delete m;
          }
          else
          {
            tempMatches.push_back(m);
            neighborCount++;
          }
        }
      }
    }

    if (_reviewMatchesOtherThanOneToOne && neighborCount > 1)
    {
      for (std::vector<Match*>::iterator it = tempMatches.begin(); it != tempMatches.end(); ++it)
      {
        Match* match = *it;
        //Not proud of this, but not sure what else to do at this point w/o having to change the
        //Match interface.
        MatchClassification& matchClass =
          const_cast<MatchClassification&>(match->getClassification());
        matchClass.setReview();
        match->setExplain("Match involved in multiple building relationships.");
      }
    }

    for (std::vector<Match*>::const_iterator it = tempMatches.begin(); it != tempMatches.end(); ++it)
    {
      _result.push_back(*it);
    }

    _neighborCountSum += neighborCount;
    _neighborCountMax = std::max(_neighborCountMax, neighborCount);
  }

  BuildingMatch* createMatch(ElementId eid1, ElementId eid2)
  {
    ConfigOptions opts = ConfigOptions();
    return new
      BuildingMatch(
        _map, _rf, eid1, eid2, _mt, opts.getBuildingReviewIfSecondaryNewer(),
        opts.getBuildingDateTagKey(), opts.getBuildingDateFormat());
  }

  static bool isRelated(ConstElementPtr e1, ConstElementPtr e2)
  {
    BuildingCriterion buildingCrit(false);
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

  Meters getSearchRadius(const boost::shared_ptr<const Element>& e) const
  {
    LOG_VART(e->getCircularError());
    return e->getCircularError();
  }

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getStatus() == _matchStatus && isMatchCandidate(e))
    {
      checkForMatch(e);

      _numMatchCandidatesVisited++;
      if (_numMatchCandidatesVisited % _taskStatusUpdateInterval == 0)
      {
        PROGRESS_DEBUG(
          "Processed " << _numMatchCandidatesVisited << " match candidates / " <<
          _map->getElementCount() << " total elements.");
      }
    }

    _numElementsVisited++;
    if (_numElementsVisited % _taskStatusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Processed " << _numElementsVisited << " / " << _map->getElementCount() << " elements.");
    }
  }

  bool isMatchCandidate(ConstElementPtr element)
  {
    if (_filter && !_filter->isSatisfied(element))
    {
      return false;
    }
    return BuildingCriterion().isSatisfied(element);
  }

  boost::shared_ptr<HilbertRTree>& getIndex()
  {
    if (!_index)
    {
      // No tuning was done, I just copied these settings from OsmMapIndex.
      // 10 children - 368
      boost::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
      _index.reset(new HilbertRTree(mps, 2));

      // Only index elements that isMatchCandidate(e)
      boost::function<bool (ConstElementPtr e)> f =
        boost::bind(&BuildingMatchVisitor::isMatchCandidate, this, _1);
      boost::shared_ptr<ArbitraryCriterion> pCrit(new ArbitraryCriterion(f));

      // Instantiate our visitor
      IndexElementsVisitor v(_index,
                             _indexToEid,
                             pCrit,
                             boost::bind(&BuildingMatchVisitor::getSearchRadius, this, _1),
                             getMap());

      getMap()->visitRo(v);
      v.finalizeIndex();
    }

    return _index;
  }

  ConstOsmMapPtr getMap() { return _map; }

  long getNumMatchCandidatesFound() const { return _numMatchCandidatesVisited; }

private:

  const ConstOsmMapPtr& _map;
  std::vector<const Match*>& _result;
  std::set<ElementId> _empty;
  boost::shared_ptr<BuildingRfClassifier> _rf;
  ConstMatchThresholdPtr _mt;
  ElementCriterionPtr _filter;
  Status _matchStatus;
  bool _reviewMatchesOtherThanOneToOne;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  /// reject any manipulation with a miss score >= _rejectScore
  double _rejectScore;

  // Used for finding neighbors
  boost::shared_ptr<HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  long _numElementsVisited;
  long _numMatchCandidatesVisited;
  int _taskStatusUpdateInterval;
};

BuildingMatchCreator::BuildingMatchCreator() :
_conflateMatchBuildingModel(ConfigOptions().getConflateMatchBuildingModel())
{
}

Match* BuildingMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2)
{
  BuildingMatch* result = 0;

  if (eid1.getType() != ElementType::Node && eid2.getType() != ElementType::Node)
  {
    ConstElementPtr e1 = map->getElement(eid1);
    ConstElementPtr e2 = map->getElement(eid2);

    if (BuildingMatchVisitor::isRelated(e1, e2))
    {
      ConfigOptions opts = ConfigOptions(conf());
      // score each candidate and push it on the result vector
      result = new BuildingMatch(map, _getRf(), eid1, eid2, getMatchThreshold(),
                                 opts.getBuildingReviewIfSecondaryNewer(),
                                 opts.getBuildingDateTagKey(), opts.getBuildingDateFormat());
    }
  }

  return result;
}

void BuildingMatchCreator::createMatches(const ConstOsmMapPtr& map, std::vector<const Match*>& matches,
  ConstMatchThresholdPtr threshold)
{
  LOG_DEBUG("Creating matches with: " << className() << "...");
  LOG_VARD(*threshold);
  BuildingMatchVisitor v(
    map, matches, _getRf(), threshold, _filter, Status::Unknown1,
    ConfigOptions().getBuildingReviewMatchesOtherThanOneToOne());
  map->visitRo(v);
  LOG_INFO("Found " << v.getNumMatchCandidatesFound() << " building match candidates.");
}

std::vector<CreatorDescription> BuildingMatchCreator::getAllCreators() const
{
  std::vector<CreatorDescription> result;
  result.push_back(
    CreatorDescription(className(), "Matches buildings", CreatorDescription::Building, false));
  return result;
}

boost::shared_ptr<BuildingRfClassifier> BuildingMatchCreator::_getRf()
{
  if (!_rf)
  {
    QString path = ConfPath::search(_conflateMatchBuildingModel);
    LOG_DEBUG("Loading model from: " << path);

    QFile file(path.toAscii().data());
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
    //LOG_VARD(doc.toString());
    file.close();

    _rf.reset(new BuildingRfClassifier());
    QDomElement docRoot = doc.elementsByTagName("RandomForest").at(0).toElement();
    _rf->import(docRoot);
  }

  return _rf;
}

bool BuildingMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map)
{
  std::vector<const Match*> matches;
  return BuildingMatchVisitor(map, matches, _filter).isMatchCandidate(element);
}

boost::shared_ptr<MatchThreshold> BuildingMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getBuildingMatchThreshold(), config.getBuildingMissThreshold(),
                         config.getBuildingReviewThreshold()));
  }
  return _matchThreshold;
}

}
