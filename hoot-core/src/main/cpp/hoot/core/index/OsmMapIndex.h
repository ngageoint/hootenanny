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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef OSMMAPINDEX_H
#define OSMMAPINDEX_H

// Hoot
#include <hoot/core/elements/ElementListener.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/ElementToRelationMap.h>

// TGS
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

class NodeToWayMap;

class OsmMapIndex : public ElementListener
{
public:

  OsmMapIndex(const OsmMap& map);
  ~OsmMapIndex() override = default;

  void addNode(const ConstNodePtr& n);
  void addRelation(const ConstRelationPtr& r) const;
  void addWay(ConstWayPtr w);

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
  std::set<ElementId> findWayRelations(const geos::geom::Envelope& e) const;

  std::vector<long> findNodes(const geos::geom::Coordinate& from, Meters maxDistance) const;

  long findNearestWay(geos::geom::Coordinate c) const;

  /**
   * Should run in approximately O(log(n)).
   * Due to the buffer added to ways, this is only efficient with a planar projection.
   */
  std::vector<long> findWayNeighbors(const ConstWayPtr& way, Meters buffer, bool addError = false) const;
  /**
   * Very inefficient.
   */
  std::vector<long> findWayNeighbors(const geos::geom::Coordinate& from, Meters buffer) const;
  std::vector<long> findWayNeighborsBruteForce(ConstWayPtr way, Meters buffer) const;

  /**
   * Return all ways that intersect the given envelope. This runs in approximately O(log(n)) time.
   * Due to the buffer added to ways, this is only efficient with a planar projection.
   */
  std::vector<long> findWays(const geos::geom::Envelope& e) const;

  Meters getIndexSlush() const { return _indexSlush; }

  std::shared_ptr<NodeToWayMap> getNodeToWayMap() const;

  /**
   * Returns a map that can tell you which relations an element is part of.
   */
  const std::shared_ptr<ElementToRelationMap>& getElementToRelationMap() const;

  /**
   * Unlike the way tree the node tree has no approximations built in.
   */
  std::shared_ptr<const Tgs::HilbertRTree> getNodeTree() const;

  /**
   * Get all the direct parents of a given element. This will not return grandparents, etc.
   */
  std::set<ElementId> getParents(ElementId eid) const;

  const std::vector<long>& getTreeIdToWidMap() const { return _treeIdToWid; }

  std::shared_ptr<const Tgs::HilbertRTree> getWayTree() const;

  /**
   * This gets called before an element changes. Between this call and the call to
   * postGeometryChange the index is in an inconsistent state.
   */
  void preGeometryChange(Element* element) override;
  void postGeometryChange(Element* element) override;

  void removeNode(ConstNodePtr n);
  void removeRelation(const ConstRelationPtr& r) const;
  void removeWay(ConstWayPtr w);

  void reset();

  /**
   * Checks that some of the existing indexes are consistent with the OsmMap backend. Any
   * inconsistencies will either be logged as warnings or throw exceptions.
   */
  bool validate() const;

private:

  const OsmMap& _map;

  Meters _indexSlush;

  mutable std::set<long> _pendingWayInsert;
  mutable std::set<long> _pendingWayRemoval;

  mutable std::set<long> _pendingNodeInsert;
  mutable std::set<long> _pendingNodeRemoval;

  mutable std::set<long> _pendingRelationChange;

  mutable std::shared_ptr<Tgs::HilbertRTree> _nodeTree;
  mutable std::shared_ptr<Tgs::HilbertRTree> _wayTree;

  mutable std::shared_ptr<NodeToWayMap> _nodeToWayMap;
  mutable std::shared_ptr<ElementToRelationMap> _elementToRelationMap;

  mutable std::vector<long> _treeIdToNid;
  mutable std::vector<long> _treeIdToWid;

  void _buildNodeTree() const;
  void _buildWayTree() const;

  int _createTreeNid(long nid) const;
  int _createTreeWid(long wid) const;

  void _insertNode(long nid) const;
  void _insertWay(long wid) const;

  long _mapToWayId(int treeId) const { return _treeIdToWid[treeId]; }
  long _mapToNodeId(int treeId) const { return _treeIdToNid[treeId]; }
};

inline void OsmMapIndex::addNode(const ConstNodePtr& n)
{
  if (_nodeTree)
  {
    // rather than manipulate the R-Tree (expensive) we'll just keep track of what changed.
    _pendingNodeInsert.insert(n->getId());
    _pendingNodeRemoval.erase(n->getId());
  }
}

}

#endif // OSMMAPINDEX_H
