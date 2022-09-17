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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "OffsetIntersectionMergerOp.h"

// Hoot
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/WayLengthCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, OffsetIntersectionMergerOp)

OffsetIntersectionMergerOp::OffsetIntersectionMergerOp()
  : _offsetMax(ConfigOptions().getOffsetIntersectionMax())
{
}

void OffsetIntersectionMergerOp::mergeOffsetIntersections(std::shared_ptr<OsmMap> map)
{
  OffsetIntersectionMergerOp op;
  op.apply(map);
}

void OffsetIntersectionMergerOp::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  //  Project to planar for calculating the way length
  MapProjector::projectToPlanar(map);
  //  Linear ways less than or equal to offset max
  ChainCriterion criterion(std::make_shared<LinearCriterion>(),
                           std::make_shared<WayLengthCriterion>(_offsetMax, NumericComparisonType::LessThanOrEqualTo, map));
  const WayMap& ways = map->getWays();
  std::shared_ptr<NodeToWayMap> nodeToWays = map->getIndex().getNodeToWayMap();
  std::set<long> intersectionWays;
  //  Iterate all of the ways looking for ways shorter or equal in length to the threshold
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr way = it->second;
    _numProcessed++;
    //  Small ways only containing two nodes
    if (way->getNodeCount() != 2)
      continue;
    long id1 = way->getFirstNodeId();
    long id2 = way->getLastNodeId();
    const std::set<long>& nodes1 = nodeToWays->getWaysByNode(id1);
    const std::set<long>& nodes2 = nodeToWays->getWaysByNode(id2);
    if (nodes1.size() != 3 || nodes2.size() != 3)
      continue;
    //  Check the criteria
    if (criterion.isSatisfied(way))
      intersectionWays.insert(way->getId());
  }

  //  Iterate all of the intersection ways found to merge the intersection
  for (auto way_id : intersectionWays)
  {
    WayPtr way = map->getWay(way_id);
    long id1 = way->getFirstNodeId();
    long id2 = way->getLastNodeId();
    //  Remove the way recursively
    RecursiveElementRemover(way->getElementId()).apply(map);
    //  Join the two nodes by replacing one with the other and deleting the replaced one
    long replace = id1;
    long remove = id2;
    if ((id1 < 0 && id2 < 0 && id1 < id2) ||
        (id1 > 0 && id2 > 0 && id1 > id2) ||
        (id1 < 0 && id2 > 0))
    {
      replace = id2;
      remove = id1;
    }
    //  Replace all instances of "remove" with "replace" and then remove "remove"
    ReplaceElementOp(ElementId::node(remove), ElementId::node(replace), true).apply(map);
    //  Increment the number affected
    _numAffected++;
  }
}

}
