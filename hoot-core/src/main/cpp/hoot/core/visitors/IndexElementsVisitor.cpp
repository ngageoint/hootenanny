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
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/criterion/ChainCriterion.h>

// TGS
#include <tgs/RStarTree/IntersectionIterator.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

IndexElementsVisitor::IndexElementsVisitor(
  std::shared_ptr<HilbertRTree>& index, deque<ElementId>& indexToEid,
  const std::shared_ptr<ElementCriterion>& criterion,
  std::function<Meters (const ConstElementPtr& e)> getSearchRadius, ConstOsmMapPtr pMap) :
_criterion(criterion),
_getSearchRadius(getSearchRadius),
_index(index),
_indexToEid(indexToEid)
{
  _map = pMap.get();

  std::shared_ptr<ChainCriterion> chainCrit = std::dynamic_pointer_cast<ChainCriterion>(_criterion);
  if (chainCrit)
  {
    LOG_VARD(chainCrit->toString());
  }
}

void IndexElementsVisitor::addCriterion(const ElementCriterionPtr& e)
{
  assert(_criterion.get() == 0);
  _criterion = e;
}

void IndexElementsVisitor::finalizeIndex()
{
  LOG_DEBUG("Finalizing index...");
  LOG_VARD(_indexToEid.size());
  LOG_VARD(_boxes.size());
  LOG_VARD(_fids.size());
  _index->bulkInsert(_boxes, _fids);
}

void IndexElementsVisitor::visit(const ConstElementPtr& e)
{
  LOG_VART(e->getElementId());
  if (!_criterion || _criterion->isSatisfied(e))
  {
    LOG_VART(e->getElementId());
    _fids.push_back((int)_indexToEid.size());
    _indexToEid.push_back(e->getElementId());

    Box b(2);
    Meters searchRadius = _getSearchRadius(e);
    std::shared_ptr<Envelope> env(e->getEnvelope(_map->shared_from_this()));
    env->expandBy(searchRadius);
    b.setBounds(0, env->getMinX(), env->getMaxX());
    b.setBounds(1, env->getMinY(), env->getMaxY());

    _boxes.push_back(b);

    _numAffected++;
  }
}

set<ElementId> IndexElementsVisitor::findNeighbors(
  const Envelope& env, const std::shared_ptr<Tgs::HilbertRTree>& index,
  const deque<ElementId>& indexToEid, ConstOsmMapPtr pMap, const ElementType& elementType,
  const bool includeContainingRelations)
{
  LOG_TRACE("Finding neighbors within env: " << env << "...");
  LOG_VART(indexToEid.size());
  LOG_VART(index.get());

  const ElementToRelationMap& e2r = *(pMap->getIndex()).getElementToRelationMap();
  LOG_VART(e2r.size());

  vector<double> min(2), max(2);
  min[0] = env.getMinX();
  min[1] = env.getMinY();
  max[0] = env.getMaxX();
  max[1] = env.getMaxY();
  IntersectionIterator it(index.get(), min, max);

  set<ElementId> neighborIds;
  while (it.next())
  {
    ElementId eid = indexToEid[it.getId()];
    LOG_VART(eid);
    if (elementType == ElementType::Unknown || eid.getType() == elementType)
    {
      // Map the tree id to an element id and push into result.
      neighborIds.insert(eid);

      if (includeContainingRelations)
      {
        // Check for relations that contain this element
        const set<long>& relations = e2r.getRelationByElement(eid);
        for (set<long>::const_iterator it = relations.begin(); it != relations.end(); ++it)
        {
          neighborIds.insert(ElementId(ElementType::Relation, *it));
        }
      }
    }
  }

  LOG_VART(neighborIds);
  LOG_VART(neighborIds.size());
  return neighborIds;
}

QList<ElementId> IndexElementsVisitor::findSortedNodeNeighbors(
  const ConstNodePtr& node, const geos::geom::Envelope& env,
  const std::shared_ptr<Tgs::HilbertRTree>& index, const std::deque<ElementId>& indexToEid,
  ConstOsmMapPtr pMap)
{
  // find the neighboring nodes
  const set<ElementId> neighborIds =
    findNeighbors(env, index, indexToEid, pMap, ElementType::Node, false);

  // map neighbors to their distance from the input node

  QMultiMap<double, ElementId> neighborNodeDistances;
  for (std::set<ElementId>::const_iterator neighborIdsItr = neighborIds.begin();
       neighborIdsItr != neighborIds.end(); ++neighborIdsItr)
  {
    ConstNodePtr neighborNode = pMap->getNode(*neighborIdsItr);
    if (!neighborNode)
    {
      // This really shouldn't happen unless the geospatial indices were set up improperly for the
      // query node. However, it does happen from time to time and haven't been able to track down
      // the cause yet, so logging as an error instead of throwing for the time being.
      const QString errorMsg =
        QString("Map does not contain neighbor node: %1. Skipping neighbor...")
          .arg((*neighborIdsItr).toString());
      //throw HootException(errorMsg);
      LOG_ERROR(errorMsg);
      continue;
    }
    else
    {
      neighborNodeDistances.insertMulti(
        Distance::euclidean(*node, *neighborNode), neighborNode->getElementId());
    }
  }
  LOG_VART(neighborNodeDistances);

  // sort neighbors by increasing distance from the input node

  const QList<double> sortedDistances = neighborNodeDistances.keys();
  LOG_VART(sortedDistances);
  QList<ElementId> sortedNeighborIds;
  for (QList<double>::const_iterator distancesItr = sortedDistances.begin();
       distancesItr != sortedDistances.end(); ++distancesItr)
  {
    const QList<ElementId> ids = neighborNodeDistances.values(*distancesItr);
    for (int i = 0; i < ids.size(); i++)
    {
      LOG_VART(ids.at(i));
      sortedNeighborIds.append(ids.at(i));
    }
  }

  LOG_VART(sortedNeighborIds);
  return sortedNeighborIds;
}

}
