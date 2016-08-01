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
#include "IndexElementsVisitor.h"

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/ConfigOptions.h>

// TGS
#include <tgs/RStarTree/IntersectionIterator.h>

namespace hoot
{

IndexElementsVisitor::IndexElementsVisitor(shared_ptr<HilbertRTree>& index,
                                           deque<ElementId>& indexToEid,
                                           const shared_ptr<ElementCriterion>& filter,
                                           boost::function<Meters (const ConstElementPtr& e)> getSearchRadius,
                                           ConstOsmMapPtr pMap):
  _pMap(pMap),
  _filter(filter),
  _getSearchRadius(getSearchRadius),
  _index(index),
  _indexToEid(indexToEid)
{
  // This space intentionally left blank
}

void IndexElementsVisitor::visit(const ConstElementPtr& e)
{
  if (!_filter || _filter->isSatisfied(e))
  {
    _fids.push_back((int)_indexToEid.size());
    _indexToEid.push_back(e->getElementId());

    Box b(2);
    Meters searchRadius = _getSearchRadius(e);
    auto_ptr<Envelope> env(e->getEnvelope(_pMap));
    env->expandBy(searchRadius);
    b.setBounds(0, env->getMinX(), env->getMaxX());
    b.setBounds(1, env->getMinY(), env->getMaxY());

    _boxes.push_back(b);
  }
}

set<ElementId> IndexElementsVisitor::findNeighbors(const Envelope& env,
                                                   const shared_ptr<Tgs::HilbertRTree>& index,
                                                   const deque<ElementId>& indexToEid,
                                                   ConstOsmMapPtr pMap)
{
  const ElementToRelationMap& e2r = *(pMap->getIndex()).getElementToRelationMap();

  set<ElementId> result;
  vector<double> min(2), max(2);
  min[0] = env.getMinX();
  min[1] = env.getMinY();
  max[0] = env.getMaxX();
  max[1] = env.getMaxY();
  IntersectionIterator it(index.get(), min, max);

  while (it.next())
  {
    ElementId eid = indexToEid[it.getId()];

    // Map the tree id to an element id and push into result.
    result.insert(eid);

    // Check for relations that contain this element
    const set<long>& relations = e2r.getRelationByElement(eid);
    for (set<long>::const_iterator it = relations.begin(); it != relations.end(); ++it)
    {
      result.insert(ElementId(ElementType::Relation, *it));
    }
  }

  return result;
}

} // End namespace hoot
