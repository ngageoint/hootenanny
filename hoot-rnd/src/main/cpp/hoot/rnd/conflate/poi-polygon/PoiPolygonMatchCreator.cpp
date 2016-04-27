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
#include "PoiPolygonMatchCreator.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/rnd/conflate/poi-polygon/PoiPolygonMatch.h>

// Standard
#include <fstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, PoiPolygonMatchCreator)

using namespace Tgs;

/**
 * Searches the specified map for any match potentials.
 */
class PoiPolygonMatchVisitor : public ElementVisitor
{
public:
  PoiPolygonMatchVisitor(const ConstOsmMapPtr& map, vector<const Match*>& result,
                         ConstMatchThresholdPtr threshold) :
    _map(map),
    _result(result),
    _threshold(threshold)
  {
    _neighborCountMax = -1;
    _neighborCountSum = 0;
    _elementsEvaluated = 0;
  }

  ~PoiPolygonMatchVisitor()
  {
  }

  void checkForMatch(const shared_ptr<const Element>& e)
  {
    auto_ptr<Envelope> env(e->getEnvelope(_map));
    env->expandBy(e->getCircularError() + ConfigOptions().getPoiPolygonMatchReviewDistance());

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

        if (n->isUnknown() &&
            PoiPolygonMatch::isBuildingIsh(n))
        {
          // score each candidate and push it on the result vector
          PoiPolygonMatch* m = new PoiPolygonMatch(_map, from, *it, _threshold);

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
    return element->isUnknown() && PoiPolygonMatch::isPoiIsh(element);
  }

private:

  const ConstOsmMapPtr& _map;
  vector<const Match*>& _result;
  set<ElementId> _empty;
  int _neighborCountMax;
  int _neighborCountSum;
  int _elementsEvaluated;
  size_t _maxGroupSize;
  ConstMatchThresholdPtr _threshold;
};

PoiPolygonMatchCreator::PoiPolygonMatchCreator()
{
}

Match* PoiPolygonMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1,
  ElementId eid2)
{
  Match* result = 0;

  if (eid1.getType() != eid2.getType())
  {
    ConstElementPtr e1 = map->getElement(eid1);
    ConstElementPtr e2 = map->getElement(eid2);
    bool foundPoi = OsmSchema::getInstance().isPoi(*e1) || OsmSchema::getInstance().isPoi(*e2);
    bool foundBuilding = OsmSchema::getInstance().isBuilding(e1->getTags(), e1->getElementType()) ||
        OsmSchema::getInstance().isBuilding(e2->getTags(), e2->getElementType());

    if (foundPoi && foundBuilding)
    {
      result = new PoiPolygonMatch(map, eid1, eid2, getMatchThreshold());
    }
  }

  return result;
}

void PoiPolygonMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
  ConstMatchThresholdPtr threshold)
{
  PoiPolygonMatchVisitor v(map, matches, threshold);
  map->visitRo(v);
}

vector<MatchCreator::Description> PoiPolygonMatchCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "POI to Polygon Match Creator", MatchCreator::POI, true));

  return result;
}

bool PoiPolygonMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& /*map*/)
{
  return PoiPolygonMatchVisitor::isMatchCandidate(element);
}

shared_ptr<MatchThreshold> PoiPolygonMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getPoiPolygonMatchThreshold(), config.getPoiPolygonMissThreshold(),
                         config.getPoiPolygonReviewThreshold()));
  }
  return _matchThreshold;
}

}
