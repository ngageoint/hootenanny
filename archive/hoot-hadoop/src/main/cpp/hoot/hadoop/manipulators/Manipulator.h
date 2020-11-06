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
#ifndef MANIPULATOR_H
#define MANIPULATOR_H

// hoot
#include <hoot/core/elements/ElementId.h>

// Standard Includes
#include <vector>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

class Manipulation;
class OsmMap;

/**
 * Manipulates a working map in some fashion that produces a vector of new working maps. The new
 * working maps should have a reasonable chance of ultimately producing a better map than the input.
 */
class Manipulator
{
public:
  virtual ~Manipulator() {}

  static std::string className() { return "hoot::Manipulator"; }

  virtual const std::vector< boost::shared_ptr<Manipulation> >& findAllManipulations(
          boost::shared_ptr<const OsmMap> map) = 0;

  virtual const std::vector< boost::shared_ptr<Manipulation> >& findManipulations(
      boost::shared_ptr<const OsmMap> map, const std::vector<ElementId>& ids) = 0;

};

}

#endif
