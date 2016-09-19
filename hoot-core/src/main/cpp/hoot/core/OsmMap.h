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
#ifndef OSMMAP_H
#define OSMMAP_H

// Boost
#include <boost/enable_shared_from_this.hpp>

// GDAL Includes
#include <ogrsf_frmts.h>

// GEOS Includes
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/Units.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>
namespace hoot {
    namespace elements {
        class Tags;
    }
}

// Standard
#include <deque>
#include <vector>

// TGS
#include <tgs/RStarTree/HilbertRTree.h>

#include "DefaultIdGenerator.h"
#include "RelationMap.h"
#include "WayMap.h"
#include "NodeMap.h"


namespace hoot {

using namespace std;

class OsmMapIndex;
class OsmMapListener;

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
class OsmMap : public enable_shared_from_this<OsmMap>, public ElementProvider
{
  // Friend classes that need to modify private elements
  friend class RemoveNodeOp;
  friend class RemoveWayOp;
  friend class RemoveRelationOp;

public:

  static string className() { return "hoot::OsmMap"; }

  OsmMap();

  explicit OsmMap(shared_ptr<const OsmMap>);

  explicit OsmMap(shared_ptr<OsmMap>);

  explicit OsmMap(shared_ptr<OGRSpatialReference> srs);

  OsmMap(shared_ptr<const OsmMap>, shared_ptr<OGRSpatialReference> srs);

  ~OsmMap();

  /**
   * Append all the elements in input map to this map.
   *
   * @param map
   * @throws If there is element ID overlap.
   * @throws If the map being appended to is the same as the map being appended from.
   * @throws If the map being appended to does not have the same projection as the map being
   * appended from
   */
  void append(shared_ptr<const OsmMap> map);

  void addElement(const shared_ptr<Element>& e);
  template<class T>
  void addElements(T it, T end);

  void addNode(const shared_ptr<Node>& n);

  void addRelation(const shared_ptr<Relation>& r);

  void addWay(const shared_ptr<Way>& w);

  void clear();

  /**
   * Returns true if an element with the specified type/id exists.
   * Throws an exception if the type is unrecognized.
   */
  virtual bool containsElement(const ElementId& eid) const;
  bool containsElement(ElementType type, long id) const;
  bool containsElement(const shared_ptr<const Element>& e) const;

  /**
   * Returns true if the node is in this map.
   */
  virtual bool containsNode(long id) const { return _nodes.find(id) != _nodes.end(); }

  virtual bool containsRelation(long id) const { return _relations.find(id) != _relations.end(); }

  virtual bool containsWay(long id) const { return _ways.find(id) != _ways.end(); }

  long createNextNodeId() const { return _idGen->createNodeId(); }

  long createNextRelationId() const { return _idGen->createRelationId(); }

  long createNextWayId() const { return _idGen->createWayId(); }

  virtual ConstElementPtr getElement(const ElementId& id) const;
  ConstElementPtr getElement(ElementType type, long id) const;
  ElementPtr getElement(const ElementId& id);
  ElementPtr getElement(ElementType type, long id);

  size_t getElementCount() const;

  const vector< shared_ptr<OsmMapListener> >& getListeners() const { return _listeners; }

  const IdGenerator& getIdGenerator() const { return *_idGen; }

  /**
   * This returns an index of the OsmMap. Adding or removing ways from the map will make the index
   * out of date and will require calling getIndex again.
   */
  const OsmMapIndex& getIndex() const { return *_index; }

  virtual const boost::shared_ptr<const Node> getNode(long id) const;

  virtual const shared_ptr<Node> getNode(long id);

  ConstNodePtr getNode(const ElementId& eid) const { return getNode(eid.getId()); }

  const NodePtr getNode(const ElementId& eid) { return getNode(eid.getId()); }

  const NodeMap& getNodeMap() const { return _nodes; }

  set<ElementId> getParents(ElementId eid) const;

  /**
   * Returns the SRS for this map. The SRS should never be changed and defaults to WGS84.
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const { return _srs; }

  virtual const shared_ptr<const Relation> getRelation(long id) const;

  virtual const shared_ptr<Relation> getRelation(long id);

  const RelationMap& getRelationMap() const { return _relations; }

  /**
   * Return the way with the specified id or null if it doesn't exist.
   */
  virtual const shared_ptr<Way> getWay(long id);
  const shared_ptr<Way> getWay(ElementId eid);
  
  /**
   * Similar to above but const'd.
   *
   * We can't return these values by reference b/c the conversion from non-const to const requires
   * a copy. The copy would be a temporary variable if we returned a reference which creates some
   * weirdness and a warning.
   */
  const shared_ptr<const Way> getWay(long id) const;
  const shared_ptr<const Way> getWay(ElementId eid) const;

