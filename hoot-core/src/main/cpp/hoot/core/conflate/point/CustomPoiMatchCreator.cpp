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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CustomPoiMatchCreator.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

#include "CustomPoiMatch.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, CustomPoiMatchCreator)

/**
 * Searches the specified map for any building match potentials.
 */
class CustomPoiMatchVisitor : public ElementVisitor
{
public:
  /**
   * @param matchStatus If the element's status matches this status then it is checked for a match.
   */
  CustomPoiMatchVisitor(const ConstOsmMapPtr& map,
    vector<const Match*>& result, Meters worstCircularError,
    ConstMatchThresholdPtr mt) :
    _map(map),
    _result(result),
    _threshold(mt)
  {
    _worstCircularError = worstCircularError;
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
    _maxGroupSize = 0;
    _rejectScore = 0.9;
    _rf.reset(new PoiRfClassifier());
  }

  ~CustomPoiMatchVisitor()
  {
  }

  void checkForMatch(const shared_ptr<const Element>& e)
  {
    auto_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(e->getCircularError() + _worstCircularError);

    // find other nearby candidates
    vector<long> neighbors = _map->getIndex().findNodes(*env);
    ElementId from(e->getElementType(), e->getId());

    _elementsEvaluated++;
    int neighborCount = 0;
    vector<double> classes;

    for (vector<long>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it)
    {
      ElementId eid(ElementType::Node, *it);

      if (from.getId() < eid.getId())
      {
        const shared_ptr<const Element>& n = _map->getElement(eid);
        if (OsmSchema::getInstance().isPoi(*n))
        {
          // score each candidate and push it on the result vector
          CustomPoiMatch* m = new CustomPoiMatch(_map, _rf, from, eid, _threshold);
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

  virtual void visit(const ConstElementPtr& e)
  {
    if (isMatchCandidate(e))
    {
      checkForMatch(e);
    }
  }

  static bool isMatchCandidate(ConstElementPtr element)
  {
    return element->getElementType() == ElementType::Node && OsmSchema::getInstance().isPoi(*element);
  }

private:

  const ConstOsmMapPtr& _map;
  vector<const Match*>& _result;
  set<ElementId> _empty;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  /// reject any manipulation with a miss score >= _rejectScore
  double _rejectScore;
  PoiRfClassifierPtr _rf;
  ConstMatchThresholdPtr _threshold;
  Meters _worstCircularError;
};

class WorstCircularErrorVisitor : public ElementConstOsmMapVisitor
{
public:
  Meters _worst;

  WorstCircularErrorVisitor()
  {
    _worst = -1;
  }

  Meters getWorstCircularError() { return _worst; }

  virtual void visit(const shared_ptr<const Element>& e)
  {
    _worst = max(_worst, e->getCircularError());
  }
};


CustomPoiMatchCreator::CustomPoiMatchCreator()
{
}

CustomPoiMatchCreator::~CustomPoiMatchCreator()
{
}

void CustomPoiMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match *> &matches,
  ConstMatchThresholdPtr threshold)
{
  WorstCircularErrorVisitor wav;
  map->visitRo(wav);
  CustomPoiMatchVisitor v(map, matches, wav.getWorstCircularError(), threshold);
  map->visitRo(v);
}

vector<MatchCreator::Description> CustomPoiMatchCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "Custom POI Match Creator", MatchCreator::POI, true));

  return result;
}

bool CustomPoiMatchCreator::isMatchCandidate(ConstElementPtr element,
                                             const ConstOsmMapPtr& /*map*/)
{
  return CustomPoiMatchVisitor::isMatchCandidate(element);
}

shared_ptr<MatchThreshold> CustomPoiMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getPoiMatchThreshold(), config.getPoiMissThreshold(),
                         config.getPoiReviewThreshold()));
  }
  return _matchThreshold;
}

}
