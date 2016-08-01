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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "FindNodesVisitor.h"
#include <hoot/core/index/OsmMapIndex.h>

namespace hoot
{

FindNodesVisitor::FindNodesVisitor (ElementCriterion* pCrit):
  _pCrit(pCrit)
{
  // This space intentionally left blank
}

void FindNodesVisitor::visit(const shared_ptr<const Element>& e)
{
  if (e->getElementType() == ElementType::Node)
  {
    ConstNodePtr pNode = dynamic_pointer_cast<const Node>(e);
    if (_pCrit->isSatisfied(e))
    {
      _nodeIds.push_back(e->getId());
    }
  }
}

// Convenience method for finding ways that match the given criterion
vector<long> FindNodesVisitor::findNodes(const ConstOsmMapPtr& map,
                                         ElementCriterion* pCrit)
{
  FindNodesVisitor v(pCrit);
  map->visitNodesRo(v);
  return v.getIds();
}

vector<long> FindNodesVisitor::findNodes(const ConstOsmMapPtr& map,
                                         ElementCriterion* pCrit,
                                         const Coordinate& refCoord,
                                         Meters maxDistance)
{
  vector<long> close = map->getIndex().findNodes(refCoord, maxDistance);
  vector<long> result;

  for (size_t i = 0; i < close.size(); i++)
  {
    const shared_ptr<const Node>& n = map->getNode(close[i]);
    if (pCrit->isSatisfied(n))
      result.push_back(n->getId());
  }

  return result;
}

// Convenience method for finding nodes that contain the given tag
vector<long> FindNodesVisitor::findNodesByTag(const ConstOsmMapPtr& map,
                                              const QString& key,
                                              const QString& value)
{
  TagCriterion crit(key, value);
  FindNodesVisitor v(&crit);
  map->visitNodesRo(v);
  return v.getIds();
}

} // namespace hoot

