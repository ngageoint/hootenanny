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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RDP_WAY_GENERALIZER_H
#define RDP_WAY_GENERALIZER_H

// Hoot
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

// Qt
#include <QString>

namespace hoot
{

class Node;
class Way;

/**
 * Uses the Ramer-Douglas Peucker algorithm [1] to generalize a set of point.  This
 * implementation modeled after [2].  A description of the algorithm from [2]:
 *
 * "The Ramer-Douglas Peucker algorithm is an algorithm for reducing the number of points in a curve
 * that is approximated by a series of points. It does so by "thinking" of a line between the first
 * and last point in a set of points that form the curve. It checks which point in between is
 * farthest away from this line.  If the point (and as follows, all other in-between points) is
 * closer than a given distance 'epsilon', it removes all these in-between points. If on the other
 * hand this 'outlier point' is farther away from our imaginary line than epsilon, the curve is
 * split in two parts:
 *
 * 1) From the first point up to and including the outlier
 * 2) The outlier and the remaining points
 *
 * The function is recursively called on both resulting curves, and the two reduced forms of
 * the curve are put back together."
 *
 * 1. http://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93Peucker_algorithm
 * 2. http://karthaus.nl/rdp/js/rdp.js
 *
 *  The input map data should be projected to an orthographic coordinate system.
 *
 * NOTE: There is a GEOS implementation of this algorithm (DouglasPeuckerLineSimplifier).  Using it
 * proved not feasible, since it takes in a collection of Coordinate objects.  To use it, new nodes
 * would have to have been generated for the points kept in the reduction, while deleting the old
 * ones.  That isn't desirable from a runtime performance standpoint.
 */
class RdpWayGeneralizer : public OsmMapConsumer
{

public:

  RdpWayGeneralizer(double epsilon);

  /**
    Generalizes a way to a set of reduced points.  The map the way belongs to is modified.

    @param way the way whose points are to be reduced
    @return the number of nodes removed
    */
  int generalize(const std::shared_ptr<Way>& way);

  /**
    Sets the distance parameter that determines to what degree the way is generalized; higher
    values result in more generalization (more nodes are removed)
    */
  void setEpsilon(double epsilon);

  void setRemoveNodesSharedByWays(bool remove) { _removeNodesSharedByWays = remove; }

  virtual void setOsmMap(OsmMap* map) { _map = map->shared_from_this(); }

private:

  friend class RdpWayGeneralizerTest;

  // distance in meters that determines to what degree a way is generalized; higher values result in
  // more generalization (more nodes removed).
  double _epsilon;

  // If true, the any node shared by ways will not be removed by generalization. Its arguable that
  // this shouldn't be an option and shared nodes should never be removed, but leaving it optional
  // for now in case there is a use case where they should be removed.
  bool _removeNodesSharedByWays;

  OsmMapPtr _map;

  /*
    Generates a set of points that make up a generalized set of the input points

    @param wayPoints the collection of points to be reduced
    @returns a reduced set of line points
    */
  virtual QList<std::shared_ptr<const Node>> _getGeneralizedPoints(
    const QList<std::shared_ptr<const Node>>& wayPoints);

  /*
    Finds the perpendicular distance between an imaginary line drawn from the first point on a line
    to the last point on a line and a split point

    @param splitPoint a location on the line to be reduced splitting it into two parts
    @param lineToBeReducedStartPoint beginning point on the line to be reduced; also the starting
    point for the imaginary line drawn directly from start to end point on the line to be reduced
    @param lineToBeReducedEndPoint end point on the line to be reduced; also the ending
    point for the imaginary line drawn directly from start to end point on the line to be reduced
    */
  double _getPerpendicularDistanceBetweenSplitNodeAndImaginaryLine(
    const std::shared_ptr<const Node> splitPoint,
    const std::shared_ptr<const Node> lineToBeReducedStartPoint,
    const std::shared_ptr<const Node> lineToBeReducedEndPoint) const;

  /*
   * Takes in the way node IDs before the way was generalized and after it was generalized and
   * constructs a final set of node IDs with the IDs that aren't allowed to be removed added back
   * in (shared node IDs). This is necessary because it was too difficult to inject the logic to
   * keep shared nodes into the recursive generalization function, so we run the logic after the
   * generalization instead.
   */
  QList<long> _getUpdatedWayNodeIdsForThoseNotAllowedToBeRemoved(
    const QSet<long>& nodeIdsNotAllowedToBeRemoved, const QList<long>& nodeIdsBeforeGeneralization,
    const QList<long>& generalizedNodeIds);
};

}

#endif // RDP_WAY_GENERALIZER_H
