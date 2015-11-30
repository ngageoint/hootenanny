#ifndef OSMNETWORKEXTRACTOR_H
#define OSMNETWORKEXTRACTOR_H

// hoot
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>

namespace hoot
{

class OsmNetworkExtractorVisitor;

/**
 * Traverse the OSM Map and extract a graph that represents some network. E.g. a road network, a
 * hydrography network, etc.
 */
class OsmNetworkExtractor
{
public:
  OsmNetworkExtractor();

  OsmNetworkPtr extractNetwork(ConstOsmMapPtr map);

  /**
   * What criterion must be satisified to include an element in this graph? For instance, status
   * is unknown1 and HighwayCriterion is satisfied.
   *
   * This criterion should only let linear types through. No arbitrary collections or nodes.
   */
  void setCriterion(ElementCriterionPtr criterion) { _criterion = criterion; }

private:
  friend class OsmNetworkExtractorVisitor;

  ElementCriterionPtr _criterion;
  ConstOsmMapPtr _map;
  OsmNetworkPtr _network;

  void _addEdge(ConstElementPtr from, ConstElementPtr to, QList<ConstElementPtr> members,
    bool directed);

  /**
   * This is a very strict definition of contiguous. We're looking to make sure that the first
   * member connects to the second, connects to the third, etc. We could be more liberal, but we
   * can let that be use case driven.
   */
  bool _isContiguous(const ConstRelationPtr& r);

  bool _isValidElement(const ConstElementPtr& e);

  void _visit(const ConstElementPtr& e);
};

}

#endif // OSMNETWORKEXTRACTOR_H
