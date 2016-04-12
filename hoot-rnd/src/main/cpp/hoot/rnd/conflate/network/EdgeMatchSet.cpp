#include "EdgeMatchSet.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

EdgeMatchSet::EdgeMatchSet()
{
}

void EdgeMatchSet::addEdgeMatch(const EdgeMatchPtr& em, double score)
{
  if (!contains(em))
  {
    _matches.insert(em, score);
  }
}

bool EdgeMatchSet::contains(const EdgeMatchPtr& em) const
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

QString EdgeMatchSet::toString() const
{
  return hoot::toString(_matches);
}

}
