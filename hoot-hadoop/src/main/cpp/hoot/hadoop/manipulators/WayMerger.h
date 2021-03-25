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
 * @copyright Copyright (C) 2015, 2017, 2018 Maxar (http://www.maxar.com/)
 */

#ifndef WAYMERGER_H
#define WAYMERGER_H

// Hoot
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/manipulators/WayManipulator.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

class OsmMap;
class Way;

/**
 * This class merges roads that are similar to roads in the companion data set. The result of the
 * merge is a new conflated highway.
 */
class WayMerger : public WayManipulator
{
public:

  static std::string className() { return "hoot::WayMerger"; }

  WayMerger();
  virtual ~WayMerger() {}

  virtual const std::vector< boost::shared_ptr<Manipulation> >& findAllManipulations(
          ConstOsmMapPtr map);

  virtual const std::vector< boost::shared_ptr<Manipulation> >& findWayManipulations(
          ConstOsmMapPtr map, const std::vector<long>& wids);

protected:

  /**
   * Creates a new manipulation and returns it. This may be delegated to a subclass.
   */
  virtual Manipulation* _createManipulation(long leftId, long rightId,
    ConstOsmMapPtr map, Meters minSplitSize);

private:

  std::vector< boost::shared_ptr<Manipulation> > _result;
  ConstOsmMapPtr _map;

  Meters _minSplitSize;

  /**
   * Find all parallel ways that are potential matches for the given way.
   */
  std::vector<long> _findOtherWays(ConstWayPtr way);

  void _findMatches(long baseWayId);
};

}

#endif // WAYMERGER_H
