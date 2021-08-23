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

#ifndef DUALHIGHWAYSPLITTER_H
#define DUALHIGHWAYSPLITTER_H

// GEOS


// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/ops/OsmMapOperation.h>

#include <unordered_set>

namespace hoot
{

class OsmMap;
class Way;

/**
 * @brief The DualHighwaySplitter class splits all "divided=yes" highways into two one way streets.
 *
 * All intersections should be split before this method is called. Failure to do so will result in
 * undefined behavior. A number of assumptions must be made to do this including assumptions about
 * the direction of travel on roads (right or left hand drivers).
 */
class DualHighwaySplitter : public OsmMapOperation
{
public:

  static QString className() { return "hoot::DualHighwaySplitter"; }

  enum DrivingSide
  {
    Left,
    Right
  };

  DualHighwaySplitter();
  DualHighwaySplitter(const std::shared_ptr<const OsmMap>& map, DrivingSide drivingSide,
                      Meters splitSize);
  ~DualHighwaySplitter() = default;

  std::shared_ptr<OsmMap> splitAll();

  static std::shared_ptr<OsmMap> splitAll(const std::shared_ptr<const OsmMap>& map,
                                          DrivingSide drivingSide, Meters defaultSplitSize);

  void apply(std::shared_ptr<OsmMap>& map) override;

  QString getInitStatusMessage() const override
  { return "Splitting divided highways into two one way streets..."; }
  QString getCompletedStatusMessage() const override
  { return "Split " + QString::number(_numAffected) + " divided highways"; }

  /**
   * @brief getCriteria isn't actually using HighwayCriterion in the filtering, but for the purposes
   * of reducing unnecessary conflate ops we don't need to run it unless we're running road
   * conflation.
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Splits all 'divided=yes' highways into two one way streets"; }

private:

  static int logWarnCount;

  Meters _defaultSplitSize;
  DrivingSide _drivingSide;
  std::shared_ptr<const OsmMap> _map;
  std::shared_ptr<OsmMap> _result;
  std::unordered_set<long> _nodes;

  // temporary variables for convenience
  std::shared_ptr<Way> _left, _right;
  std::shared_ptr<const Way> _working;
  Meters _splitSize;

  void _addConnector(long nodeId);
  std::shared_ptr<Way> _createOneWay(const std::shared_ptr<const Way>& w, Meters bufferSize,
                                     bool left) const;
  void _createStub(const std::shared_ptr<Way>& dividedWay, long centerNodeId, long edgeNodeId);
  double _dotProduct(const geos::geom::Coordinate& c1, const geos::geom::Coordinate& c2) const;
  void _fixLanes(const std::shared_ptr<Way>& w) const;

  /**
   * Returns the node id of the nearest node to nid on w
   */
  long _nearestNode(long nid, const std::shared_ptr<const Way>& w) const;

  geos::geom::Coordinate _normalizedVector(long nid1, long nid2) const;
  bool _onRight(long intersectionId, const std::shared_ptr<Way>& inbound, long leftNn,
                long rightNn) const;
  void _reconnectEnd(long centerNodeId, const std::shared_ptr<Way>& edge) const;
  void _splitIntersectingWays(long nid) const;
  void _splitWay(long wid);
};

}

#endif // DUALHIGHWAYSPLITTER_H
