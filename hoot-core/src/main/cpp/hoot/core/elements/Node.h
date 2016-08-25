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
#ifndef _ELEMENTS_NODE_H_
#define _ELEMENTS_NODE_H_

#include "Element.h"
#include "NodeData.h"

// GEOS
#include <geos/geom/Coordinate.h>
namespace geos {
  namespace geom {
    class Envelope;
    class Point;
  }
}

namespace hoot {

using namespace geos::geom;

class NodeData;

class Node : public Element
{
public:

  static string className() { return "hoot::Node"; }

  Node(const Node& from);

  Node(Status s, long id, const Coordinate& c, Meters circularError);

  Node(Status s, long id, double x, double y, Meters circularError,
       long changeset = ElementData::CHANGESET_EMPTY, long version = ElementData::VERSION_EMPTY,
       unsigned int timestamp = ElementData::TIMESTAMP_EMPTY,
       QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
       bool visible = ElementData::VISIBLE_EMPTY);

  virtual ~Node() {}

  /**
   * Clears all tags. However, unlike the other elements the x/y data and circular error aren't
   * modified b/c there isn't a clear definition of "unset" for this value.
   */
  virtual void clear();

  virtual Element* clone() const { return new Node(*this); }

  virtual Envelope* getEnvelope(const shared_ptr<const ElementProvider>& ep) const;

  double getX() const { return _nodeData.getX(); }

  double getY() const { return _nodeData.getY(); }

  void setX(double y);

  void setY(double x);

  virtual ElementType getElementType() const { return ElementType::Node; }

  geos::geom::Coordinate toCoordinate() const {
    return geos::geom::Coordinate(_nodeData.getX(), _nodeData.getY()); }

  boost::shared_ptr<geos::geom::Point> toPoint() const;

  QString toString() const;

  virtual void visitRo(const ElementProvider& map, ElementVisitor& visitor) const;

  virtual void visitRw(ElementProvider& map, ElementVisitor& visitor);

protected:

  NodeData _nodeData;

  virtual ElementData& _getElementData() { return _nodeData; }

  virtual const ElementData& _getElementData() const { return _nodeData; }

};

typedef boost::shared_ptr<Node> NodePtr;
typedef boost::shared_ptr<const Node> ConstNodePtr;

inline uint qHash(const ConstNodePtr& n)
{
  return qHash(n->getElementId());
}

} // hoot

#endif
