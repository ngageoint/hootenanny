
#include "NodesPerWayVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NodesPerWayVisitor)

NodesPerWayVisitor::NodesPerWayVisitor() :
_totalNodes(0),
_numWays(0),
_minNodesPerWay(INT_MAX),
_maxNodesPerWay(0)
{
}

void NodesPerWayVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    ConstWayPtr way = boost::dynamic_pointer_cast<const Way>(e);
    const int numNodes = way->getNodeCount();
    _totalNodes += numNodes;
    if (numNodes < _minNodesPerWay)
    {
      _minNodesPerWay = numNodes;
    }
    if (numNodes > _minNodesPerWay)
    {
      _minNodesPerWay = numNodes;
    }
    _numAffected++;
  }
}

}
