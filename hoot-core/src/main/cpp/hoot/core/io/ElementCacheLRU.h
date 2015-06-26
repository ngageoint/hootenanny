#ifndef ELEMENTCACHELRU_H
#define ELEMENTCACHELRU_H

#include <map>
#include <utility>         // For std::pair
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/ElementOutputStream.h>
#include "ElementCache.h"

#include <ogr_spatialref.h>


namespace hoot
{

/**
 * Class description
 */
class ElementCacheLRU : public ElementCache
{

public:

  /**
   * @brief ElementCacheLRU
   * @param maxCountEachElementType Total size of cache is three times this value (i.e.,
   *      cache can hold that many nodes, that many ways, and that many relations before items
   *      start being replaced)
   */
  ElementCacheLRU(const unsigned long maxCountEachElementType);

  /**
   * @brief ~ElementCache
   */
  virtual ~ElementCacheLRU() { }

  virtual bool isEmpty() const;

  virtual unsigned long size() const;

  /**
   * @brief addElement
   * @param newElement
   * @note The last access time for an element added to the cache will be initialized to the
   *    current time
   */
  virtual void addElement(ConstElementPtr& newElement);

  virtual void resetElementIterators();

  virtual ConstNodePtr getNextNode();

  virtual ConstWayPtr getNextWay();

  virtual ConstRelationPtr getNextRelation();

  // Functions for ElementInputStream
  virtual void close();             // Also works for elementoutputstream
  virtual bool hasMoreElements();

  virtual ElementPtr readNextElement();

  // Functions for ElementOutputStream
  virtual void writeElement(ElementInputStream& inputStream);

  // Functions for ElementProvider

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual bool containsElement(const ElementId& eid) const;

  virtual ConstElementPtr getElement(const ElementId& id) const;

  virtual const boost::shared_ptr<const Node> getNode(long id) const;

  virtual const boost::shared_ptr<Node> getNode(long id);

  virtual const boost::shared_ptr<const Relation> getRelation(long id) const;

  virtual const boost::shared_ptr<Relation> getRelation(long id);

  virtual const boost::shared_ptr<const Way> getWay(long id) const;

  virtual const boost::shared_ptr<Way> getWay(long id);

  virtual bool containsNode(long id) const;

  virtual bool containsRelation(long id) const;

  virtual bool containsWay(long id) const;

protected:

  unsigned long _maxCountPerType;

  boost::shared_ptr<OGRSpatialReference> _projection;

  /// Nodes in the cache (key is node ID, then value is pair for node and its access time)
  std::map<long, std::pair<ConstNodePtr, boost::posix_time::ptime> >      _nodes;

  /// Iterator used to walk nodes in cache
  std::map<long, std::pair<ConstNodePtr, boost::posix_time::ptime> >::iterator _nodesIter;

  /// Ways in the cache (key is way ID, then value is pair for way and its access time)
  std::map<long, std::pair<ConstWayPtr, boost::posix_time::ptime> >       _ways;

  /// Iterator used to walk ways in cache
  std::map<long, std::pair<ConstWayPtr, boost::posix_time::ptime> >::iterator _waysIter;

  /// Relations in the cache (key is relation ID, then value is pair for relation and access time)
  std::map<long, std::pair<ConstRelationPtr, boost::posix_time::ptime> >  _relations;

  /// Iterator used to walk relations in cache
  std::map<long,
    std::pair<ConstRelationPtr, boost::posix_time::ptime> >::iterator _relationsIter;

  void _removeOldest(const ElementType::Type typeToRemove);
};

typedef boost::shared_ptr<ElementCacheLRU> ElementCacheLRUPtr;

}

#endif // ELEMENTCACHELRU_H
