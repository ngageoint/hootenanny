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

#ifndef DUPLICATE_NODE_REMOVER_H
#define DUPLICATE_NODE_REMOVER_H

// hoot
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/elements/OsmMap.h>

// GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

/**
 * Merges together nodes that are within a small configurable distance of each other and don't have
 * differing tags (outside of metadata tags).
 *
 * This class works with four pass as long as distance is less than the four pass buffer. The input
 * map can be in either a planar or geographic projection.
 */
class DuplicateNodeRemover : public OsmMapOperation, public Serializable, public Boundable
{
public:

  static std::string className() { return "hoot::DuplicateNodeRemover"; }

  DuplicateNodeRemover(Meters distanceThreshold = -1.0);

  virtual void apply(std::shared_ptr<OsmMap>& map);

  virtual std::string getClassName() const override { return className(); }

  /**
   * Removes duplicate nodes from a map
   *
   * @param map the map to remove duplicate nodes from
   * @param distanceThreshold the distance threshold under which nodes are considered duplicates
   * based on proximity
   */
  static void removeNodes(std::shared_ptr<OsmMap> map, Meters distanceThreshold = -1);

  virtual void readObject(QDataStream& is) override;

  virtual void setBounds(const geos::geom::Envelope& bounds) override { _bounds = bounds; }

  virtual void writeObject(QDataStream& os) const override;

  virtual QString getDescription() const override { return "Removes duplicate nodes"; }

  virtual QString getInitStatusMessage() const override { return "Removing duplicate nodes..."; }

  virtual QString getCompletedStatusMessage() const override
  { return "Merged " + StringUtils::formatLargeNumber(_numAffected) + " node pairs."; }

protected:

  std::shared_ptr<OsmMap> _map;
  Meters _distance;
  geos::geom::Envelope _bounds;

  void _logMergeResult(const long nodeId1, const long nodeId2, OsmMapPtr& map, const bool replaced,
                       const double distance = -1.0, const double calcdDistance = -1.0);
  bool _passesLogMergeFilter(const long nodeId1, const long nodeId2, OsmMapPtr& map) const;
};

}

#endif // DUPLICATE_NODE_REMOVER_H
