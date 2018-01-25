
#include "RemoveDuplicateWayNodesVisitor.h"

// Hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QVector>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, RemoveDuplicateWayNodesVisitor)

RemoveDuplicateWayNodesVisitor::RemoveDuplicateWayNodesVisitor()
{
}

void RemoveDuplicateWayNodesVisitor::visit(const ElementPtr& e)
{
  if (e->getElementType() == ElementType::Way)
  {
    LOG_TRACE("Looking for duplicate way nodes...");

    WayPtr way = boost::dynamic_pointer_cast<Way>(e);
    assert(way.get());
    const std::vector<long>& wayNodeIds = way->getNodeIds();
    LOG_VART(wayNodeIds);

    QVector<long> parsedNodeIds;
    QVector<long> duplicateWayNodeIds;
    bool foundDuplicateWayNode = false;
    for (size_t i = 0; i < wayNodeIds.size(); i++)
    {
      const long nodeId = wayNodeIds[i];
      LOG_VART(nodeId);
      if (!parsedNodeIds.contains(nodeId))
      {
        //we haven't seen this node ID yet
        LOG_TRACE("Valid way node: " << nodeId);
        parsedNodeIds.append(nodeId);
      }
      else if (i == wayNodeIds.size() -1 && nodeId == parsedNodeIds.at(0))
      {
        //this node ID is duplicated, but its the end node duplicating the start node, which is ok
        //(closed way)
        LOG_TRACE("End node matches start node: " << nodeId);
        parsedNodeIds.append(nodeId);
      }
      else
      {
        //we've found a duplicate where both nodes aren't start and end nodes...shouldn't ever
        //happen
        LOG_TRACE("Found duplicate way node: " << nodeId);
        duplicateWayNodeIds.append(nodeId);
        foundDuplicateWayNode = true;
      }
    }
    if (foundDuplicateWayNode)
    {
      LOG_TRACE("Removing duplicate way nodes: " << duplicateWayNodeIds);
      LOG_VART(parsedNodeIds);
      way->setNodes(parsedNodeIds.toStdVector());
    }
  }
}

}
