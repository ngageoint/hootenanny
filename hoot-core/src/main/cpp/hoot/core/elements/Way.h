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
#ifndef __ELEMENT_WAY_H__
#define __ELEMENT_WAY_H__

// Local
#include "WayData.h"
#include "Node.h"

// Boost
#include <boost/shared_ptr.hpp>

// Geos
#include <geos/geom/Envelope.h>

namespace geos {
  namespace geom {
    class LinearRing;
    class LineString;
    class Polygon;
  }
}

// Hoot
#include <hoot/core/util/HootException.h>

// Standard
#include <deque>

namespace hoot
{

using namespace boost;
using namespace geos::geom;
using namespace std;

class Way : public Element
{
public:

  static string className() { return "hoot::Way"; }

  Way(Status s, long id, Meters circularError, long changeset = ElementData::CHANGESET_EMPTY,
      long version = ElementData::VERSION_EMPTY,
      unsigned int timestamp = ElementData::TIMESTAMP_EMPTY,
      QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
      bool visible = ElementData::VISIBLE_EMPTY);

  Way(const Way& way);

  virtual ~Way();

  void addNode(long id);

  void addNodes(const vector<long>& ids);

  /**
   * Adds nodes from the provided iterator. This can be faster than repeatedly calling addNode
   * because the indexes will only need to be updated once.
   */
  template<typename IT>
  void addNodes(IT start, IT end);

  /**
   * Removes tags, nodes and circularError.
   */
  virtual void clear();

  Element* clone() const { return new Way(*this); }

  bool containsNodeId(long nid) const;

  /**
   * Returns the approximate (AKA cached) envelope for this way. If the exact envelope is known
   * then it will be returned. Otherwise, if the envelope was cached in the input file it will
   * use any knowledge available to expand (not contract) the envelope and return a best guess.
   * If none of the nodes for this way are loaded into memory and there was no envelope cached in
   * the input file then this function may return a "null" envelope (e.g. isNull() returns
   * true). There are no guarantees on how close this envelope will be to the "correct" envelope.
   *
   * Be cause of the approximate nature the envelope may change between calls even if the Way
   * doesn't change. E.g. a node that was not originally in RAM appears and changes the bounding
   * box between calls. Unlikely, but possible.
   *
   * The cached BBOX value will be serialized out to PBF. See PbfWriter for details. This is most
   * useful in conjunction with Four Pass operations, but should generally be avoided unless there
   * are some other external guarantees.
   */
  const Envelope& getApproximateEnvelope(shared_ptr<const ElementProvider> ep) const;

  virtual ElementType getElementType() const { return ElementType::Way; }

  /**
   * Returns the same result as getEnvelopeInternal, but copied so the caller gets ownership.
   */
  virtual Envelope* getEnvelope(const shared_ptr<const ElementProvider>& ep) const
  { return new Envelope(getEnvelopeInternal(ep)); }

  /**
   * Returns the envelope for this way. This is guaranteed to be exact. If any of the nodes for
   * this way are not loaded into RAM then the behavior is undefined (probably an assert).
   */
  const Envelope& getEnvelopeInternal(shared_ptr<const ElementProvider> ep) const;

  /**
   * Returns the index of the first time this node occurs in the way. It is possible that the node
   * exists in the way multiple times.
   */
  int getNodeIndex(long nodeId) const;

  long getFirstNodeId() const { return getNodeId(0); }

  long getLastNodeId() const { return getNodeId(getNodeCount() - 1); }

  size_t getNodeCount() const { return _wayData->getNodeIds().size(); }

  long getNodeId(int index) const { return _wayData->getNodeIds()[index]; }

  const std::vector<long>& getNodeIds() const { return _wayData->getNodeIds(); }

  void setNodes(const vector<long>& newNodes);

  bool hasNode(long nodeId) const;

  bool isOneWay() const;

  /**
   * Returns true if this could possibly be a valid polygon. This is only checking for rudimentary
   * conditions and doesn't look for bow ties, etc.
   */
  bool isValidPolygon() const;

  /**
   * Returns True if the first and last node in the Way have the same node ID, provided the way
   * contains two or more nodes. If way has zero or one nodes, returns false
   */
  bool isFirstLastNodeIdentical() const;

  /**
   * Remove all instances of the node with the specified id. If the node isn't in this way then
   * nothing happens.
   */
  void removeNode(long id);

  /**
   * Replaces any node instance with oldId with newId. If oldId isn't referenced by this way then
   * no action is taken.
   */
  void replaceNode(long oldId, long newId);

  /**
   * Reverse the order of the nodes.
   */
  void reverseOrder();

  /**
   * This is rarely used. Primarily it is useful when loading the way from a file that does
   * cache way envelope bounds (see .osm.pbf).
   */
  void setCachedEnvelope(const Envelope& e) { _cachedEnvelope = e; }

  QString toString() const;

  virtual void visitRo(const ElementProvider& map, ElementVisitor& filter) const;

  virtual void visitRw(ElementProvider& map, ElementVisitor& filter);

protected:

  boost::shared_ptr<WayData> _wayData;
  /**
   * This envelope may be cached, but it also may not be exact.
   */
  mutable Envelope _cachedEnvelope;

  virtual ElementData& _getElementData() { _makeWritable(); return *_wayData; }

  virtual const ElementData& _getElementData() const { return *_wayData; }

  void _makeWritable();

private:

};

typedef boost::shared_ptr<Way> WayPtr;
typedef boost::shared_ptr<const Way> ConstWayPtr;

inline bool operator<(WayPtr w1, WayPtr w2)
{
  return w1->getElementId() < w2->getElementId();
}

inline bool operator<(ConstWayPtr w1, ConstWayPtr w2)
{
  return w1->getElementId() < w2->getElementId();
}

template<typename IT>
void Way::addNodes(IT start, IT end)
{
  _preGeometryChange();
  _makeWritable();
  while (start != end)
  {
    _wayData->addNode(*start);
    ++start;
  }
  _postGeometryChange();
}

struct WayPtrCompare {
  bool operator() (const ConstWayPtr& w1, const ConstWayPtr& w2) const
  {
    return w1->getId() < w2->getId();
  }

  bool operator() (const WayPtr& w1, const WayPtr& w2) const
  {
    return w1->getId() < w2->getId();
  }
};

}

#endif // __ELEMENT_WAY_H__