  const WayMap& getWays() const { return _ways; }

  bool isEmpty() const { return _nodes.size() == 0 && _ways.size() == 0 && _relations.size() == 0;}

  void registerListener(shared_ptr<OsmMapListener> l) { _listeners.push_back(l); }

  /**
   * Replace the all instances of from with instances of to. In some cases this may be an invalid
   * operation and an exception will be throw. E.g. replacing a node with a way where the node
   * is part of another way.
   */
  void replace(const shared_ptr<const Element>& from, const shared_ptr<Element>& to);

  /**
   * Similar to above, but from is replaced with a collection of elements. This makes sense in the
   * context of a relation, but may not make sense in other cases (e.g. replace a single node
   * that is part of a way with multiple nodes).
   */
  void replace(const shared_ptr<const Element>& from, const QList<ElementPtr> &to);

  /**
   * Intelligently replaces all instances of oldNode with newNode. This looks at all the ways
   * for references to oldNode and replaces those references with newNode. Finally, oldNode is
   * removed from this OsmMap entirely.
   */
  void replaceNode(long oldId, long newId);

  /**
   * Resets the way and node counters. This should ONLY BE CALLED BY UNIT TESTS.
   */
  static void resetCounters() { IdGenerator::getInstance()->reset(); }

  void setIdGenerator(shared_ptr<IdGenerator> gen) { _idGenSp = gen; _idGen = gen.get(); }

  void setProjection(shared_ptr<OGRSpatialReference> srs);

  /**
   * Validates the consistency of the map. Primarily this checks to make sure that all nodes
   * referenced by a way exist in the map. A full dump of all invalid ways is logged before the
   * function throws an error.
   * @param strict If true, the method throws an exception rather than returning a result if the
   *               validation fails.
   * @return True if the map is valid, false otherwise.
   */
  bool validate(bool strict = true) const;

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
  void visitRo(ElementVisitor& visitor) const;
  void visitNodesRo(ElementVisitor& visitor) const;
  void visitWaysRo(ElementVisitor& visitor) const;
  void visitRelationsRo(ElementVisitor& visitor) const;


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
  void visitWaysRw(ElementVisitor& visitor);

protected:

  mutable IdGenerator* _idGen;
  mutable shared_ptr<IdGenerator> _idGenSp;

  static shared_ptr<OGRSpatialReference> _wgs84;

  boost::shared_ptr<OGRSpatialReference> _srs;

  mutable NodeMap _nodes;
  mutable RelationMap _relations;
  mutable WayMap _ways;

  shared_ptr<OsmMapIndex> _index;
  shared_ptr<Node> _nullNode;
  shared_ptr<const Node> _constNullNode;
  shared_ptr<Relation> _nullRelation;
  shared_ptr<Way> _nullWay;
  shared_ptr<const Way> _constNullWay;
  mutable NodeMap::const_iterator _tmpNodeMapIt;
  RelationMap::iterator _tmpRelationIt;
  mutable WayMap::const_iterator _tmpWayIt;
  std::vector< shared_ptr<OsmMapListener> > _listeners;

  vector< shared_ptr<Element> > _replaceTmpArray;

  void _copy(boost::shared_ptr<const OsmMap> from);

  /**
   * Returns true if there is a node in l.
   */
  bool _listContainsNode(const QList<ElementPtr> l) const;

  void _replaceNodeInRelations(long oldId, long newId);

};

typedef boost::shared_ptr<OsmMap> OsmMapPtr;
typedef boost::shared_ptr<const OsmMap> ConstOsmMapPtr;
// ConstOsmMapPtrR - The R is for reference.
typedef const ConstOsmMapPtr& ConstOsmMapPtrR;

template<class T>
void addElements(T it, T end)
{
  while (it != end)
  {
    addElement(*it);
    ++it;
  }
}

inline const shared_ptr<Node> OsmMap::getNode(long id)
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

inline const boost::shared_ptr<const Node> OsmMap::getNode(long id) const
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

inline const shared_ptr<const Relation> OsmMap::getRelation(long id) const
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

inline const shared_ptr<Relation> OsmMap::getRelation(long id)
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

inline const shared_ptr<const Way> OsmMap::getWay(long id) const
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

inline const shared_ptr<const Way> OsmMap::getWay(ElementId eid) const
{
  assert(eid.getType() == ElementType::Way);
  return getWay(eid.getId());
}

inline const shared_ptr<Way> OsmMap::getWay(long id)
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

inline const shared_ptr<Way> OsmMap::getWay(ElementId eid)
{
  assert(eid.getType() == ElementType::Way);
  return getWay(eid.getId());
}

}

#endif // OSMMAP_H
