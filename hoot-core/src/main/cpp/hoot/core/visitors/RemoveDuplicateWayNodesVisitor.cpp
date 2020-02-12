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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveDuplicateWayNodesVisitor.h"

// Hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>

// Qt
#include <QVector>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveDuplicateWayNodesVisitor)

RemoveDuplicateWayNodesVisitor::RemoveDuplicateWayNodesVisitor()
{
}

void RemoveDuplicateWayNodesVisitor::removeDuplicates(const WayPtr& way)
{
  RemoveDuplicateWayNodesVisitor vis;
  vis.visit(way);
}

void RemoveDuplicateWayNodesVisitor::visit(const ElementPtr& e)
{
  if (e->getElementType() == ElementType::Way)
  {
    LOG_TRACE("Looking for duplicate way nodes...");

    WayPtr way = std::dynamic_pointer_cast<Way>(e);
    assert(way.get());
    const std::vector<long>& wayNodeIds = way->getNodeIds();
    LOG_VART(wayNodeIds);

    QVector<long> parsedNodeIds;
    QVector<long> duplicateWayNodeIds;
    bool foundDuplicateWayNode = false;
    for (size_t i = 0; i < wayNodeIds.size(); i++)
    {
      const long nodeId = wayNodeIds[i];
      LOG_VART(nodeId);
      if (!parsedNodeIds.contains(nodeId))
      {
        // we haven't seen this node ID yet
        LOG_TRACE("Valid way node: " << nodeId);
        parsedNodeIds.append(nodeId);
      }
      else if (i == wayNodeIds.size() -1 && nodeId == parsedNodeIds.at(0))
      {
        // this node ID is duplicated, but its the end node duplicating the start node, which is ok
        // (closed way)
        LOG_TRACE("End node matches start node: " << nodeId);
        parsedNodeIds.append(nodeId);
      }
      else
      {
        // we've found a duplicate where both nodes aren't start and end nodes...not allowed
        LOG_TRACE("Found duplicate way node: " << nodeId);
        duplicateWayNodeIds.append(nodeId);
        foundDuplicateWayNode = true;
        _numAffected++;
      }
    }
    if (foundDuplicateWayNode)
    {
      LOG_TRACE("Removing duplicate way nodes: " << duplicateWayNodeIds);
      LOG_VART(parsedNodeIds);
      way->setNodes(parsedNodeIds.toStdVector());
    }
  }
}

QStringList RemoveDuplicateWayNodesVisitor::getCriteria() const
{
  QStringList criteria;
  criteria.append(QString::fromStdString(LinearCriterion::className()));
  criteria.append(QString::fromStdString(PolygonCriterion::className()));
  return criteria;
}

}
