#ifndef EDGEMATCHSET_H
#define EDGEMATCHSET_H

#include "EdgeMatch.h"

namespace hoot
{

class EdgeMatchSet
{
public:
  EdgeMatchSet();
  
  void addEdgeMatch(const EdgeMatchPtr& em, double score);

  /**
   * Returns true if the specified element (or the reversed equivalent) is contained in this set.
   */
  bool contains(const EdgeMatchPtr &em) const;

  QString toString() const;

private:
  QHash<EdgeMatchPtr, double> _matches;
};

typedef shared_ptr<EdgeMatchSet> EdgeMatchSetPtr;
typedef shared_ptr<const EdgeMatchSet> ConstEdgeMatchSetPtr;

}

#endif // EDGEMATCHSET_H
