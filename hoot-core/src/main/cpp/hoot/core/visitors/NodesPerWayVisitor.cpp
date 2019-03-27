
#include "NodesPerWayVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NodesPerWayVisitor)

NodesPerWayVisitor::NodesPerWayVisitor() :
_totalWayNodes(0),
_minNodesPerWay(INT_MAX),
_maxNodesPerWay(0)
{
}

void NodesPerWayVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    ConstWayPtr way = boost::dynamic_pointer_cast<const Way>(e);
    const int numWayNodes = way->getNodeCount();
    _totalWayNodes += numWayNodes;
    if (numWayNodes < _minNodesPerWay)
    {
      _minNodesPerWay = numWayNodes;
    }
    if (numWayNodes > _maxNodesPerWay)
    {
      _maxNodesPerWay = numWayNodes;
    }
    _numAffected++;
  }
}

}
