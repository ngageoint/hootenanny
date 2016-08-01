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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Node.h"

// Boost
using namespace boost;

// GEOS
#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

Node::Node(Status s, long id, const Coordinate& c, Meters circularError) :
Element(s)
{
  _nodeData.init(id, c.x, c.y);
  _getElementData().setCircularError(circularError);
}

Node::Node(Status s, long id, double x, double y, Meters circularError, long changeset,
           long version, unsigned int timestamp, QString user, long uid, bool visible) :
Element(s)
{
  _nodeData.init(id, x, y, changeset, version, timestamp, user, uid, visible);
  _getElementData().setCircularError(circularError);
}

Node::Node(const Node& from) :
Element(from.getStatus()),
_nodeData(from._nodeData)
{
}

void Node::clear()
{
  _nodeData.clear();
}

Envelope* Node::getEnvelope(const shared_ptr<const ElementProvider> &/*ep*/) const
{
  return new Envelope(getX(), getX(), getY(), getY());
}

void Node::setX(double x)
{
  _nodeData.setX(x);
}

void Node::setY(double y)
{
  _nodeData.setY(y);
}

shared_ptr<geos::geom::Point> Node::toPoint() const
{
  Coordinate c(getX(), getY());

  shared_ptr<Point> result(GeometryFactory::getDefaultInstance()->createPoint(c));

  return result;
}

QString Node::toString() const
{
  return
    QString("Node(%1): x: %2 y: %3 tags:\n%4\n version: %5\n visible: %6")
      .arg(getId())
      .arg(getX())
      .arg(getY())
      .arg(getTags().toString())
      .arg(getVersion())
      .arg(getVisible());
}

void Node::visitRo(const ElementProvider& map, ElementVisitor& filter) const
{
  filter.visit(map.getNode(getId()));
}

void Node::visitRw(ElementProvider& map, ElementVisitor& filter)
{
  filter.visit(map.getNode(getId()));
}

}
