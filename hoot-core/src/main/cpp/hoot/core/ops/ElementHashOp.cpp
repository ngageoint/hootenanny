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
#include "ElementHashOp.h"

// hoot
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QCryptographicHash>

namespace hoot
{

// This isn't ready for primetime yet.
//HOOT_FACTORY_REGISTER(OsmMapOperation, ElementHashOp)

ElementHashOp::ElementHashOp() :
_addParentToWayNodes(false)
{
}

void ElementHashOp::apply(const OsmMapPtr& map)
{
  _hashVis.setOsmMap(map.get());

  // Process parent element types before nodes, so that we have all the correct way information
  // available by the time we get to node processing. Note, this class doesn't take into account
  // relation membership for comparison purposes (yet?).

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;
    if (!relation)
    {
      continue;
    }

    _hashVis.visit(relation);
  }

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    if (!way)
    {
      continue;
    }

    _hashVis.visit(way);
  }

  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    NodePtr node = it->second;
    if (!node)
    {
      continue;
    }

    if (!_addParentToWayNodes)
    {
      // If parent way info isn't set to be added to way nodes, proceed with ElementHashVisitor's
      // implementation.
      _hashVis.visit(node);
    }
    else
    {
      // Otherwise, we'll dig a little deeper to find differences between way nodes.

      LOG_VART(node->getElementId());

      // Get a unique set of the most specific type for each way that contains this node. We use
      // types here, b/c way IDs won't work across maps, which is required by C&R element ID sync.
      const std::set<QString> containingWaysTypeKeys =
        WayUtils::getContainingWaysMostSpecificTypeKeysByNodeId(node->getId(), map);
      LOG_VART(containingWaysTypeKeys);
      if (!containingWaysTypeKeys.empty())
      {
        QString nodeJson = _hashVis.toJson(node);
        nodeJson.chop(1); // chop off the ending brace that's already there
        // Add in the types to a field to enable distinguishing this way nodes from others that may
        // have a nearly identical location but belong to completely different ways.
        nodeJson += ", \"parentWayTypes\":[";
        for (std::set<QString>::const_iterator itr = containingWaysTypeKeys.begin();
             itr != containingWaysTypeKeys.end(); ++itr)
        {
          const QString tagKey = *itr;
          nodeJson += tagKey + ",";
        }
        nodeJson.chop(1); // remove the trailing comma
        nodeJson += "]}"; // close up the array
        LOG_VART(nodeJson);

        QCryptographicHash hash(QCryptographicHash::Sha1);
        hash.addData(nodeJson.toLatin1().constData());
        _hashVis.insertHash(node, "sha1sum:" + QString::fromUtf8(hash.result().toHex()));
      }
      else
      {
        _hashVis.visit(node);
      }
    }
  }
}

}
