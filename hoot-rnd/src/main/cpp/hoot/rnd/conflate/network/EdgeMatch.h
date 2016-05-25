#ifndef EDGEMATCH_H
#define EDGEMATCH_H

#include "EdgeString.h"

namespace hoot
{

/**
 * Represents the match between two sets of edges. This could represent:
 *
 * - a 1:1 match where all of edge1 matches all of edge2
 * - a n:m match where all the edges in n combine to match all the edges in m
 * - a partial match where all or some of edge1 matches all or some of edge2
 * - This will always be a contiguous match between edges1 and edges2
 */
class EdgeMatch
{
public:
  EdgeMatch();

  EdgeStringPtr getString1() { return _edges1; }

  EdgeStringPtr getString2() { return _edges2; }

  ConstEdgeStringPtr getString1() const { return _edges1; }

  ConstEdgeStringPtr getString2() const { return _edges2; }

  shared_ptr<EdgeMatch> clone() const;

  /**
   * Returns true if the specified edge is in either the first or second edge string.
   */
  bool contains(ConstNetworkEdgePtr e) const;

  /**
   * Returns true if any of the edges in this edge match overlap with other. Overlapping vertexes
   * are ignored.
   */
  bool overlaps(const shared_ptr<const EdgeMatch>& other) const;

  /**
   * Reverse both edge strings that make up this EdgeMatch.
   */
  void reverse() const { _edges1->reverse(); _edges2->reverse(); }

  QString toString() const;

private:

  EdgeStringPtr _edges1, _edges2;
};

typedef shared_ptr<EdgeMatch> EdgeMatchPtr;
typedef shared_ptr<const EdgeMatch> ConstEdgeMatchPtr;

// needed for QSet
bool operator==(const hoot::EdgeMatchPtr& em1, const hoot::EdgeMatchPtr& em2);
uint qHash(const hoot::EdgeMatchPtr& em);

}

#endif // EDGEMATCH_H
