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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DIVIDEDHIGHWAYMERGER_H
#define DIVIDEDHIGHWAYMERGER_H

// Hoot
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/manipulators/Manipulator.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

class OsmMap;
class Node;
class Way;

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

  virtual const std::vector< boost::shared_ptr<Manipulation> >& findAllManipulations(
          ConstOsmMapPtr map);

  virtual const std::vector< boost::shared_ptr<Manipulation> >& findWayManipulations(
          ConstOsmMapPtr map, const std::vector<long>& wids);

private:

  std::vector< boost::shared_ptr<Manipulation> > _result;
  ConstOsmMapPtr _map;

  Meters _minSeparation, _maxSeparation, _vectorError;
  ChainCriterion _oneWayUnknownCriterion;
  double _matchPercent;

  std::vector<long> _findCenterWays(ConstWayPtr w1, ConstWayPtr w2);

  /**
   * Find all parallel ways that go in the opposite direction and are within the appropriate
   * threshold.
   */
  std::vector<long> _findOtherWays(boost::shared_ptr<const hoot::Way> way);

  void _findMatches(long baseWayId);

};

}

#endif // DIVIDEDHIGHWAYMERGER_H
