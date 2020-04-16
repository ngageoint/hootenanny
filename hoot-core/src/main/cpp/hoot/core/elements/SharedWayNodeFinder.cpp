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

#include "SharedWayNodeFinder.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

QSet<long> SharedWayNodeFinder::getSharedWayNodes(const ConstWayPtr& way1, const ConstWayPtr& way2)
{
  QSet<long> nodeIdsSet1 =
    QList<long>::fromVector(QVector<long>::fromStdVector(way1->getNodeIds())).toSet();
  const QSet<long> nodeIdsSet2 =
    QList<long>::fromVector(QVector<long>::fromStdVector(way2->getNodeIds())).toSet();
  return nodeIdsSet1.intersect(nodeIdsSet2);
}

bool SharedWayNodeFinder::waysShareNode(const ConstWayPtr& way1, const ConstWayPtr& way2)
{
  return getSharedWayNodes(way1, way2).size() > 0;
}

bool SharedWayNodeFinder::waysShareEndNode(const ConstWayPtr& way1, const ConstWayPtr& way2,
                                const bool sameDirection)
{
  LOG_VART(way1->getElementId());
  LOG_VART(way2->getElementId());
  const QSet<long> sharedNodes = getSharedWayNodes(way1, way2);
  LOG_VART(sharedNodes.size());
  for (QSet<long>::const_iterator it = sharedNodes.begin(); it != sharedNodes.end(); ++it)
  {
    const long nodeId = *it;
    if (sameDirection)
    {
      if ((way1->getFirstNodeId() == nodeId && way2->getLastNodeId()) ||
          (way2->getFirstNodeId() == nodeId && way1->getLastNodeId()))
      {
        LOG_TRACE("Found same direction shared node: " << nodeId);
        return true;
      }
    }
    else
    {
      if ((way1->getLastNodeId() == nodeId && way2->getLastNodeId()) ||
          (way2->getFirstNodeId() == nodeId && way1->getFirstNodeId()))
      {
        LOG_TRACE("Found different direction shared node: " << nodeId);
        return true;
      }
    }
  }
  return false;
}

}
