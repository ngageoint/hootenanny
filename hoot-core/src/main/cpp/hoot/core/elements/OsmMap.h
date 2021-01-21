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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMMAP_H
#define OSMMAP_H

// GDAL Includes
#include <ogrsf_frmts.h>

// GEOS Includes
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/NodeMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/RelationMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/WayMap.h>
#include <hoot/core/util/DefaultIdGenerator.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/elements/ElementIterator.h>

#include <memory>

namespace hoot
{
  namespace elements
  {
    class Tags;
  }
}

// Standard
#include <vector>

namespace hoot
{

class ElementId;
class IdSwap;
class OsmMapIndex;
class OsmMapListener;
class Roundabout;
class RubberSheet;

/**
 * The OsmMap contains all the information necessary to represent an OSM map. It holds the nodes,
 * ways, relations and an index to access them efficiently. It also provides a number of methods
 * for CRUD.
 *
 *  - In the long term it might be nice to remove the OsmIndex circular reference, but I
 *    haven't figured out a good way to do that. Possibly refactor into an OsmMap class
 *    and OsmData class. The OsmMap class maintains pointers to OsmData and an OsmIndex
 *    where neither directly references the other. (?)
 */
class OsmMap : public std::enable_shared_from_this<OsmMap>, public ElementProvider,
  public ElementIterator
{
  // Friend classes that need to modify private elements
  friend class RemoveNodeByEid;
  friend class RemoveWayByEid;
  friend class RemoveRelationByEid;

public:

  static std::string className() { return "hoot::OsmMap"; }

  OsmMap();

  explicit OsmMap(const std::shared_ptr<const OsmMap>&);
  explicit OsmMap(const std::shared_ptr<OsmMap>&);
  explicit OsmMap(const std::shared_ptr<OGRSpatialReference>& srs);
  OsmMap(const std::shared_ptr<const OsmMap>&, const std::shared_ptr<OGRSpatialReference>& srs);

  ~OsmMap() = default;

  // GENERIC ELEMENT

  void addElement(const std::shared_ptr<Element>& e);
  template<class T>
  void addElements(T it, T end);

  virtual ConstElementPtr getElement(const ElementId& id) const;
  ConstElementPtr getElement(ElementType type, long id) const;
  ElementPtr getElement(const ElementId& id);
  ElementPtr getElement(ElementType type, long id);

  QSet<ElementId> getElementIds() const;

  size_t getElementCount() const;
  size_t size() const { return getElementCount(); }

  /**
   * Returns true if an element with the specified type/id exists.
   * Throws an exception if the type is unrecognized.
   */
  virtual bool containsElement(const ElementId& eid) const;
  bool containsElement(ElementType type, long id) const;
  bool containsElement(const std::shared_ptr<const Element>& e) const;

  /**
   * Calls the visitRo method on all elements. See Element::visitRo for a more
   * thorough description.
   *  - The order will always be nodes, ways, relations, but the IDs will not
   *    be in any specific order.
   *  - Unlike Element::visitRo, elements will not be visited multiple times.
   *  - Modifying the OsmMap while traversing will result in undefined behaviour.
   *  - This should be slightly faster than visitRw.
   *
   * If the visitor implements OsmMapConsumer then setOsmMap will be called before visiting any
   * elements.
   */
  void visitRo(ConstElementVisitor& visitor) const;

  /**
   * Calls the visitRw method on all elements. See Element::visitRw for a more
   * thorough description.
   *  - The order will always be nodes, ways, relations, but the IDs will not
   *    be in any specific order.
   *  - Elements that are added during the traversal may or may not be visited.
   *  - Elements may be deleted during traversal.
   *  - The visitor is guaranteed to not visit deleted elements.
   *
   * If the visitor implements OsmMapConsumer then setOsmMap will be called before
   * visiting any elements.
   */
  void visitRw(ElementVisitor& visitor);
  void visitRw(ConstElementVisitor& visitor);

  /**
   * Replace the all instances of from with instances of to. In some cases this may be an invalid
   * operation and an exception will be throw. E.g. replacing a node with a way where the node
   * is part of another way.
   */
  void replace(const std::shared_ptr<const Element>& from, const std::shared_ptr<Element>& to);
  /**
   * Similar to above, but from is replaced with a collection of elements. This makes sense in the
   * context of a relation, but may not make sense in other cases (e.g. replace a single node
   * that is part of a way with multiple nodes).
   */
  void replace(const std::shared_ptr<const Element>& from, const QList<ElementPtr>& to);

  //NODE///////////////////////////////////////////////////////////////////////////////////

  virtual const ConstNodePtr getNode(long id) const;
  virtual const NodePtr getNode(long id);
  ConstNodePtr getNode(const ElementId& eid) const { return getNode(eid.getId()); }
  const NodePtr getNode(const ElementId& eid) { return getNode(eid.getId()); }
  const NodeMap& getNodes() const { return _nodes; }
  QSet<long> getNodeIds() const;
  QSet<ElementId> getNodeElementIds() const;

  long getNodeCount() const { return _nodes.size(); }

  /**
   * Returns true if the node is in this map.
   */
  virtual bool containsNode(long id) const { return _nodes.find(id) != _nodes.end(); }

  void addNode(const NodePtr& n);
  /**
   * Add all the nodes in the provided vector. This can be faster than calling addNode multiple
   * times.
   */
  void addNodes(const std::vector<NodePtr>& nodes);

  /**
   * Intelligently replaces all instances of oldNode with newNode. This looks at all the ways
   * for references to oldNode and replaces those references with newNode. Finally, oldNode is
   * removed from this OsmMap entirely.
   */
  void replaceNode(long oldId, long newId);

  long createNextNodeId() const { return _idGen->createNodeId(); }

  void visitNodesRo(ConstElementVisitor& visitor) const;
  void visitNodesRw(ConstElementVisitor& visitor);
  void visitNodesRw(ElementVisitor& visitor);

  int numNodesAppended() const { return _numNodesAppended; }
  int numNodesSkippedForAppending() const { return _numNodesSkippedForAppending; }

  //WAY///////////////////////////////////////////////////////////////////////////////////

  /**
   * Return the way with the specified id or null if it doesn't exist.
   */
  virtual const WayPtr getWay(long id);
  const WayPtr getWay(ElementId eid);
  /**
   * Similar to above but const'd.
   *
   * We can't return these values by reference b/c the conversion from non-const to const requires
   * a copy. The copy would be a temporary variable if we returned a reference which creates some
   * weirdness and a warning.
   */
  const ConstWayPtr getWay(long id) const;
  const ConstWayPtr getWay(ElementId eid) const;
  const WayMap& getWays() const { return _ways; }
  QSet<long> getWayIds() const;
  QSet<ElementId> getWayElementIds() const;
  long getWayCount() const { return _ways.size(); }

  void addWay(const WayPtr& w);

  virtual bool containsWay(long id) const { return _ways.find(id) != _ways.end(); }

  long createNextWayId() const { return _idGen->createWayId(); }

  void visitWaysRo(ConstElementVisitor& visitor) const;
  void visitWaysRw(ConstElementVisitor& visitor);
  void visitWaysRw(ElementVisitor& visitor);

  int numWaysAppended() const { return _numWaysAppended; }
  int numWaysSkippedForAppending() const { return _numWaysSkippedForAppending; }

  //RELATION///////////////////////////////////////////////////////////////////////////////////

  virtual const ConstRelationPtr getRelation(long id) const;
  virtual const RelationPtr getRelation(long id);
  const ConstRelationPtr getRelation(ElementId eid) const;
  const RelationMap& getRelations() const { return _relations; }
  QSet<long> getRelationIds() const;
  QSet<ElementId> getRelationElementIds() const;
  long getRelationCount() const { return _relations.size(); }

  void addRelation(const RelationPtr& r);

  virtual bool containsRelation(long id) const { return _relations.find(id) != _relations.end(); }

  long createNextRelationId() const { return _idGen->createRelationId(); }

  void visitRelationsRo(ConstElementVisitor& visitor) const;
  void visitRelationsRw(ConstElementVisitor& visitor);
  void visitRelationsRw(ElementVisitor& visitor);

  int numRelationsAppended() const { return _numRelationsAppended; }
  int numRelationsSkippedForAppending() const { return _numRelationsSkippedForAppending; }

  /////////////////////////////////////////////////////////////////////////////////////

  /**
   * Append all the elements in input map to this map.
   *
   * The default behavior is to skip an element from the map being appended from if it has the same
   * ID as an element in this map and the elements are considered identical. If the elements are
   * considered to be identical, an error occurs. Alternatively, the throwOutDupes parameter will
   * allow for overriding that behavior at the expense of not appending the elements.
   *
   * @param map
   * @param throwOutDupes if true, and elements in the map being appended from have the same IDs as
   * elements in this map, those elements are ignored
   * @throws if there is element ID overlap and throwOutDupes = false
   * @throws if the map being appended to is the same as the map being appended from
   * @throws if the map being appended to does not have the same projection as the map being
   * appended from
   */
  void append(const std::shared_ptr<const OsmMap>& map, const bool throwOutDupes = false);

  const std::vector<std::shared_ptr<OsmMapListener>>& getListeners() const { return _listeners; }

  /**
   * This returns an index of the OsmMap. Adding or removing ways from the map will make the index
   * out of date and will require calling getIndex again.
   */
  const OsmMapIndex& getIndex() const { return *_index; }

  std::set<ElementId> getParents(ElementId eid) const;

  /**
   * Returns the SRS for this map. The SRS should never be changed and defaults to WGS84.
   */
  virtual std::shared_ptr<OGRSpatialReference> getProjection() const { return _srs; }

  void clear();

  bool isEmpty() const { return getElementCount() == 0; }

  void registerListener(const std::shared_ptr<OsmMapListener>& l) { _listeners.push_back(l); }

  /**
   * Resets the way and node counters. This should ONLY BE CALLED BY UNIT TESTS.
   */
  static void resetCounters() { IdGenerator::getInstance()->reset(); }

  const IdGenerator& getIdGenerator() const { return *_idGen; }
  void setIdGenerator(const std::shared_ptr<IdGenerator>& gen)
  { _idGen = gen;  }

  void setProjection(const std::shared_ptr<OGRSpatialReference>& srs);

  /**
   * Validates the consistency of the map. Primarily this checks to make sure that all nodes
   * referenced by a way exist in the map. A full dump of all invalid ways is logged before the
   * function throws an error.
   * @param strict If true, the method throws an exception rather than returning a result if the
   *               validation fails.
   * @return True if the map is valid, false otherwise.
   */
  bool validate(bool strict = true) const;

  // Helps us handle roundabouts
  void setRoundabouts(const std::vector<std::shared_ptr<Roundabout>>& rnd) { _roundabouts = rnd; }
  std::vector<std::shared_ptr<Roundabout>> getRoundabouts() const { return _roundabouts; }

  //  Handle ID preservation swaps
  void setIdSwap(const std::shared_ptr<IdSwap>& swap) { _idSwap = swap; }
  std::shared_ptr<IdSwap> getIdSwap() const { return _idSwap; }

  QString getName() const { return _name; }
  void setName(const QString& name) { _name = name; }

  QString getSource() const;
  void appendSource(const QString& url);
  void replaceSource(const QString& url);

  void setEnableProgressLogging(bool enable) { _enableProgressLogging = enable; }

  void setCachedRubberSheet(std::shared_ptr<RubberSheet> rubbersheet) { _cachedRubberSheet = rubbersheet; }
  std::shared_ptr<RubberSheet> getCachedRubberSheet() const { return _cachedRubberSheet; }

protected:

  mutable std::shared_ptr<IdGenerator> _idGen;

  static std::shared_ptr<OGRSpatialReference> _wgs84;

  std::shared_ptr<OGRSpatialReference> _srs;

  mutable NodeMap _nodes;
  mutable RelationMap _relations;
  mutable WayMap _ways;

  std::shared_ptr<OsmMapIndex> _index;
  NodePtr _nullNode;
  ConstNodePtr _constNullNode;
  RelationPtr _nullRelation;
  WayPtr _nullWay;
  ConstWayPtr _constNullWay;
  mutable NodeMap::const_iterator _tmpNodeMapIt;
  RelationMap::iterator _tmpRelationIt;
  mutable WayMap::const_iterator _tmpWayIt;
  std::vector<std::shared_ptr<OsmMapListener>> _listeners;

  std::vector<std::shared_ptr<Element>> _replaceTmpArray;

  std::vector<std::shared_ptr<Roundabout>> _roundabouts;

  std::shared_ptr<IdSwap> _idSwap;

  // useful during debugging
  QString _name;
  /** List of source URLs of map data */
  std::set<QString> _sources;

  int _numNodesAppended;
  int _numWaysAppended;
  int _numRelationsAppended;
  int _numNodesSkippedForAppending;
  int _numWaysSkippedForAppending;
  int _numRelationsSkippedForAppending;

  // If we're making recursive calls to the visit methods in another class doing its own progress
  // logging, its helpful to be able to turn loging here off.
  bool _enableProgressLogging;

  // for use with ElementIterator
  ElementId _currentElementId;
  NodeMap::const_iterator _currentNodeItr;
  WayMap::const_iterator _currentWayItr;
  RelationMap::const_iterator _currentRelationItr;

  std::shared_ptr<RubberSheet> _cachedRubberSheet;

  void _copy(const std::shared_ptr<const OsmMap>& from);

  /**
   * Returns true if there is a node in l.
   */
  bool _listContainsNode(const QList<ElementPtr> l) const;

  void _replaceNodeInRelations(long oldId, long newId);

  void _initCounters();

  virtual void _next();
  virtual void resetIterator();
};

typedef std::shared_ptr<OsmMap> OsmMapPtr;
typedef std::shared_ptr<const OsmMap> ConstOsmMapPtr;

template<class T>
void addElements(T it, T end)
{
  while (it != end)
  {
    addElement(*it);
    ++it;
  }
}

inline const NodePtr OsmMap::getNode(long id)
{
  _tmpNodeMapIt = _nodes.find(id);
  if (_tmpNodeMapIt != _nodes.end())
  {
    return _tmpNodeMapIt->second;
  }
  else
  {
    return _nullNode;
  }
}

inline const ConstNodePtr OsmMap::getNode(long id) const
{
  _tmpNodeMapIt = _nodes.find(id);
  if (_tmpNodeMapIt != _nodes.end())
  {
    return _tmpNodeMapIt->second;
  }
  else
  {
    return _constNullNode;
  }
}

inline const ConstRelationPtr OsmMap::getRelation(long id) const
{
  RelationMap::iterator it = _relations.find(id);
  if (it != _relations.end())
  {
    return it->second;
  }
  else
  {
    return _nullRelation;
  }
}

inline const ConstRelationPtr OsmMap::getRelation(ElementId eid) const
{
  return getRelation(eid.getId());
}

inline const RelationPtr OsmMap::getRelation(long id)
{
  _tmpRelationIt = _relations.find(id);
  if (_tmpRelationIt != _relations.end())
  {
    return _tmpRelationIt->second;
  }
  else
  {
    return _nullRelation;
  }
}

inline const ConstWayPtr OsmMap::getWay(long id) const
{
  _tmpWayIt = _ways.find(id);
  if (_tmpWayIt != _ways.end())
  {
    return _tmpWayIt->second;
  }
  else
  {
    return _constNullWay;
  }
}

inline const ConstWayPtr OsmMap::getWay(ElementId eid) const
{
  return getWay(eid.getId());
}

inline const WayPtr OsmMap::getWay(long id)
{
  _tmpWayIt = _ways.find(id);
  if (_tmpWayIt != _ways.end())
  {
    return _tmpWayIt->second;
  }
  else
  {
    return _nullWay;
  }
}

inline const WayPtr OsmMap::getWay(ElementId eid)
{
  return getWay(eid.getId());
}

}

#endif // OSMMAP_H
