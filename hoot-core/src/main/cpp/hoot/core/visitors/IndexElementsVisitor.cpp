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
#include "IndexElementsVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/ElementCriterion.h>

// TGS
#include <tgs/RStarTree/IntersectionIterator.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

IndexElementsVisitor::IndexElementsVisitor(
  boost::shared_ptr<HilbertRTree>& index, deque<ElementId>& indexToEid,
  const boost::shared_ptr<ElementCriterion>& criterion,
  boost::function<Meters (const ConstElementPtr& e)> getSearchRadius, ConstOsmMapPtr pMap) :
_criterion(criterion),
_getSearchRadius(getSearchRadius),
_index(index),
_indexToEid(indexToEid)
{
  _map = pMap.get();
}

void IndexElementsVisitor::addCriterion(const ElementCriterionPtr& e)
{
  assert(_criterion.get() == 0);
  _criterion = e;
}

void IndexElementsVisitor::finalizeIndex()
{
  LOG_DEBUG("Finalizing index...");
  _index->bulkInsert(_boxes, _fids);
}

void IndexElementsVisitor::visit(const ConstElementPtr& e)
{
  if (!_criterion || _criterion->isSatisfied(e))
  {
    _fids.push_back((int)_indexToEid.size());
    _indexToEid.push_back(e->getElementId());

    Box b(2);
    Meters searchRadius = _getSearchRadius(e);
    boost::shared_ptr<Envelope> env(e->getEnvelope(_map->shared_from_this()));
    env->expandBy(searchRadius);
    b.setBounds(0, env->getMinX(), env->getMaxX());
    b.setBounds(1, env->getMinY(), env->getMaxY());

    _boxes.push_back(b);

    _numAffected++;
  }
}

set<ElementId> IndexElementsVisitor::findNeighbors(const Envelope& env,
                                                   const boost::shared_ptr<Tgs::HilbertRTree>& index,
                                                   const deque<ElementId>& indexToEid,
                                                   ConstOsmMapPtr pMap)
{
  LOG_TRACE("Finding neighbors within env: " << env << "...");
  LOG_VART(indexToEid.size());
  LOG_VART(index.get());

  const ElementToRelationMap& e2r = *(pMap->getIndex()).getElementToRelationMap();
  LOG_VART(e2r.size());

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

  LOG_VART(result.size());
  return result;
}

}
