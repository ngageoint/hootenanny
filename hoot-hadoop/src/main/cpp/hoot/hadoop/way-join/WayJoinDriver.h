/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef WAYJOINDRIVER_H
#define WAYJOINDRIVER_H

// Hoot
#include <hoot/core/elements/Status.h>
#include <hoot/hadoop/Driver.h>
#include <hoot/hadoop/stats/MapStats.h>

// Qt
#include <QString>

// Standard
#include <string>

namespace hoot
{
using namespace std;

class WayJoinDriver : public Driver
{
public:


  /**
   * @param maxWaySize The maximum way size in degrees.
   */
  WayJoinDriver(double maxWaySize);

  /**
   * Calculates the bounding boxes for all the ways in "in" and writes them to "out".
   *
   * @param in - Path to an input PBF OsmMap.
   * @param out - Path to where the resulting PBF OsmMap should be written.
   *
   * Internally this calls joinNodesToWays and then joinPointsToWays.
   */
  void calculateWayBounds(QString in, QString out);

  /**
   * Runs the WayJoin1 MapReduce job. In most cases you'll want to call calculateWayBounds().
   *
   * This job will join a ways ID to a nodes ID & x/y.
   *
   * The input is a PBF complete OSM map.
   *
   * The output is a custom sequence format that contains:
   *   * key - way ID
   *   * value - node ID, x & y
   */
  void joinWaysToNodes(QString in, QString out);

  /**
   * Runs the WayJoin2 MapReduce job. In most cases you'll want to call calculateWayBounds().
   *
   * This job will join the output of joinWaysToNodes() up to the ways in a complete OSM map.
   *
   * The input is a comma delimited list of .csq files and .pbf files. The .pbf file
   * will be the same as the input provided to joinWaysToNodes(). The .csq files are the output
   * of joinWaysToNodes().
   *
   * The output is a complete OsmMap that contains the same elements as the input .pbf, but the
   * bbox values will be populated for the ways. While the bbox values are populated in the
   * Way::getApproximateEnvelope() field. The bbox values are guaranteed to be exact within rounding
   * error.
   */
  void joinPointsToWays(QString in, QString out);

  /**
   * Set the maximum way size in degrees.
   */
  void setMaximumWaySize(double size) { _maxWaySize = size; }

  /**
   * Set the new node ids
   */
  void setRenumbering(long nodeIdDelta, long wayIdDelta, long relationIdDelta, Status newStatus);

private:
  /// max size in degrees.
  double _maxWaySize;

  long _nodeIdDelta;
  long _relationIdDelta;
  long _wayIdDelta;
  Status _newStatus;

  MapStats _stats;

};

}

#endif // WAYJOINDRIVER_H
