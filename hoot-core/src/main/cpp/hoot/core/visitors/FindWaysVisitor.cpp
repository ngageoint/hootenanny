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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "FindWaysVisitor.h"
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/filters/ContainsNodeCriterion.h>
#include <hoot/core/filters/TagCriterion.h>

using namespace std;

namespace hoot
{

FindWaysVisitor::FindWaysVisitor (ElementCriterion* pCrit):
  _pCrit(pCrit)
{
}

void FindWaysVisitor::visit(const boost::shared_ptr<const Element>& e)
{
  if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr w = boost::dynamic_pointer_cast<const Way>(e);
    if (_pCrit->isSatisfied(e))
    {
      _wayIds.push_back(e->getId());
    }
  }
}


// Convenience method for finding ways that match the given criterion
vector<long> FindWaysVisitor::findWays(const ConstOsmMapPtr& map, ElementCriterion* pCrit)
{
  FindWaysVisitor v(pCrit);
  map->visitWaysRo(v);
  return v.getIds();
}

vector<long> FindWaysVisitor::findWays(const ConstOsmMapPtr& map,
                                       ElementCriterion* pCrit,
                                      ConstWayPtr refWay,
                                       Meters maxDistance,
                                       bool addError)
{
  vector<long> close = map->getIndex().findWayNeighbors(refWay, maxDistance, addError);
  vector<long> result;

  for (size_t i = 0; i < close.size(); i++)
  {
    const ConstWayPtr& w = map->getWay(close[i]);
    if (pCrit->isSatisfied(w))
      result.push_back(w->getId());
  }

  return result;
}

// Convenience method for finding ways that contain the given node
vector<long> FindWaysVisitor::findWaysByNode(const ConstOsmMapPtr& map, long nodeId)
{
  ContainsNodeCriterion crit(nodeId);
  FindWaysVisitor v(&crit);
  map->visitWaysRo(v);
  return v.getIds();
}

// Convenience method for finding ways that contain the given tag
vector<long> FindWaysVisitor::findWaysByTag(const ConstOsmMapPtr& map,
                                  const QString& key,
                                  const QString& value)
{
  TagCriterion crit(key, value);
  FindWaysVisitor v(&crit);
  map->visitWaysRo(v);
  return v.getIds();
}

}
