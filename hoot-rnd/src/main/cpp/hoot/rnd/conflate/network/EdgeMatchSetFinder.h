#ifndef EDGEMATCHSETFINDER_H
#define EDGEMATCHSETFINDER_H

#include "IndexedEdgeMatchSet.h"
#include "MatchScoreProvider.h"
#include "NetworkDetails.h"

namespace hoot
{

class EdgeMatchSetFinder
{
public:
  EdgeMatchSetFinder(NetworkDetailsPtr details, IndexedEdgeMatchSetPtr matchSet,
    ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2);

  /**
   * Evaluate edges e1 and e2 for match. If the end vertices don't match then recursively search
   * for potential matches until an end is found.
   */
  void addEdgeMatches(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

private:

  NetworkDetailsPtr _details;
  IndexedEdgeMatchSetPtr _matchSet;
  ConstOsmNetworkPtr _n1, _n2;
  int _steps;

  void _addEdgeMatches(EdgeMatchPtr em);

  void _addEdgeNeighborsToEnd(EdgeMatchPtr em, QList<ConstNetworkEdgePtr> neighbors1,
    QList<ConstNetworkEdgePtr> neighbors2);

  void _addEdgeNeighborsToStart(EdgeMatchPtr em, QList<ConstNetworkEdgePtr> neighbors1,
    QList<ConstNetworkEdgePtr> neighbors2);

  /// @todo replace with a real method
  double _scoreMatch(EdgeMatchPtr em) const;
};

}

#endif // EDGEMATCHSETFINDER_H
