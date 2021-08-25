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

#ifndef SUPERFLUOUSNODEREMOVER_H
#define SUPERFLUOUSNODEREMOVER_H

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>

namespace hoot
{

class OsmMap;

/**
 * Removes all the nodes from a map that aren't part of a way or a relation and have no information
 * in them.
 *
 * If the bounds have been set via Boundable's setBounds then only nodes that are both not part
 * of a way and inside the bounds will be removed. This is most useful when performing tile based
 * operations such as the FourPassDriver (Hadoop).
 *
 * No point in implementing FilteredByGeometryTypeCriteria here, as there is no such thing as a map
 * with no nodes. ElementConflatableCheck does need to be implemented here to handle the case when
 * _ignoreInformationTags = true.
 *
 * @todo Do we really need to implement ConflateInfoCacheConsumer here? A superfluous node should
 * always be removed, right?
 */
class SuperfluousNodeRemover : public OsmMapOperation, public Boundable, public Configurable,
  public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "SuperfluousNodeRemover"; }

  SuperfluousNodeRemover();
  ~SuperfluousNodeRemover() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * Removes superfluous nodes from a map
   *
   * @param map map from which to remove the nodes
   * @param ignoreInformationTags if true, will remove nodes even if they have an info tag
   * @param e bounds within which to remove nodes
   * @return the number of superfluous nodes removed
   */
  static long removeNodes(
    std::shared_ptr<OsmMap>& map, const bool ignoreInformationTags = false,
    const std::shared_ptr<geos::geom::Geometry>& bounds = std::shared_ptr<geos::geom::Geometry>());

  /**
   * Counts superfluous nodes in a map without removing them
   *
   * @param map map from which to count the nodes
   * @param ignoreInformationTags if true, will count nodes even if they have an info tag
   * @param e bounds within which to count nodes
   * @return the number of superfluous nodes found
   */
  static long countSuperfluousNodes(
    std::shared_ptr<OsmMap>& map, const bool ignoreInformationTags = false,
    const std::shared_ptr<geos::geom::Geometry>& bounds = std::shared_ptr<geos::geom::Geometry>());

  /**
   * Collects superfluous nodes from a map without removing them
   *
   * @param map map from which to collect the node IDs
   * @param ignoreInformationTags if true, will collect IDs of nodes even if they have an info tag
   * @param e bounds within which to collect node IDs
   * @return the superfluous node IDs found
   */
  static std::set<long> collectSuperfluousNodeIds(
    std::shared_ptr<OsmMap>& map, const bool ignoreInformationTags = false,
    const std::shared_ptr<geos::geom::Geometry>& bounds = std::shared_ptr<geos::geom::Geometry>());

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes all nodes not part of a way"; }

  QString getInitStatusMessage() const override { return "Removing superfluous nodes..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + StringUtils::formatLargeNumber(_numAffected) + " superfluous nodes"; }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

  std::set<long> getSuperfluousNodeIds() const { return _superfluousNodeIds; }

  void setIgnoreInformationTags(bool ignore) { _ignoreInformationTags = ignore; }
  void setRemoveNodes(bool remove) { _removeNodes = remove; }

private:

  // Turning this off is useful for debugging the existence of orphaned nodes.
  bool _removeNodes;
  // nodes with these IDs will never be removed
  QSet<long> _excludeIds;
  // the number of nodes that explicitly weren't removed due to configuration
  int _numExplicitlyExcluded;

  // the non-superfluous nodes
  std::set<long> _usedNodeIds;
  // nodes either removed or counted; useful for debugging
  std::set<long> _superfluousNodeIds;

  // if true, a node can be removed even if it has information tags (non-metadata)
  bool _ignoreInformationTags;
  // configurable set of tags where if found on a node, we always want to remove it
  QStringList _unallowedOrphanKvps;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;

  int _taskStatusUpdateInterval;
};

}

#endif // SUPERFLUOUSNODEREMOVER_H
