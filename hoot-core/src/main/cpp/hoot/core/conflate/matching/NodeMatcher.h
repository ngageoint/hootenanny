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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef NODEMATCHER_H
#define NODEMATCHER_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Units.h>

// Std
#include <set>
#include <vector>

// Qt
#include <QList>

namespace hoot
{

class OsmMap;
class ElementId;

class NodeMatcher
{
public:

  static QString className() { return "hoot::NodeMatcher"; }

  static int logWarnCount;

  // exposed for white box testing.
  friend class NodeMatcherTest;

  NodeMatcher();

  /**
   * Returns the degree of a node (number of ways incident to the node)
   */
  int getDegree(ElementId nid);

  /**
   * Returns a score for the confidence that two nodes represent the same intersection. 0 means
   * they're not the same. There is no upper bound, but if the score doubles it is reasonable to
   * say that we're twice as confident. The reasonable range is about 0 to 5.
   */
  double scorePair(long nid1, long nid2);

  void setMap(const std::shared_ptr<const OsmMap>& map) { _map = map; }

  /**
   * Calculates the outbound heading for an node that intersects the given wids. The nid must be
   * a start and/or end node. The angle is determined by moving delta meters along the road before
   * calculating the angle. This helps smooth out bumps if necessary.
   *
   * If the node is both a start and end node then two entries will be made in the resulting vector.
   * First the start, then the end node.
   */
  static std::vector<Radians> calculateAngles(const OsmMap* map, long nid,
                                              const std::set<long>& wids, Meters delta = 0.001);

  /**
   * Identifies network feature types (linear feature types that can have certain operations
   * performed on them)
   *
   * @param element the element to examine
   * @return true if the element is a network type; false otherwise
   */
  static bool isNetworkFeatureType(ConstElementPtr element);

  /**
   * Returns the class names of all criterion associated with a network based feature type
   *
   * Is there a better class location for this?
   *
   * @return a list of criterion class names
   */
  static QStringList getNetworkCriterionClassNames();

private:

  std::shared_ptr<const OsmMap> _map;
  double _strictness;
  double _delta;
  static QList<std::shared_ptr<ElementCriterion>> _networkFeatureTypeCriteria;

  double _calculateAngleScore(const std::vector<Radians>& theta1,
                              const std::vector<Radians>& theta2, std::vector<bool>& exclude,
                              size_t depth, bool debug = false);
};

typedef std::shared_ptr<NodeMatcher> NodeMatcherPtr;
typedef std::shared_ptr<const NodeMatcher> ConstNodeMatcherPtr;

}

#endif // NODEMATCHER_H
