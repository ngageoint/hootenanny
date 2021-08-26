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

#include "NodeUtils.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

QList<long> NodeUtils::nodesToNodeIds(const QList<std::shared_ptr<const Node>>& nodes)
{
  QList<long> nodeIds;
  for (QList<std::shared_ptr<const Node>>::const_iterator it = nodes.constBegin();
       it != nodes.constEnd(); ++it)
  {
    std::shared_ptr<const Node> node = *it;
    nodeIds.append(node->getElementId().getId());
  }
  return nodeIds;
}

std::vector<long> NodeUtils::nodesToNodeIds(const std::vector<std::shared_ptr<const Node>>& nodes)
{
  std::vector<long> nodeIds;
  for (std::vector<std::shared_ptr<const Node>>::const_iterator it = nodes.begin();
       it != nodes.end(); ++it)
  {
    std::shared_ptr<const Node> node = *it;
    if (node)
    {
      nodeIds.push_back(node->getElementId().getId());
    }
  }
  return nodeIds;
}

QList<std::shared_ptr<const Node>> NodeUtils::nodeIdsToNodes(
  const QList<long>& nodeIds, const std::shared_ptr<const OsmMap>& map)
{
  QList<std::shared_ptr<const Node>> nodes;
  for (QList<long>::const_iterator it = nodeIds.constBegin(); it != nodeIds.constEnd(); ++it)
  {
    nodes.append(std::dynamic_pointer_cast<const Node>(map->getElement(ElementType::Node, *it)));
  }
  return nodes;
}

std::vector<std::shared_ptr<const Node>> NodeUtils::nodeIdsToNodes(
  const std::vector<long>& nodeIds, const std::shared_ptr<const OsmMap>& map)
{
  std::vector<std::shared_ptr<const Node>> nodes;
  for (std::vector<long>::const_iterator it = nodeIds.begin(); it != nodeIds.end(); ++it)
  {
    nodes.push_back(std::dynamic_pointer_cast<const Node>(map->getElement(ElementType::Node, *it)));
  }
  return nodes;
}

bool NodeUtils::nodeCoordsMatch(std::vector<std::shared_ptr<const Node>> nodes1,
                                std::vector<std::shared_ptr<const Node>> nodes2)
{
  if (nodes1.size() != nodes2.size())
  {
    return false;
  }

  for (size_t i = 0; i < nodes1.size(); i++)
  {
    ConstNodePtr node1 = nodes1[i];
    ConstNodePtr node2 = nodes2[i];

    if (!node1 || !node2)
    {
      return false;
    }

    if (!node1->coordsMatch(*node2))
    {
      return false;
    }
  }

  return true;
}

QString NodeUtils::nodeCoordsToString(const std::vector<ConstNodePtr>& nodes)
{
  QString str;
  const int comparisonSensitivity = ConfigOptions().getNodeComparisonCoordinateSensitivity();
  for (size_t i = 0; i < nodes.size(); i++)
  {
    ConstNodePtr node = nodes[i];
    if (node)
    {
      str +=
        "ID: " + QString::number(node->getId()) + ", X: " +
        QString::number(node->getX(), 'f', comparisonSensitivity) + ", Y: " +
        QString::number(node->getY(), 'f', comparisonSensitivity) + "; ";
    }
    else
    {
      str += "null coord; ";
    }
  }
  str.chop(2);
  return str;
}

geos::geom::Coordinate NodeUtils::nodeToCoord(const std::shared_ptr<const Node>& node)
{
  return geos::geom::Coordinate(node->getX(), node->getY());
}

}
