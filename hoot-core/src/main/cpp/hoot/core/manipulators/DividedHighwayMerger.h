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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DIVIDEDHIGHWAYMERGER_H
#define DIVIDEDHIGHWAYMERGER_H

#include "../Conflator.h"
#include "Manipulator.h"
#include "WayMatchSet.h"

// GEOS
namespace geos {
  namespace geom {
    class LineString;
  }
}

// Hoot
#include <hoot/core/Units.h>
#include <hoot/core/filters/WayFilterChain.h>
#include <hoot/core/filters/ChainCriterion.h>
namespace hoot {
  class OsmMap;
  class Node;
  class Way;
}


namespace hoot
{

/**
 * This class merges roads that are mapped as divided highways with roads in the companion data set
 * that are mapped as undivided highways. The result of the merge is a new divided highway.
 */
class DividedHighwayMerger : public Manipulator
{
public:

  /**
   * @param minSeparation - Minimum distance between divided highways
   * @param maxSeparation - Maximum distance between divided highways
   * @param vectorError - Maximum error in vector data.
   */
  DividedHighwayMerger(Meters minSeparation, Meters maxSeparation, Meters vectorError,
                       double matchPercent);

  virtual const vector< shared_ptr<Manipulation> >& findAllManipulations(
          shared_ptr<const OsmMap> map);

  virtual const vector< shared_ptr<Manipulation> >& findWayManipulations(
          shared_ptr<const OsmMap> map, const vector<long>& wids);

private:

  vector< shared_ptr<Manipulation> > _result;
  boost::shared_ptr<const OsmMap> _map;

  Meters _minSeparation, _maxSeparation, _vectorError;
  ChainCriterion _oneWayUnknownCriterion;
  double _matchPercent;

  vector<long> _findCenterWays(shared_ptr<const Way> w1, shared_ptr<const Way> w2);

  /**
   * Find all parallel ways that go in the opposite direction and are within the appropriate
   * threshold.
   */
  vector<long> _findOtherWays(shared_ptr<const hoot::Way> way);

  void _findMatches(long baseWayId);

};

}

#endif // DIVIDEDHIGHWAYMERGER_H
