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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "WayNodeCopier.h"

// hoot
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/elements/ElementComparer.h>

namespace hoot
{

WayNodeCopier::WayNodeCopier()
{
}

void WayNodeCopier::addCriterion(const ElementCriterionPtr& e)
{
  _crit = e;
}

void WayNodeCopier::copy(const ElementId& toReplaceWayId, const ElementId& replacingWayId)
{
  if (!_map)
  {
    throw IllegalArgumentException("No map set on WayNodeCopier.");
  }

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
    LOG_VART(replacingWay->getElementId());
    LOG_VART(replacingWay->getNodeCount());

    for (size_t i = 0; i < toReplaceWay->getNodeCount(); i++)
    {
      ConstNodePtr nodeToBeRemoved = _map->getNode(toReplaceWay->getNodeId(i));
      LOG_VART(nodeToBeRemoved->getElementId());

      // for each informational node in the way being replaced
      if (!_crit || _crit->isSatisfied(nodeToBeRemoved))
      {
        const WayLocation closestBeingReplacedLocToInfoNode =
          LocationOfPoint::locate(_map, toReplaceWay, nodeToBeRemoved->toCoordinate());
        LOG_VART(closestBeingReplacedLocToInfoNode);
        // if there is no node at the same location along the way,
        if (closestBeingReplacedLocToInfoNode.isValid())
        {
          // add the node at that location on the replacement way
          const WayLocation closestReplacementLocToInfoNode =
            LocationOfPoint::locate(_map, replacingWay, nodeToBeRemoved->toCoordinate());
          LOG_VART(closestReplacementLocToInfoNode);
          const double tolerance = 0.01;
          LOG_VART(closestReplacementLocToInfoNode.isNode(tolerance));
          if (!closestReplacementLocToInfoNode.isNode(tolerance))
          {
            const long index =
              OsmUtils::closestWayNodeIndexToNode(nodeToBeRemoved, replacingWay, _map, false);
            LOG_VART(index);
            if (index != -1)
            {
              const geos::geom::Coordinate closestCoord =
                closestReplacementLocToInfoNode.getCoordinate();
              LOG_VART(closestCoord);
              NodePtr nodeToAdd = nodeToBeRemoved->cloneSp();
              // by cloning the node being removed, we'll keep its id in the new node (?)
              nodeToAdd->setX(closestCoord.x);
              nodeToAdd->setY(closestCoord.y);
              _map->addNode(nodeToAdd);
              LOG_TRACE(
                "Inserting " << nodeToAdd->getElementId() << " in " <<
                replacingWay->getElementId() << " at index: " << index << "...");
              replacingWay->insertNode(index, nodeToAdd->getId());
              LOG_VART(replacingWay->getNodeCount());
            }
          }
          // If there is a node at the same location along the way, and...
          else
          {
            // the two nodes have identical tags, skip adding anything
            ConstNodePtr closestReplacementNode =
              closestReplacementLocToInfoNode.getNode(tolerance);
            LOG_VART(closestReplacementNode->getElementId());
            LOG_VART(ElementComparer::tagsAreSame(nodeToBeRemoved, closestReplacementNode));
            if (ElementComparer::tagsAreSame(nodeToBeRemoved, closestReplacementNode))
            {
              LOG_TRACE(
                "Nodes " << nodeToBeRemoved->getElementId() << " and " <<
                closestReplacementNode->getElementId() << " are identical, so skipping " <<
                "insertion of node to " << replacingWay->getElementId());
              continue;
            }
            else
            {
              // the two nodes do not have identical tags, then merge the tags in from the way being
              // replaced
              // not sure how to get around this cast..
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
            }
          }
        }
      }
    }
  }
}

}
