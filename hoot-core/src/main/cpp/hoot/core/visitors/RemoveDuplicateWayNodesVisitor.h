
#ifndef REMOVEDUPLICATEWAYNODESVISITOR_H
#define REMOVEDUPLICATEWAYNODESVISITOR_H

// Hoot
#include "ElementOsmMapVisitor.h"

namespace hoot
{

/**
 * Removes all any way nodes that are duplicates, with the exception of start/end node duplicates.
 *
 * This is kind of a bandaid for the issue discovered in #2066.  The source creating the
 * duplicated nodes appears to be in the conflation routines somewhere and should be found and
 * fixed.  If that happens, this visitor could be removed from the post conflation ops.
 */
class RemoveDuplicateWayNodesVisitor : public ElementOsmMapVisitor
{
public:

  static std::string className() { return "hoot::RemoveDuplicateWayNodesVisitor"; }

  RemoveDuplicateWayNodesVisitor();

  virtual void visit(const ElementPtr& e);
};

}

#endif // REMOVEDUPLICATEWAYNODESVISITOR_H
