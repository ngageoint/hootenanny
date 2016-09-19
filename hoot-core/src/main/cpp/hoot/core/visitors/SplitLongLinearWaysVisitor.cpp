/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SplitLongLinearWaysVisitor.h"

#include <stddef.h>

#include <vector>

#include <boost/shared_ptr.hpp>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/Factory.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, SplitLongLinearWaysVisitor)

SplitLongLinearWaysVisitor::SplitLongLinearWaysVisitor():
  _maxNodesPerWay(0)
{
  _map = NULL;

  // Find out if user set our configuration value at cmdline or if we should use default
  ConfigOptions configOptions;
  _maxNodesPerWay = configOptions.getWayMaxNodesPerWay();

  if ( _maxNodesPerWay < 2 )
  {
    LOG_WARN("Invalid value for config value " << configOptions.getWayMaxNodesPerWayKey() << ": " <<
      _maxNodesPerWay << ", ignoring");
    _maxNodesPerWay = _defaultMaxNodesPerWay;
  }

  LOG_INFO("Max nodes per way set to " << getMaxNumberOfNodes() );
}

void SplitLongLinearWaysVisitor::visit(const boost::shared_ptr<Element>& element)
{
  // If not a way, ignore
  if ( element->getElementType() != ElementType::Way)
  {
    return;
  }

  // Convert to a Way
  boost::shared_ptr<Way> way = boost::dynamic_pointer_cast<Way>(element);
  boost::shared_ptr<Way> emptyWay;

  if ( way == emptyWay )
  {
    //LOG_DEBUG("SplitLongLinearWaysVisitor::visit: element is not a way, ignoring");
    return;
  }

  bool printInfo = false;
  if ( way->getNodeCount() > getMaxNumberOfNodes() )
  {
    LOG_DEBUG("Found way " << way->getId() << " with " << way->getNodeCount() << " nodes");
    printInfo = true;
  }

  // Make sure we've got highway tag
  Tags myTags = way->getTags();

  if ( myTags.contains("road") )
  {
    LOG_DEBUG("Way has road tag with value " << myTags.get("road")  );
  }

  // Ensure we're a linear way -- heuristic is reported to be mostly accurate
  if ( OsmSchema::getInstance().isLinear(*way) == false )
  {
    if ( printInfo == true )
    {
      LOG_DEBUG("SplitLongLinearWaysVisitor::visit: way " << way->getId() <<
        " is not linear, ignoring");
    }
    return;
  }

  // Does way exceed max number of nodes?
  if ( way->getNodeCount() <= getMaxNumberOfNodes() )
  {
    /*
    LOG_DEBUG("SplitLongLinearWaysVisitor::visit: way " << way->getId() <<
      " has " << way->getNodeCount() << " nodes which is <= than"
      " max of " << getMaxNumberOfNodes() << ", ignoring");
    */

    return;
  }

  LOG_DEBUG("SplitLongLinearWaysVisitor::visit: way " << way->getId() <<
    " has " << way->getNodeCount() << " nodes which is greater thank"
    " max of " << getMaxNumberOfNodes() << ", splitting this way!");

  unsigned int nodesRemaining = way->getNodeCount();

  std::vector< boost::shared_ptr< Way > > newWays;

  unsigned int masterNodeIndex = 0;
  while ( nodesRemaining > 0 )
  {
    unsigned int nodesThisTime = std::min(nodesRemaining, getMaxNumberOfNodes());

    // If there's only one node, this is a no-op as it's last node in previous way, don't add
    if ( nodesThisTime == 1 )
    {
      LOG_DEBUG("No need to add new way, there's only one node left over and it's stored in previous way");
      break;
    }

    // Create a new way
    boost::shared_ptr<Way> newWay( new Way(Status::Unknown1, _map->createNextWayId(),
      way->getRawCircularError() ) );
    LOG_DEBUG("Created new way w/ ID " << newWay->getId() << " that is going to hold " << nodesThisTime << " nodes");
    for ( unsigned int i = 0; i < nodesThisTime; ++i )
    {
      if ( i == 0 )
      {
        LOG_DEBUG("Adding first node to way " << newWay->getId() << " with master index " << (masterNodeIndex + i));
      }
      else if ( i == nodesThisTime - 1)
      {
        LOG_DEBUG("Adding last node to way " << newWay->getId() << " with master index " << (masterNodeIndex + i));
      }

      newWay->addNode(way->getNodeId(masterNodeIndex + i));
    }

    // Add new way to the map
    _map->addWay(newWay);

    // If we copied less than max nodes, that means we're done
    if ( nodesThisTime < getMaxNumberOfNodes() )
    {
      break;
    }
    else
    {
      // Figure out where to start next time -- note the minus one as last node in current way
      //    has to be first node in next way to ensure continuity from the original way
      nodesRemaining -= (nodesThisTime - 1);
      masterNodeIndex += (nodesThisTime - 1);
    }
  }

  // Remove original way from the map now that all the "child" ways are added
  RemoveWayOp::removeWay(_map->shared_from_this(), way->getId());
}


void SplitLongLinearWaysVisitor::setConfiguration(const Settings& )
{
  ;
}


}
