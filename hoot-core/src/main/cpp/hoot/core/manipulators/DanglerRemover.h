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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DANGLERREMOVER_H
#define DANGLERREMOVER_H

// Hoot
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/manipulators/WayManipulator.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

class OsmMap;
class Node;
class Way;

/**
 * This removes short stubby roads and unusual loops that are likely a leftover from a previous
 * manipulation.
 */
class DanglerRemover : public WayManipulator
{
public:

  /**
   * @param errorPlus - Take the circular error of a road and add this value. That is the maximum
   *   length of road that will still be considered for removal.
   */
  DanglerRemover(Meters errorPlus = 0);

  virtual ~DanglerRemover() {}

  virtual const std::vector< boost::shared_ptr<Manipulation> >& findAllManipulations(
          ConstOsmMapPtr map);

  virtual const std::vector< boost::shared_ptr<Manipulation> >& findWayManipulations(
          ConstOsmMapPtr map, const std::vector<long>& wids);

private:

  std::vector< boost::shared_ptr<Manipulation> > _result;
  ConstOsmMapPtr _map;

  double _errorPlus;

  void _findMatches(long baseWayId);

};

}

#endif // DANGLERREMOVER_H
