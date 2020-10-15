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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef SUPERFLUOUSNODEREMOVER_H
#define SUPERFLUOUSNODEREMOVER_H

// GEOS
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Configurable.h>

// Standard
#include <set>

// Qt
#include <QSet>

namespace hoot
{

class OsmMap;

/**
 * Removes all the nodes from a map that aren't part of a way or a relation and have no information
 * in them.
 *
 * If the bounds have been set via Boundable's setBounds then only nodes that are both not part
 * of a way and inside the bounds will be removed. This is most useful when performing tile based
 * operations such as the FourPassDriver.
 *
 * No point in implementing FilteredByGeometryTypeCriteria here, as there is no such thing as a map
 * with no nodes.
 */
class SuperfluousNodeRemover : public OsmMapOperation, public Serializable, public Boundable,
  public Configurable
{
public:

  static std::string className() { return "hoot::SuperfluousNodeRemover"; }

  SuperfluousNodeRemover();
  virtual ~SuperfluousNodeRemover() = default;

  /**
   * @see OsmMapOperation
   */
  virtual void apply(std::shared_ptr<OsmMap>& map);

  virtual std::string getClassName() const { return className(); }

  /**
   * Removes superfluous nodes from a map
   *
   * @param map map from which to remove the nodes
   * @param ignoreInformationTags if true, will remove nodes even if they have an info tag
   * @param e bounds within which to remove nodes
   * @return the number of superfluous nodes removed
   */
  static long removeNodes(std::shared_ptr<OsmMap>& map, const bool ignoreInformationTags = false,
                          const geos::geom::Envelope& e = geos::geom::Envelope());

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
    const geos::geom::Envelope& e = geos::geom::Envelope());

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
    const geos::geom::Envelope& e = geos::geom::Envelope());

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  virtual void readObject(QDataStream& is);
  virtual void writeObject(QDataStream& os) const;

  virtual QString getDescription() const { return "Removes all nodes not part of a way"; }

  virtual QString getInitStatusMessage() const { return "Removing superfluous nodes..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + StringUtils::formatLargeNumber(_numAffected) + " superfluous nodes"; }

  std::set<long> getSuperfluousNodeIds() const { return _superfluousNodeIds; }

  virtual void setBounds(const geos::geom::Envelope& bounds);
  void setIgnoreInformationTags(bool ignore) { _ignoreInformationTags = ignore; }
  void setRemoveNodes(bool remove) { _removeNodes = remove; }

protected:

  geos::geom::Envelope _bounds;

  // turning this off is useful for debugging the existence of orphaned nodes
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

  int _taskStatusUpdateInterval;
};

}

#endif // SUPERFLUOUSNODEREMOVER_H
