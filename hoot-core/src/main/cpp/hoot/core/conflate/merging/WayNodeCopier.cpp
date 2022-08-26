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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "WayNodeCopier.h"

// hoot
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/schema/TagMergerFactory.h>

namespace hoot
{

WayNodeCopier::WayNodeCopier()
  : _duplicateNodeTolerance(0.05)
{
  setConfiguration(conf());
}

void WayNodeCopier::addCriterion(const ElementCriterionPtr& e)
{
  _crit = e;
}

void WayNodeCopier::setConfiguration(const Settings& conf)
{
  _duplicateNodeTolerance = ConfigOptions(conf).getWayNodeCopierDuplicateNodeTolerance();
}

void WayNodeCopier::copy(const ElementId& toReplaceWayId, const ElementId& replacingWayId) const
{
  if (!_map)
    throw IllegalArgumentException("No map set on WayNodeCopier.");
  if (toReplaceWayId.getType() != ElementType::Way || replacingWayId.getType() != ElementType::Way)
  {
    throw IllegalArgumentException(
          "WayNodeCopier only processes ways. Input IDs: " + toReplaceWayId.toString() + ", " + replacingWayId.toString());
  }

  bool elementsModified = false;

  LOG_VART(toReplaceWayId);
  LOG_VART(replacingWayId);
  ConstWayPtr toReplaceWay = _map->getWay(toReplaceWayId);
  WayPtr replacingWay = _map->getWay(replacingWayId);
  LOG_VART(toReplaceWay.get());
  LOG_VART(replacingWay.get());
  if (toReplaceWay && replacingWay)
  {
    LOG_VART(toReplaceWay->getElementId());
    LOG_VART(toReplaceWay->getNodeCount());
    LOG_VART(toReplaceWay->getNodeIds());
    LOG_VART(replacingWay->getElementId());
    LOG_VART(replacingWay->getNodeCount());
    LOG_VART(replacingWay->getNodeIds());

    for (auto node_id : toReplaceWay->getNodeIds())
    {
      ConstNodePtr nodeToBeRemoved = _map->getNode(node_id);
      if (!nodeToBeRemoved)
        continue;
      LOG_VART(nodeToBeRemoved->getElementId());

      // for each node satisfying the specifying criteria (if there is one)
      if (!_crit || _crit->isSatisfied(nodeToBeRemoved))
      {
        // get the location of the node top copy on the way being replaced
        const WayLocation closestBeingReplacedLocToInfoNode = LocationOfPoint::locate(_map, toReplaceWay, nodeToBeRemoved->toCoordinate());
        LOG_VART(closestBeingReplacedLocToInfoNode);
        if (closestBeingReplacedLocToInfoNode.isValid())
        {
          // get the same location of the node to be copied on the way we're copying the node to
          const WayLocation closestReplacementLocToInfoNode = LocationOfPoint::locate(_map, replacingWay, nodeToBeRemoved->toCoordinate());
          LOG_VART(closestReplacementLocToInfoNode);
          LOG_VART(closestReplacementLocToInfoNode.isNode(_duplicateNodeTolerance));

          // if there is no node at the same location in the way we're copying to,
          if (!closestReplacementLocToInfoNode.isNode(_duplicateNodeTolerance))
          {
            // find the closest way node index at the location of the node being copied to the way
            // we're copying to
            const long index = WayUtils::closestWayNodeInsertIndex(nodeToBeRemoved, replacingWay, _map);
            LOG_VART(index);
            if (index != -1)
            {
              // copy the node from the way being replaced
              const geos::geom::Coordinate closestCoord = closestReplacementLocToInfoNode.getCoordinate();
              LOG_VART(closestCoord);
              NodePtr nodeToAdd = nodeToBeRemoved->cloneSp();
              // by cloning the node being removed, we'll keep its id in the new node (?)
              nodeToAdd->setX(closestCoord.x);
              nodeToAdd->setY(closestCoord.y);
              _map->addNode(nodeToAdd);

              // add the node at the calculated position on the replacement way

              LOG_TRACE(
                "Inserting " << nodeToAdd->getElementId() << " in " <<
                replacingWay->getElementId() << " at index: " << index << "...");
              replacingWay->insertNode(index, nodeToAdd->getId());
              LOG_VART(replacingWay->getNodeCount());
              elementsModified = true;
            }
          }
          // if there is a node at the same location along the way, and...
          else
          {
            ConstNodePtr closestReplacementNode = closestReplacementLocToInfoNode.getNode(_duplicateNodeTolerance);
            LOG_VART(closestReplacementNode->getElementId());
            LOG_VART(nodeToBeRemoved->getTags().dataOnlyEqual(closestReplacementNode->getTags()));

            if (nodeToBeRemoved->hasSameNonMetadataTags(*closestReplacementNode))
            {
              // the two nodes have identical tags, don't make any changes
              LOG_TRACE(
                "Nodes " << nodeToBeRemoved->getElementId() << " and " <<
                closestReplacementNode->getElementId() << " are identical, so skipping " <<
                "insertion of node to " << replacingWay->getElementId());
              continue;
            }
            else
            {
              // the two nodes do not have identical tags, then merge the tags in from the way node
              // being replaced

              // not sure how to get around this const cast...
              NodePtr closestReplacementNode2 =
                std::const_pointer_cast<Node>(closestReplacementNode);
              closestReplacementNode2->setTags(
                TagMergerFactory::mergeTags(
                  closestReplacementNode->getTags(), nodeToBeRemoved->getTags(),
                  ElementType::Node));
              LOG_VART(closestReplacementNode2->getTags());
              LOG_TRACE(
                "Updating tags on " << closestReplacementNode2->getElementId() << " with tags " <<
                "from " << nodeToBeRemoved->getElementId() << " for " <<
                replacingWay->getElementId() << "...");
              elementsModified = true;
            }
          }

          if (elementsModified)
          {
            LOG_VART(replacingWay->getNodeIds());
            // Turn on for debugging only; could be very expensive
            //OsmMapWriterFactory::writeDebugMap(
              //_map, "WayNodeCopier-" + nodeToBeRemoved->getElementId().toString());
          }
        }
      }
    }
  } 
}

}
