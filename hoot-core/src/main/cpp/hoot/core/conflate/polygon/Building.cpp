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
#include "Building.h"

// geos
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

Building::Building(const OsmMap& map, const ConstElementPtr& e) : _e(e), _map(map)
{
}

boost::shared_ptr<Geometry> Building::buildOutline() const
{
  ElementConverter ec(_map.shared_from_this());
  boost::shared_ptr<Geometry> result;

  // if this is a building relation
  if (_e->getElementType() == ElementType::Relation)
  {
    // construct the outline from the union of the parts.
    result.reset(GeometryFactory::getDefaultInstance()->createEmptyGeometry());

    ConstRelationPtr r = dynamic_pointer_cast<const Relation>(_e);
    const vector<RelationData::Entry> entries = r->getMembers();
    for (size_t i = 0; i < entries.size(); i++)
    {
      if (entries[i].role == "part")
      {
        boost::shared_ptr<Geometry> g = ec.convertToGeometry(_map.getElement(entries[i].getElementId()));
        result.reset(result->Union(g.get()));
      }
    }
  }
  else
  {
    result = ec.convertToGeometry(_e);
  }
  return result;
}

}
