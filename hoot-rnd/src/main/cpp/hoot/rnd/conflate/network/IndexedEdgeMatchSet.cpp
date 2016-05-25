#include "IndexedEdgeMatchSet.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

IndexedEdgeMatchSet::IndexedEdgeMatchSet()
{
}

void IndexedEdgeMatchSet::addEdgeMatch(const EdgeMatchPtr& em, double score)
{
  if (!contains(em))
  {
    _matches.insert(em, score);
  }
}

shared_ptr<IndexedEdgeMatchSet> IndexedEdgeMatchSet::clone() const
{
  IndexedEdgeMatchSetPtr result(new IndexedEdgeMatchSet());
  result->_matches = _matches;
  return result;
}

bool IndexedEdgeMatchSet::contains(const EdgeMatchPtr& em) const
{
  bool result = false;

  if (_matches.contains(em))
  {
    result = true;
  }
  else
  {
    EdgeMatchPtr r = em->clone();
    r->reverse();
    if (_matches.contains(r))
    {
      result = true;
    }
  }

  return result;
}

EdgeMatchPtr IndexedEdgeMatchSet::getMatch(const EdgeMatchPtr &em) const
{
  MatchHash::const_iterator it = _matches.find(em);

  if (it == _matches.end())
  {
    throw HootException("The specified match was not found: " + em->toString());
  }

  return it.key();
}

QSet<EdgeMatchPtr> IndexedEdgeMatchSet::getMatchesWithTermination(ConstNetworkVertexPtr v1,
  ConstNetworkVertexPtr v2) const
{
  QSet<EdgeMatchPtr> result;

  /// @todo replace this with an index. This will get quite slow w/ 100k edges.

  foreach (const EdgeMatchPtr& em, _matches.keys())
  {
    if (em->getString1()->getFrom() == v1 &&
      em->getString2()->getFrom() == v2)
    {
      result.insert(em);
    }
    else if (em->getString1()->getTo() == v1 &&
      em->getString2()->getTo() == v2)
    {
      result.insert(em);
    }
  }

  return result;
}

QString IndexedEdgeMatchSet::toString() const
{
  return hoot::toString(_matches);
}

}
