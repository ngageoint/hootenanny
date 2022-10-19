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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "MultiLineStringVisitor.h"

// geos
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MultiLineStringVisitor)

std::shared_ptr<geos::geom::Geometry> MultiLineStringVisitor::createGeometry()
{
  if (_ways.empty())  //  Empty ways gets an empty multilinestring object
    return std::shared_ptr<geos::geom::Geometry>(GeometryFactory::getDefaultInstance()->createMultiLineString().release());
  else
  {
    //  Check all of the ways to make sure that they are mergable
    bool mergable = true;
    const Tags& first_tags = _ways[0]->getTags();
    int first_count = first_tags.getInformationCount();
    for (const auto& w : _ways)
    {
      const Tags& tags = w->getTags();
      if (first_count != tags.getInformationCount() || TagComparator::getInstance().compareTags(tags, first_tags) > 0.0)
      {
        mergable = false;
        break;
      }
    }
    //  Merging the multiline string requires converting all strings into one vector of coordinates and creating
    //  a geos::geom::Linestring
    if (mergable)
    {
      vector<Coordinate>* coords = new vector<Coordinate>();
      long last_id = 0;

      //  Iterate the ways in order to add all nodes to the coordinate vector
      for (const auto& w : _ways)
      {
        for (const auto& n : w->getNodeIds())
        {
          //  Make sure that we haven't already used this ID
          if (last_id == n)
            continue;
          //  Update the latest node ID
          last_id = n;
          //  Convert the node to a coordinate and add it
          ConstNodePtr node = _provider->getNode(n);
          coords->push_back(Coordinate(node->getX(), node->getY()));
        }
      }
      //  Create a coordinate sequence that we pass ownership of to the new geos::geom::LineString object
      CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(coords).release();
      return std::shared_ptr<geos::geom::Geometry>(GeometryFactory::getDefaultInstance()->createLineString(cs));
    }
    else
    {
      //  Create a vector of linestrings that will be passed to the geometry factory
      vector<const Geometry*> ls;
      for (const auto& w : _ways)
        ls.push_back(ElementToGeometryConverter(_provider).convertToLineString(w)->clone().release());
      //  Create the MultiLineString object
      return std::shared_ptr<geos::geom::MultiLineString>(GeometryFactory::getDefaultInstance()->createMultiLineString(ls));
    }
  }
}

void MultiLineStringVisitor::visit(const ConstElementPtr& e)
{
  if (!e)
    return;

  if (e->getElementType() == ElementType::Way)
    _visit(std::dynamic_pointer_cast<const Way>(e));
}

void MultiLineStringVisitor::_visit(const ConstWayPtr& w)
{
  if (!w || w->getNodeCount() < 2)
    return;

  _ways.push_back(w);
}

}
