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
#include "BuildingMatchCreator.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Settings.h>

// Standard
#include <fstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

#include "BuildingMergeManipulator.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, BuildingMatchCreator)

using namespace Tgs;

/**
 * Searches the specified map for any building match potentials.
 */
class BuildingMatchVisitor : public ElementVisitor
{
public:
  /**
   * @param matchStatus If the element's status matches this status then it is checked for a match.
   */
  BuildingMatchVisitor(const ConstOsmMapPtr& map,
    vector<const Match*>& result, shared_ptr<BuildingRfClassifier> rf,
    ConstMatchThresholdPtr threshold, Status matchStatus = Status::Invalid) :
    _map(map),
    _result(result),
    _rf(rf),
    _mt(threshold),
    _matchStatus(matchStatus)
  {
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
    _maxGroupSize = 0;
  }

  ~BuildingMatchVisitor()
  {
    LOG_DEBUG("neighbor counts, max: " << _neighborCountMax << " mean: " <<
             (double)_neighborCountSum / (double)_elementsEvaluated);
  }

  void checkForMatch(const shared_ptr<const Element>& e)
  {
    auto_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(e->getCircularError());

    // find other nearby candidates
    set<ElementId> neighbors = _map->findElements(*env);
    ElementId from(e->getElementType(), e->getId());

    _elementsEvaluated++;
    int neighborCount = 0;

    for (set<ElementId>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      if (from != *it)
      {
        const shared_ptr<const Element>& n = _map->getElement(*it);
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
            _result.push_back(m);
            neighborCount++;
          }
        }
      }
    }

    _neighborCountSum += neighborCount;
    _neighborCountMax = std::max(_neighborCountMax, neighborCount);
  }

  BuildingMatch* createMatch(ElementId eid1, ElementId eid2)
  {
    return new BuildingMatch(_map, _rf, eid1, eid2, _mt);
  }

  static bool isRelated(ConstElementPtr e1, ConstElementPtr e2)
  {
    if (e1->getStatus() != e2->getStatus() &&
        e1->isUnknown() && e2->isUnknown() &&
        OsmSchema::getInstance().isBuilding(e1->getTags(), e1->getElementType()) &&
        OsmSchema::getInstance().isBuilding(e2->getTags(), e2->getElementType()))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getStatus() == _matchStatus && isMatchCandidate(e))
    {
      checkForMatch(e);
    }
  }

  static bool isMatchCandidate(ConstElementPtr element)
  {
    return OsmSchema::getInstance().isBuilding(element->getTags(), element->getElementType());
  }

private:
  const ConstOsmMapPtr& _map;
  vector<const Match*>& _result;
  set<ElementId> _empty;
  shared_ptr<BuildingRfClassifier> _rf;
  ConstMatchThresholdPtr _mt;
  Status _matchStatus;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  /// reject any manipulation with a miss score >= _rejectScore
  double _rejectScore;
};

BuildingMatchCreator::BuildingMatchCreator()
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
      // score each candidate and push it on the result vector
      result = new BuildingMatch(map, _getRf(), eid1, eid2, getMatchThreshold());
    }
  }

  return result;
}

void BuildingMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
  ConstMatchThresholdPtr threshold)
{
  LOG_VAR(*threshold);
  BuildingMatchVisitor v(map, matches, _getRf(), threshold, Status::Unknown1);
  map->visitRo(v);
}

vector<MatchCreator::Description> BuildingMatchCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "Building Match Creator", false));

  return result;
}

bool BuildingMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& /*map*/)
{
  return BuildingMatchVisitor::isMatchCandidate(element);
}

shared_ptr<MatchThreshold> BuildingMatchCreator::getMatchThreshold()
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

shared_ptr<BuildingRfClassifier> BuildingMatchCreator::_getRf() const
{
  if (!_rf)
  {
    QString path = ConfPath::search(ConfigOptions().getBuildingModelPath());
    LOG_INFO("Loading model from: " << path);

    ifstream fp;
    fp.open(path.toAscii().data());
    if (!fp.is_open())
    {
      throw HootException("Error opening file: " + path);
    }
    _rf.reset(new BuildingRfClassifier());
    _rf->import(fp);
  }

  return _rf;
}

}
