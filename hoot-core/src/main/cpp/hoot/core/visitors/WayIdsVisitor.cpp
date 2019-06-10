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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "WayIdsVisitor.h"
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/ContainsNodeCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>

using namespace std;

namespace hoot
{

WayIdsVisitor::WayIdsVisitor (ElementCriterion* pCrit):
  _pCrit(pCrit)
{
}

void WayIdsVisitor::visit(const std::shared_ptr<const Element>& e)
{
  if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e);
    if (_pCrit->isSatisfied(e))
    {
      _wayIds.push_back(e->getId());
    }
  }
}


// Convenience method for finding ways that match the given criterion
vector<long> WayIdsVisitor::findWays(const ConstOsmMapPtr& map, ElementCriterion* pCrit)
{
  WayIdsVisitor v(pCrit);
  map->visitWaysRo(v);
  return v.getIds();
}

vector<long> WayIdsVisitor::findWays(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                       ConstWayPtr refWay, Meters maxDistance, bool addError)
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
vector<long> WayIdsVisitor::findWaysByNode(const ConstOsmMapPtr& map, long nodeId)
{
  ContainsNodeCriterion crit(nodeId);
  WayIdsVisitor v(&crit);
  map->visitWaysRo(v);
  return v.getIds();
}

// Convenience method for finding ways that contain the given tag
vector<long> WayIdsVisitor::findWaysByTag(const ConstOsmMapPtr& map, const QString& key,
                                            const QString& value)
{
  TagCriterion crit(key, value);
  WayIdsVisitor v(&crit);
  map->visitWaysRo(v);
  return v.getIds();
}

}
