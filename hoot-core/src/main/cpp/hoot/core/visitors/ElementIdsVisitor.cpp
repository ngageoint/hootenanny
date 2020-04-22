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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ElementIdsVisitor.h"
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/ContainsNodeCriterion.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

ElementIdsVisitor::ElementIdsVisitor(const ElementType& elementType, ElementCriterion* pCrit) :
_elementType(elementType),
_pCrit(pCrit)
{
  if (_elementType == ElementType::Relation)
  {
    // why is this?
    throw NotImplementedException("ElementIdsVisitor does not currently support relations.");
  }
}

void ElementIdsVisitor::visit(const std::shared_ptr<const Element>& e)
{
  if (e->getElementType() == _elementType)
  {
    if (_pCrit->isSatisfied(e))
    {
      _elementIds.push_back(e->getId());
    }
  }
}

// Convenience method for finding elements that match the given criterion
vector<long> ElementIdsVisitor::findElements(const ConstOsmMapPtr& map,
                                             const ElementType& elementType,
                                             ElementCriterion* pCrit)
{
  ElementIdsVisitor v(elementType, pCrit);
  if (elementType == ElementType::Node)
  {
    map->visitNodesRo(v);
  }
  else
  {
    map->visitWaysRo(v);
  }
  return v.getIds();
}

// TODO: Some of these may be redundant with related methods in WayNodeUtils.

vector<long> ElementIdsVisitor::_findCloseNodes(const ConstOsmMapPtr& map,
                                                const Coordinate& refCoord, Meters maxDistance)
{
  return map->getIndex().findNodes(refCoord, maxDistance);
}

vector<long> ElementIdsVisitor::_findCloseWays(const ConstOsmMapPtr& map,
                                               ConstWayPtr refWay, Meters maxDistance, bool addError)
{
  return map->getIndex().findWayNeighbors(refWay, maxDistance, addError);
}

vector<long> ElementIdsVisitor::_findElements(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                              const vector<long>& closeElementIds)
{
  vector<long> result;
  for (size_t i = 0; i < closeElementIds.size(); i++)
  {
    const ConstElementPtr& e = map->getElement(ElementId(ElementType::Node, closeElementIds[i]));
    if (pCrit->isSatisfied(e))
      result.push_back(e->getId());
  }
  return result;
}

vector<long> ElementIdsVisitor::findNodes(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                          const Coordinate& refCoord, Meters maxDistance)
{
  return _findElements(map, pCrit, map->getIndex().findNodes(refCoord, maxDistance));
}

vector<long> ElementIdsVisitor::findWays(const ConstOsmMapPtr& map, ElementCriterion* pCrit,
                                         ConstWayPtr refWay, Meters maxDistance, bool addError)
{
  return _findElements(map, pCrit, map->getIndex().findWayNeighbors(refWay, maxDistance, addError));
}

vector<long> ElementIdsVisitor::findElementsByTag(const ConstOsmMapPtr& map,
                                                  const ElementType& elementType,
                                                  const QString& key, const QString& value)
{
  TagCriterion crit(key, value);
  ElementIdsVisitor v(elementType, &crit);
  if (elementType == ElementType::Node)
  {
    map->visitNodesRo(v);
  }
  else
  {
    map->visitWaysRo(v);
  }
  return v.getIds();
}

// Convenience method for finding ways that contain the given node
vector<long> ElementIdsVisitor::findWaysByNode(const ConstOsmMapPtr& map, long nodeId)
{
  ContainsNodeCriterion crit(nodeId);
  ElementIdsVisitor v(ElementType::Way, &crit);
  map->visitWaysRo(v);
  return v.getIds();
}

}

