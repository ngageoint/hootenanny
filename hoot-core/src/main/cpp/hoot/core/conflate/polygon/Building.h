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
#ifndef BUILDING_H
#define BUILDING_H

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{
using namespace boost;
using namespace geos;

class Building
{
public:
  Building(const OsmMap& map, const ConstElementPtr& e);

  /**
   * Given an input building, build the outline for the building and return it. This will not
   * add the outline to the building's element. If the building does have an outline it is ignored
   * and a new one is generated from the parts.
   */
  boost::shared_ptr<Geometry> buildOutline() const;

private:
  ConstElementPtr _e;
  const OsmMap& _map;
};

}

#endif // BUILDING_H
