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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSMMAPINDEX_H
#define OSMMAPINDEX_H

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementListener.h>
#include <hoot/core/index/ElementToRelationMap.h>

// STL
#include <vector>

// TGS
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

  using namespace Tgs;

  class NodeToWayMap;

class OsmMapIndex : public ElementListener
{
public:
  OsmMapIndex(const OsmMap& map);

  void addNode(boost::shared_ptr<const Node> n);

  void addRelation(const boost::shared_ptr<const Relation>& r);

  void addWay(boost::shared_ptr<const Way> w);

  /**
   * The relation index can be very expensive to maintain when the relations are large. If it
   * isn't actively being used it can be beneficial to clear it out. It will be rebuilt when it
   * is needed again.
   */
  void clearRelationIndex() const { _elementToRelationMap.reset(); }

  /**
   * Return all the way elements that intersect with the specified envelope along with their parent
   * relations.
   */
  set<ElementId> findWayRelations(const Envelope& e) const;

  vector<long> findNodes(const Coordinate& from, Meters maxDistance) const;

  vector<long> findNodes(const Envelope& e) const;

  long findNearestWay(Coordinate c) const;

  /**
   * Should run in approximately O(lg(n)).
   * Due to the buffer added to ways this is only efficient with a planar projection.
   */
  std::vector<long> findWayNeighbors(const boost::shared_ptr<const Way>& way, Meters buffer,
                                     bool addError = false) const;

  /**
   * Very inefficient.
   */
  std::vector<long> findWayNeighbors(Coordinate& from, Meters buffer) const;

  std::vector<long> findWayNeighborsBruteForce(boost::shared_ptr<const Way> way, Meters buffer) const;

  /**
   * Return all ways that intersect the given envelope. This runs in approximately O(lg(n)) time.
   * Due to the buffer added to ways this is only efficient with a planar projection.
   */
  vector<long> findWays(const Envelope& e) const;

  Meters getIndexSlush() const { return _indexSlush; }

  boost::shared_ptr<NodeToWayMap> getNodeToWayMap() const;

  /**
   * Returns a map that can tell you which relations an element is part of.
   */
  const boost::shared_ptr<ElementToRelationMap>& getElementToRelationMap() const;

  /**
   * Unlike the way tree the node tree has no approximations built in.
   */
  boost::shared_ptr<const HilbertRTree> getNodeTree() const;

  /**
   * Get all the direct parents of a given element. This will not return grand parents, etc.
   */
  set<ElementId> getParents(ElementId eid) const;

  const vector<long>& getTreeIdToWidMap() const { return _treeIdToWid; }

  boost::shared_ptr<const HilbertRTree> getWayTree() const;

  /**
   * This gets called before an element changes. Between this call and the call to
   * postGeometryChange the index is in an inconsistent state.
   */
  virtual void preGeometryChange(Element* element);

  virtual void postGeometryChange(Element* element);

  void removeNode(boost::shared_ptr<const Node> n);

  void removeRelation(const boost::shared_ptr<const Relation>& r);

  void removeWay(boost::shared_ptr<const Way> w);

  void reset();

  /**
   * Checks that some of the existing indexes are consistent with the OsmMap backend. Any
   * inconsitencies will either be logged as warnings or throw exceptions.
   */
  bool validate() const;

private:
  const OsmMap& _map;

  Meters _indexSlush;

  mutable set<long> _pendingWayInsert;
  mutable set<long> _pendingWayRemoval;

  mutable set<long> _pendingNodeInsert;
  mutable set<long> _pendingNodeRemoval;

  mutable set<long> _pendingRelationChange;

  mutable boost::shared_ptr<HilbertRTree> _nodeTree;
  mutable boost::shared_ptr<HilbertRTree> _wayTree;

  mutable boost::shared_ptr<NodeToWayMap> _nodeToWayMap;
  mutable boost::shared_ptr<ElementToRelationMap> _elementToRelationMap;

  mutable vector<long> _treeIdToNid;
  mutable vector<long> _treeIdToWid;

  void _buildNodeTree() const;
  void _buildWayTree() const;

  int _createTreeNid(long nid) const;
  int _createTreeWid(long wid) const;

  void _insertNode(long nid);
  void _insertWay(long wid);

  long _mapToWayId(int treeId) const { return _treeIdToWid[treeId]; }
  long _mapToNodeId(int treeId) const { return _treeIdToNid[treeId]; }
};

}

#endif // OSMMAPINDEX_H
