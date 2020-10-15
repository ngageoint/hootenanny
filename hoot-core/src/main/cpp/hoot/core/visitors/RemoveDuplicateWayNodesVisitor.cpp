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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RemoveDuplicateWayNodesVisitor.h"

// Hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/ops/RecursiveElementRemover.h>

// Qt
#include <QVector>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveDuplicateWayNodesVisitor)

void RemoveDuplicateWayNodesVisitor::removeDuplicates(const WayPtr& way)
{
  RemoveDuplicateWayNodesVisitor vis;
  vis.visit(way);
}

void RemoveDuplicateWayNodesVisitor::visit(const ElementPtr& e)
{
  if (!e)
  {
    return;
  }

  if (e->getElementType() == ElementType::Way)
  {
    WayPtr way = std::dynamic_pointer_cast<Way>(e);
    assert(way.get());
    LOG_TRACE("Looking for duplicate way nodes in: " << way->getElementId() << "...");
    const std::vector<long>& wayNodeIds = way->getNodeIds();
    LOG_VART(wayNodeIds);

    // This is invalid. Not even sure if it would get loaded or if it is being cleaned somewhere
    // else but removing it anyway.
    if (wayNodeIds.size() == 2 && way->isSimpleLoop())
    {
      RecursiveElementRemover(way->getElementId()).apply(_map);
      return;
    }

    QVector<long> parsedNodeIds;
    QVector<long> duplicateWayNodeIds;
    long lastNodeId = 0;
    bool foundDuplicateWayNode = false;
    for (size_t i = 0; i < wayNodeIds.size(); i++)
    {
      const long nodeId = wayNodeIds[i];
      // We're only removing consecutive duplicates. Other duplicate entries could be part of a
      // valid loop feature.
      if (nodeId != lastNodeId)
      {
        LOG_TRACE("Valid way node: " << ElementId(ElementType::Node, nodeId));
        parsedNodeIds.append(nodeId);
      }
      else
      {
        // we've found a duplicate where both nodes aren't start and end nodes...not allowed
        LOG_TRACE("Found duplicate way node: " << ElementId(ElementType::Node, nodeId));
        duplicateWayNodeIds.append(nodeId);
        foundDuplicateWayNode = true;
        _numAffected++;
      }
      lastNodeId = nodeId;
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
