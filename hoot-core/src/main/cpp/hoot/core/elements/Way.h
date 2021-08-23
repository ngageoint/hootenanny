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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __ELEMENT_WAY_H__
#define __ELEMENT_WAY_H__

// Geos
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/WayData.h>
#include <hoot/core/util/HootException.h>

// Standard

#include <memory>

namespace hoot
{

class Way : public Element
{
public:

  static QString className() { return "hoot::Way"; }

  Way(Status s, long id, Meters circularError = ElementData::CIRCULAR_ERROR_EMPTY,
      long changeset = ElementData::CHANGESET_EMPTY,
      long version = ElementData::VERSION_EMPTY,
      quint64 timestamp = ElementData::TIMESTAMP_EMPTY,
      QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
      bool visible = ElementData::VISIBLE_EMPTY, long pid = WayData::PID_EMPTY);

  Way(const Way& way);

  ~Way() = default;

  void addNode(long id);
  void insertNode(long index, long id);
  void addNodes(const std::vector<long>& ids);
  /**
   * Adds nodes from the provided iterator. This can be faster than repeatedly calling addNode
   * because the indexes will only need to be updated once.
   */
  template<typename IT>
  void addNodes(IT start, IT end);

  /**
   * Removes tags, nodes and circularError.
   */
  void clear() override;

  ElementPtr clone() const override { return std::make_shared<Way>(*this); }

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
   * The cached BBOX value will be serialized out to PBF. See OsmPbfWriter for details. This is most
   * useful in conjunction with Four Pass operations, but should generally be avoided unless there
   * are some other external guarantees.
   */
  const geos::geom::Envelope& getApproximateEnvelope(
    const std::shared_ptr<const ElementProvider>& ep) const;

  ElementType getElementType() const override { return ElementType(ElementType::Way); }

  /**
   * Returns the same result as getEnvelopeInternal, but copied so the caller gets ownership.
   */
  geos::geom::Envelope* getEnvelope(
    const std::shared_ptr<const ElementProvider>& ep) const override
  { return new geos::geom::Envelope(getEnvelopeInternal(ep)); }

  /**
   * Returns the envelope for this way. This is guaranteed to be exact. If any of the nodes for
   * this way are not loaded into RAM then the behavior is undefined (probably an assert).
   */
  const geos::geom::Envelope& getEnvelopeInternal(
    const std::shared_ptr<const ElementProvider>& ep) const override;

  /**
   * Returns the index of the first time this node occurs in the way. It is possible that the node
   * exists in the way multiple times.
   */
  int getNodeIndex(long nodeId) const;

  long getFirstNodeId() const { return getNodeId(0); }
  long getLastNodeId() const { return getNodeId(getNodeCount() - 1); }

  /**
   * Determines if an index is the first or last
   *
   * @param index index to examine
   * @return true if the index is extreme; false otherwise
   */
  bool isExtremeIndex(const int index) const
  { return index == 0 || index == (int)getNodeCount() - 1; }

  size_t getNodeCount() const { return _wayData->getNodeIds().size(); }

  long getNodeId(int index) const { return _wayData->getNodeIds()[index]; }

  const std::vector<long>& getNodeIds() const { return _wayData->getNodeIds(); }

  void setNodes(const std::vector<long>& newNodes);

  bool hasNode(long nodeId) const;

  bool isOneWay() const;

  /**
   * Determines if a node ID represents the first and last node
   *
   * @param nodeId ID of the node to search for
   * @return true if the node ID maps to the first and last node; false otherwise
   */
  bool isFirstAndLastNode(const long nodeId) const;

  /**
   * Determines if a node ID represents and end node
   *
   * @param nodeId ID of the node to search for
   * @return true if the node ID maps to the an end node; false otherwise
   */
  bool isEndNode(const long nodeId) const;

  /**
   * @brief isSimpleLoop - checks to see if the way starts and ends at the same
   *                       node. If it does, return true.
   * @return true if the way starts and ends at the same node
   */
  bool isSimpleLoop() const;

  /**
   * Returns true if this could possibly be a valid polygon. This is only checking for rudimentary
   * conditions and doesn't look for bow ties, etc.
   */
  bool isValidPolygon() const;

  /**
   * Returns true if there are more than 3 nodes and the first and last nodes in the Way are the
   * same.  Otherwise returns false
   */
  bool isClosedArea() const;

  /**
   * Remove all instances of the node with the specified id. If the node isn't in this way then
   * nothing happens.
   */
  void removeNode(long id) const;

  /**
   * Replaces any node instance with oldId with newId. If oldId isn't referenced by this way then
   * no action is taken. If newId already exists in the way, it is first removed before replacement.
   */
  void replaceNode(long oldId, long newId);

  /**
   * Reverse the order of the nodes.
   */
  void reverseOrder();

  /**
   * Determines if two ways have the same nodes
   *
   * @param other way to compare with
   * @return true if the other way has the same nodes in the same order as this way; false otherwise
   */
  bool hasSameNodes(const Way& other) const;

  /**
   * Determines if two ways share the same node
   *
   * @param other way to compare with
   * @return true if the other way shares at least one node with this way; false otherwise
   */
  bool hasSharedNode(const Way& other) const;

  /**
   * Determines if two ways share the same end node
   *
   * @param other way to compare with
   * @return true if the other way shares at least one end node with this way; false otherwise
   */
  bool hasSharedEndNode(const Way& other) const;

  /**
   * Retrieves the IDs of shared nodes between two ways
   *
   * @param other way to compare with
   * @return a collection of node IDs
   */
  QSet<long> sharedNodeIds(const Way& other) const;

  /**
   * This is rarely used. Primarily it is useful when loading the way from a file that does
   * cache way envelope bounds (see .osm.pbf).
   */
  void setCachedEnvelope(const geos::geom::Envelope& e) { _cachedEnvelope = e; }

  QString toString() const override;

  /**
   * @see Element
   */
  void visitRo(const ElementProvider& map, ConstElementVisitor& filter,
               const bool recursive = true) const override;

  /**
   * @see Element
   */
  void visitRw(ElementProvider& map, ConstElementVisitor& filter,
               const bool recursive = true) override;

  /**
   * Functions for getting/setting/resetting the parent ID, i.e. the ID of the way
   * that this way was split from
   */
  bool hasPid() const { return _wayData->getPid() != WayData::PID_EMPTY; }
  long getPid() const { return _wayData->getPid(); }
  void setPid(long pid) const { _wayData->setPid(pid); }
  void resetPid() const { _wayData->setPid(WayData::PID_EMPTY); }
  static long getPid(const std::shared_ptr<const Way>& p, const std::shared_ptr<const Way>& c);
  static long getPid(long p, long c);

protected:

  ElementData& _getElementData() override { _makeWritable(); return *_wayData; }
  const ElementData& _getElementData() const override { return *_wayData; }

private:

  std::shared_ptr<WayData> _wayData;

  void _makeWritable();

  // for debugging only; SLOW - We don't check for duplicated nodes (outside of start/end) at
  // runtime due to the performance hit. So, use this to debug when that occurs.
  bool _nodeIdsAreDuplicated(const std::vector<long>& ids) const;
};

using WayPtr = std::shared_ptr<Way>;
using ConstWayPtr = std::shared_ptr<const Way>;

inline bool operator<(const WayPtr& w1, const WayPtr& w2)
{
  return w1->getElementId() < w2->getElementId();
}

inline bool operator<(const ConstWayPtr& w1, const ConstWayPtr& w2)
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

struct WayPtrCompare
{
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
