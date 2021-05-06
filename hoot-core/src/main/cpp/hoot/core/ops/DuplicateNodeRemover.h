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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef DUPLICATE_NODE_REMOVER_H
#define DUPLICATE_NODE_REMOVER_H

// hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>

// GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * Merges together nodes that are within a small configurable distance of each other and don't have
 * differing tags (outside of metadata tags). Due to how Way::replaceNode is being called, we could
 * end up with some duplicate way nodes, so its best to put RemoveDuplicateWayNodesVisitor in the
 * cleaning chain immediately after this runs.
 *
 * No point in implementing FilteredByGeometryTypeCriteria here, as there is no such thing as a map
 * with no nodes. ConflateInfoCacheConsumer does need to be implemented here to prevent removal of a
 * conflatable node or a node belonging to a conflatable way/relation.
 */
class DuplicateNodeRemover : public OsmMapOperation, public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "hoot::DuplicateNodeRemover"; }

  DuplicateNodeRemover(Meters distanceThreshold = -1.0);
  ~DuplicateNodeRemover() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * Removes duplicate nodes from a map
   *
   * @param map the map to remove duplicate nodes from
   * @param distanceThreshold the distance threshold under which nodes are considered duplicates
   * based on proximity
   */
  static void removeNodes(
    std::shared_ptr<OsmMap> map, Meters distanceThreshold = -1, const bool ignoreStatus = false);

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes duplicate nodes"; }

  QString getInitStatusMessage() const override { return "Removing duplicate nodes..."; }
  QString getCompletedStatusMessage() const override
  { return "Merged " + StringUtils::formatLargeNumber(_numAffected) + " node pairs."; }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

  void setIgnoreStatus(bool ignore) { _ignoreStatus = ignore; }

protected:

  std::shared_ptr<OsmMap> _map;

  // the maximum distance allowed between removed nodes
  Meters _distance;
  // allows for removing nodes with different statuses
  bool _ignoreStatus;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;

  void _logMergeResult(
    const long nodeId1, const long nodeId2, OsmMapPtr& map, const bool replaced,
    const double distance = -1.0, const double calcdDistance = -1.0);
  bool _passesLogMergeFilter(const long nodeId1, const long nodeId2, OsmMapPtr& map) const;
};

}

#endif // DUPLICATE_NODE_REMOVER_H
