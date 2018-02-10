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

  //find the area(s) as ways
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const int wayId = wayItr->first;

    ElementPtr pElement = planar->getElement(ElementType::Way, wayId);
    ElementPtr tElement;

    // We find the measurement using the projected map and transfer the result to
    // the main map via the element id.
    if (planar == map)
    {
      tElement = pElement;
    }
    else
    {
      tElement = map->getElement(pElement->getElementId());
    }

    boost::shared_ptr<Geometry> g = ElementConverter(planar).convertToGeometry(pElement, true, true);

    if (OsmSchema::getInstance().isArea(pElement))
    {
      if (pElement->getTags().get("feature_area").isEmpty())
      {
        tElement->getTags()["feature_area"] = QString::number(g->getArea(),'f',2);
      }
//      else
//      {
//        LOG_INFO("Has Area Tag: " + tElement->getTags()["feature_area"]);
//      }
    }
    else // We assume that it is linear
    {
      if (pElement->getTags().get("length").isEmpty())
      {
        tElement->getTags()["length"] = QString::number(g->getLength(),'f',2);
      }
//      else
//      {
//        LOG_INFO("Has Length Tag: " + tElement->getTags()["length"]);
//      }
    }
  }

  //find the area(s) as Relations
  const RelationMap& rels = map->getRelations();
  for (RelationMap::const_iterator relItr = rels.begin(); relItr != rels.end(); ++relItr)
  {
    const int relId = relItr->first;

    ElementPtr pElement = planar->getElement(ElementType::Relation, relId);
    ElementPtr tElement;

    // We find the measurement using the projected map and transfer the result to
    // the main map via the element id.
    if (planar == map)
    {
      tElement = pElement;
    }
    else
    {
      tElement = map->getElement(pElement->getElementId());
    }

    boost::shared_ptr<Geometry> g = ElementConverter(planar).convertToGeometry(pElement, true, true);

    if (OsmSchema::getInstance().isArea(pElement))
    {
      if (pElement->getTags().get("feature_area").isEmpty())
      {
        tElement->getTags()["feature_area"] = QString::number(g->getArea(),'f',2);
      }
//      else
//      {
//        LOG_INFO("Has Area Tag: " + tElement->getTags()["feature_area"]);
//      }
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
