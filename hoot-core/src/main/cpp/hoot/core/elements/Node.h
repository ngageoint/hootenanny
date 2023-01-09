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
#ifndef _ELEMENTS_NODE_H_
#define _ELEMENTS_NODE_H_

// GEOS
#include <geos/geom/Point.h>

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/NodeData.h>
#include <hoot/core/util/SharedPtrPool.h>

namespace hoot
{

class Node : public Element
{
public:

  static QString className() { return "Node"; }

  Node(const Node& from);
  Node(Status s, long id, const geos::geom::Coordinate& c,
       Meters circularError = ElementData::CIRCULAR_ERROR_EMPTY);
  Node(Status s, long id, double x, double y,
       Meters circularError = ElementData::CIRCULAR_ERROR_EMPTY,
       long changeset = ElementData::CHANGESET_EMPTY, long version = ElementData::VERSION_EMPTY,
       OsmTimestamp timestamp = ElementData::TIMESTAMP_EMPTY,
       QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
       bool visible = ElementData::VISIBLE_EMPTY);
  ~Node() override = default;

  /**
   * Allocate a node as a shared pointer. At this time the allocated node will be allocated as
   * part of an object pool which should avoid some memory fragmentation and provide faster
   * allocation/deletion. See: https://github.com/ngageoint/hootenanny/issues/1715
   *
   * The implementation details may change in the future.
   *
   * @param s Status of this Node.
   * @param id The ID of the node, typically new nodes get negative IDs. See
   *  OsmMap::getIdGenerator()
   * @param x The x coordinate. The actual meaning depends on the projection of the containing
   *  OsmMap.
   * @param circularError The circular error in meters. @sa Element::getCircularError()
   * @return A newly allocated and initialized Node shared pointer.
   */
  static std::shared_ptr<Node> newSp(Status s, long id, double x, double y,
                                     Meters circularError = ElementData::CIRCULAR_ERROR_EMPTY);
  static std::shared_ptr<Node> newSp(Status s, long id, double x, double y, Meters circularError,
                                     long changeset, long version, OsmTimestamp timestamp,
                                     QString user = ElementData::USER_EMPTY,
                                     long uid = ElementData::UID_EMPTY,
                                     bool visible = ElementData::VISIBLE_EMPTY);

  /**
   * Determines if the coordinates from this node match with that of another given a configurable
   * tolerance
   *
   * @param other the node to compare coordinates with
   * @return true if the coordinates match; false otherwise
   */
  bool coordsMatch(const Node& other) const;

  geos::geom::Coordinate toCoordinate() const
  { return geos::geom::Coordinate(_nodeData.getX(), _nodeData.getY()); }

  /**
   * Clears all tags. However, unlike the other elements the x/y data and circular error aren't
   * modified b/c there isn't a clear definition of "unset" for this value.
   */
  void clear() override;

  ElementPtr clone() const override { return std::make_shared<Node>(*this); }
  /**
   * Clone this node as a shared pointer. At this time the allocated node will be allocated as
   * part of an object pool which should avoid some memory fragmentation and provide faster
   * allocation/deletion. See: https://github.com/ngageoint/hootenanny/issues/1715
   *
   * The implementation details may change in the future.
   *
   * @return A newly allocated and copied Node shared pointer.
   * @todo wrap this inside of clone() and make it the default behavior?
   */
  std::shared_ptr<Node> cloneSp() const;

  std::shared_ptr<geos::geom::Envelope> getEnvelope(const std::shared_ptr<const ElementProvider>& ep) const override;
  const geos::geom::Envelope& getEnvelopeInternal(const std::shared_ptr<const ElementProvider>& ep) const override;

  ElementType getElementType() const override { return ElementType(ElementType::Node); }

  QString toString() const override;

  /**
   * @see Element
   */
  void visitRo(const ElementProvider& map, ConstElementVisitor& visitor, const bool recursive = true) const override;
  void visitRw(ElementProvider& map, ConstElementVisitor& visitor, const bool recursive = true) override;

  double getX() const { return _nodeData.getX(); }
  double getY() const { return _nodeData.getY(); }

  void setX(double y);
  void setY(double x);

private:

  friend class SharedPtrPool<Node>;

  /**
   * The default constructor shouldn't really be used in typical code. We really _need_ the
   * parameters passed into the other constructors. However, the pool method requires a default
   * constructor. To work around this, the pool objects are friends (above).
   */
  Node() = default;

  NodeData _nodeData;

  ElementData& _getElementData() override { return _nodeData; }
  const ElementData& _getElementData() const override { return _nodeData; }
};

using NodePtr = std::shared_ptr<Node>;
using ConstNodePtr = std::shared_ptr<const Node>;

inline NodePtr Node::newSp(Status s, long id, double x, double y, Meters circularError)
{
  NodePtr result = SharedPtrPool<Node>::getInstance().allocate();

  result->_nodeData.init(id, x, y);
  result->_getElementData().setCircularError(circularError);
  result->setStatus(s);

  return result;
}

inline NodePtr Node::newSp(Status s, long id, double x, double y, Meters circularError,
                           long changeset, long version, OsmTimestamp timestamp, QString user,
                           long uid, bool visible)
{
  NodePtr result = SharedPtrPool<Node>::getInstance().allocate();

  result->_nodeData.init(id, x, y, changeset, version, timestamp, user, uid, visible);
  result->_getElementData().setCircularError(circularError);
  result->setStatus(s);

  return result;
}

inline uint qHash(const ConstNodePtr& n)
{
  return qHash(n->getElementId());
}

}

#endif
