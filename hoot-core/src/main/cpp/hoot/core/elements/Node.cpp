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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "Node.h"

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

using namespace geos::geom;
using namespace std;

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

std::shared_ptr<Node> Node::cloneSp() const
{
  NodePtr result = SharedPtrPool<Node>::getInstance().allocate();
  result->_nodeData = _nodeData;
  return result;
}

Envelope* Node::getEnvelope(const std::shared_ptr<const ElementProvider>& /*ep*/) const
{
  return new Envelope(getX(), getX(), getY(), getY());
}

const Envelope& Node::getEnvelopeInternal(const std::shared_ptr<const ElementProvider>& /*ep*/) const
{
  _cachedEnvelope = Envelope(getX(), getX(), getY(), getY());
  return _cachedEnvelope;
}

void Node::setX(double x)
{
  _nodeData.setX(x);
}

void Node::setY(double y)
{
  _nodeData.setY(y);
}

QString Node::toString() const
{
  stringstream ss(stringstream::out);
  ss << "Node(" << getId() << "):"
     << " x: " << QString::number(getX(), 'f', ConfigOptions().getWriterPrecision())
     << " y: " << QString::number(getY(), 'f', ConfigOptions().getWriterPrecision())
     << " tags:" << endl
     << getTags().toString() << endl
     << " version: " << getVersion() << endl
     << " visible: " << getVisible() << endl
     << " status: " << getStatus().toString();
  if (hasCircularError())
    ss << endl << " circular error: " << QString::number(getCircularError());
  return QString::fromUtf8(ss.str().data());
}

void Node::visitRo(const ElementProvider& map, ConstElementVisitor& filter,
                   const bool /*recursive*/) const
{
  filter.visit(map.getNode(getId()));
}

void Node::visitRw(ElementProvider& map, ConstElementVisitor& filter, const bool /*recursive*/)
{
  filter.visit(std::dynamic_pointer_cast<const Node>(map.getNode(getId())));
}

bool Node::coordsMatch(const Node& other) const
{
  const int comparisonSensitivity = ConfigOptions().getNodeComparisonCoordinateSensitivity();
  const double x =
    std::round(getX() * std::pow(10.0, comparisonSensitivity)) /
    std::pow(10.0, comparisonSensitivity);
  const double otherX =
    std::round(other.getX() * std::pow(10.0, comparisonSensitivity)) /
    std::pow(10.0, comparisonSensitivity);
  const double y =
    std::round(getY() * std::pow(10.0, comparisonSensitivity)) /
    std::pow(10.0, comparisonSensitivity);
  const double otherY =
    std::round(other.getY() * std::pow(10.0, comparisonSensitivity)) /
    std::pow(10.0, comparisonSensitivity);
  return ((x - otherX) == 0.0) && ((y - otherY) == 0.0);
}

}
