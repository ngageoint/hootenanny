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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYMANIPULATOR_H
#define WAYMANIPULATOR_H

// hoot
#include <hoot/core/manipulators/Manipulator.h>

namespace hoot {

/**
 * Manipulates a working map in some fashion that produces a vector of new working maps. The new
 * working maps should have a reasonable chance of ultimately producing a better map than the input.
 */
class WayManipulator : public Manipulator
{
public:

  virtual ~WayManipulator() {}

  static std::string className() { return "hoot::WayManipulator"; }

  virtual const vector< boost::shared_ptr<Manipulation> >& findAllManipulations(
          boost::shared_ptr<const OsmMap> map) = 0;

  /**
   * A convenience method that maintains compatibility with the older implementations of
   * Manipulator.
   */
  virtual const vector< boost::shared_ptr<Manipulation> >& findManipulations(
      boost::shared_ptr<const OsmMap> map, const vector<ElementId>& ids);

  virtual const vector< boost::shared_ptr<Manipulation> >& findWayManipulations(
      boost::shared_ptr<const OsmMap> map, const vector<long>& wids) = 0;
};

}

#endif // WAYMANIPULATOR_H
