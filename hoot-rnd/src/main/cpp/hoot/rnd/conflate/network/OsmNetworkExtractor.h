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
  void setCriterion(ElementCriterionPtr criterion);

private:
  friend class OsmNetworkExtractorVisitor;

  ElementCriterionPtr _criterion;

  bool _isValidElement(const ConstElementPtr& e);

  void _visit(const ConstElementPtr& e);
};

}

#endif // OSMNETWORKEXTRACTOR_H
