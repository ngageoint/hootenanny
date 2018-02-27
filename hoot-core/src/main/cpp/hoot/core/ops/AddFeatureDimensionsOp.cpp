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

#include "AddFeatureDimensionsOp.h"

// geos
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/Polygon.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, AddFeatureDimensionsOp)

AddFeatureDimensionsOp::AddFeatureDimensionsOp()
{
}

void AddFeatureDimensionsOp::apply(boost::shared_ptr<OsmMap> &map)
{
  boost::shared_ptr<OsmMap> planar;

    // If the map is geographic, we need to project it so we can get measurements in metres.
  if (MapProjector::isGeographic(map))
  {
    planar.reset(new OsmMap(map));
    MapProjector::projectToPlanar(planar);
  }
  else
  {
    planar = map;
  }

  // find the area(s) as ways
  const WayMap& ways = planar->getWays();
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const int wayId = wayItr->first;

    ElementPtr pElement = planar->getElement(ElementType::Way, wayId);

    // NOTE: pElement and tElement may be identical
    ElementPtr tElement = map->getElement(pElement->getElementId());

    boost::shared_ptr<Geometry> g = ElementConverter(planar).convertToGeometry(pElement, true, true);

    if (OsmSchema::getInstance().isArea(pElement))
    {
      if (pElement->getTags().get("feature_area").isEmpty())
      {
        tElement->getTags().set("feature_area",QString::number(g->getArea(),'f',2));
      }
      continue;
    }

    // Not an Area, must be Linear
    if (pElement->getTags().get("length").isEmpty())
    {
      tElement->getTags().set("length",QString::number(g->getLength(),'f',2));
    }
  }

  // find the area(s) as Relations
  const RelationMap& rels = planar->getRelations();
  for (RelationMap::const_iterator relItr = rels.begin(); relItr != rels.end(); ++relItr)
  {
    const int relId = relItr->first;

    ElementPtr pElement = planar->getElement(ElementType::Relation, relId);
    ElementPtr tElement = map->getElement(pElement->getElementId());

    boost::shared_ptr<Geometry> g = ElementConverter(planar).convertToGeometry(pElement, true, true);

    if (OsmSchema::getInstance().isArea(pElement))
    {
      if (pElement->getTags().get("feature_area").isEmpty())
      {
        tElement->getTags().set("feature_area",QString::number(g->getArea(),'f',2));
      }
      continue;
    }

    // Just making sure....
    if (OsmSchema::getInstance().isLinear(*pElement))
    {
      if (pElement->getTags().get("length").isEmpty())
      {
        tElement->getTags().set("length",QString::number(g->getLength(),'f',2));
      }
    }
  }

} // End apply

void AddFeatureDimensionsOp::readObject(QDataStream& /*is*/)
{
  // no parameters to read
}

void AddFeatureDimensionsOp::writeObject(QDataStream& /*os*/) const
{
  // no parameters to write
}

} // End namespace
