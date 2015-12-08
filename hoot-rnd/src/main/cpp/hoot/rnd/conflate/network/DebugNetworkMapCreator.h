#ifndef DEBUGNETWORKMAPCREATOR_H
#define DEBUGNETWORKMAPCREATOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/rnd/conflate/network/NetworkEdgeScore.h>
#include <hoot/rnd/conflate/network/NetworkVertexScore.h>

namespace hoot
{

/**
 * Create a number of elements in the specified map that describe the network matches that were
 * established. This is useful for debugging.
 */
class DebugNetworkMapCreator
{
public:
  DebugNetworkMapCreator();

  void addDebugElements(OsmMapPtr map, QList<NetworkEdgeScorePtr> edgeScores,
    QList<NetworkVertexScorePtr> vertexScores);

private:
  void _addEdgeLink(OsmMapPtr map, NetworkEdgeScorePtr edgeScore);

  void _addVertexLink(OsmMapPtr map, NetworkVertexScorePtr vertexScore);

  ConstNodePtr _getMedianNode(ConstOsmMapPtr map, QList<ConstElementPtr> e);
  ConstNodePtr _getMedianNode(ConstOsmMapPtr map, ConstElementPtr e);
};

}

#endif // DEBUGNETWORKMAPCREATOR_H
