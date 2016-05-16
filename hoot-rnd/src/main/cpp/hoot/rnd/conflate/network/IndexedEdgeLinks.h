#ifndef INDEXEDEDGELINKS_H
#define INDEXEDEDGELINKS_H

#include "EdgeMatch.h"

namespace hoot
{

/**
 * links in the form {from, [to]}
 *
 * To make the terminology a little more precise, the term link will refer to a link from one
 * pair of edges to another pair of edges.
 */
class IndexedEdgeLinks : public QMultiHash<EdgeMatchPtr, EdgeMatchPtr>
{
public:
  IndexedEdgeLinks();
};

}

#endif // INDEXEDEDGELINKS_H
