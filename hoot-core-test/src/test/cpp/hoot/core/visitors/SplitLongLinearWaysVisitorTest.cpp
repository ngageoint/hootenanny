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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include <limits> // for std::min

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/SplitLongLinearWaysVisitor.h>

namespace hoot
{

class SplitLongLinearWaysVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SplitLongLinearWaysVisitorTest);
  CPPUNIT_TEST(defaultConstructorNoOpTest);
  CPPUNIT_TEST(defaultConstructorModifyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void defaultConstructorNoOpTest()
  {
    SplitLongLinearWaysVisitor splitVisitor;
    CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(1900), splitVisitor.getMaxNumberOfNodes());

    _noOpTests(splitVisitor);

  }

  void defaultConstructorModifyTest()
  {
    SplitLongLinearWaysVisitor splitVisitor;
    CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(1900), splitVisitor.getMaxNumberOfNodes());

    _modifyTests(splitVisitor);
  }

private:
  std::shared_ptr<OsmMap> _map;

  void _createWay(const long wayId, const unsigned int startNodeId, const unsigned int endNodeId)
  {
    // Make sure way in question does not already exist in map
    CPPUNIT_ASSERT_EQUAL(_map->containsWay(wayId), false );

    WayPtr newWay = std::make_shared<Way>(Status::Unknown1, wayId, 1.0);

    // Make sure there are nodes to add
    if (startNodeId <= endNodeId)
    {
      // Create the nodes and and add them to the map, if necessary
      for (unsigned int i = startNodeId; i <= endNodeId; i++)
      {
        // Make sure map does not already have node in question
        if (_map->containsNode(i) == false)
        {
          _map->addNode(std::make_shared<Node>(Status::Unknown1, i, i * 1.0, i * 1.0, 1.0));
        }

        // Add the node into the new way
        newWay->addNode(i);
      }
    }
    else
    {
      LOG_TRACE("Adding way to map, but could not add any nodes due to start ID of " << startNodeId
        << " and end node ID " << endNodeId );
    }

    // Tag the way as a highway to ensure it passes linear test
    newWay->setTag("highway", "road");

    // Add the way to the map
    _map->addWay(newWay);
    LOG_TRACE("Way added to map with ID " << newWay->getId());
  }

  void _noOpTests(SplitLongLinearWaysVisitor& splitVisitor)
  {
    int wayID = 0;
    int startNode;
    int numNodes;

    // 0 nodes: no-op
    _map = std::make_shared<OsmMap>();
    startNode = 1;
    numNodes = 0;
    _createWay(++wayID, startNode, startNode + numNodes - 1);

    _map->visitRw(splitVisitor);

    // Sanity checks to make sure it was no op
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes + 1), _map->getElementCount());
    CPPUNIT_ASSERT_EQUAL(true, _map->containsWay(wayID));
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes), _map->getWay(wayID)->getNodeCount());
    for ( int i = startNode; i < startNode + numNodes; ++i )
    {
      CPPUNIT_ASSERT_EQUAL(true, _map->containsNode(i));
      CPPUNIT_ASSERT_EQUAL(true, _map->getWay(wayID)->hasNode(i));
    }

    // 1 nodes: no-op
    _map = std::make_shared<OsmMap>();
    startNode = 5;
    numNodes = 1;
    _createWay(++wayID, startNode, startNode + numNodes - 1);

    _map->visitRw(splitVisitor);

    // Sanity checks to make sure it was no op
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes + 1), _map->getElementCount());
    CPPUNIT_ASSERT_EQUAL(true, _map->containsWay(wayID));
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes), _map->getWay(wayID)->getNodeCount());
    for ( int i = startNode; i < startNode + numNodes; ++i )
    {
      CPPUNIT_ASSERT_EQUAL(true, _map->containsNode(i));
      CPPUNIT_ASSERT_EQUAL(true, _map->getWay(wayID)->hasNode(i));
    }

    // 2 nodes: no-op
    _map = std::make_shared<OsmMap>();
    startNode = 10;
    numNodes = 2;
    _createWay(++wayID, startNode, startNode + numNodes - 1);

    _map->visitRw(splitVisitor);

    // Sanity checks to make sure it was no op
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes + 1), _map->getElementCount());
    CPPUNIT_ASSERT_EQUAL(true, _map->containsWay(wayID));
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes), _map->getWay(wayID)->getNodeCount());
    for ( int i = startNode; i < startNode + numNodes; ++i )
    {
      CPPUNIT_ASSERT_EQUAL(true, _map->containsNode(i));
      CPPUNIT_ASSERT_EQUAL(true, _map->getWay(wayID)->hasNode(i));
    }

    // N - 1 nodes: no-op
    _map = std::make_shared<OsmMap>();
    startNode = 1000;
    numNodes = splitVisitor.getMaxNumberOfNodes() - 1;
    _createWay(++wayID, startNode, startNode + numNodes - 1);

    _map->visitRw(splitVisitor);

    // Sanity checks to make sure it was no op
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes + 1), _map->getElementCount());
    CPPUNIT_ASSERT_EQUAL(true, _map->containsWay(wayID));
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes), _map->getWay(wayID)->getNodeCount());
    for ( int i = startNode; i < startNode + numNodes; ++i )
    {
      CPPUNIT_ASSERT_EQUAL(true, _map->containsNode(i));
      CPPUNIT_ASSERT_EQUAL(true, _map->getWay(wayID)->hasNode(i));
    }

    // N nodes: no-op
    _map = std::make_shared<OsmMap>();
    startNode = 2001;
    numNodes = splitVisitor.getMaxNumberOfNodes();
    _createWay(++wayID, startNode, startNode + numNodes - 1);

    _map->visitRw(splitVisitor);

    // Sanity checks to make sure it was no op
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes + 1), _map->getElementCount());
    CPPUNIT_ASSERT_EQUAL(true, _map->containsWay(wayID));
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(numNodes), _map->getWay(wayID)->getNodeCount());
    for ( int i = startNode; i < startNode + numNodes; ++i )
    {
      CPPUNIT_ASSERT_EQUAL(true, _map->containsNode(i));
      CPPUNIT_ASSERT_EQUAL(true, _map->getWay(wayID)->hasNode(i));
    }
  }

  void _modifyTests(SplitLongLinearWaysVisitor& splitVisitor)
  {

    int wayID = 0;
    int startNode = 234;
    int numNodes;
    int numWays;

    // N + 1 nodes: 2 ways
    _map = std::make_shared<OsmMap>();
    numNodes = splitVisitor.getMaxNumberOfNodes() + 1;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    WayMap ways = _map->getWays();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(numWays), ways.size() );
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // N + 2 nodes: 2 ways
    _map = std::make_shared<OsmMap>();
    numNodes = splitVisitor.getMaxNumberOfNodes() + 2;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    ways = _map->getWays();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(numWays), ways.size() );
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;

    // (2N) - 2 nodes: 2 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 2) - 2;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    ways = _map->getWays();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(numWays), ways.size() );
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // (2N) - 1 nodes: 2 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 2) - 1;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    ways = _map->getWays();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(numWays), ways.size() );
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // (2N) nodes: 2 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 2);
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    ways = _map->getWays();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(numWays), ways.size() );
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // (2N) + 1 nodes: 3 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 2) + 1;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    ways = _map->getWays();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(numWays), ways.size() );
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // (2N) + 2 nodes: 3 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 2) + 2;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    ways = _map->getWays();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(numWays), ways.size() );
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;



    // Picking 541N as 541 is large enough to be a good stress test, plus it's a prime number
    //    and primes are good at exposing problems

    // (541N) - 2 nodes: 541 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 541) - 2;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // (541N) - 1 nodes: 541 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 541) - 1;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // (541N) nodes: 541 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 541);
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // (541N) + 1 nodes: 542 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 541) + 1;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
    startNode += numNodes;


    // (541N) + 2 nodes: 542 ways
    _map = std::make_shared<OsmMap>();
    numNodes = (splitVisitor.getMaxNumberOfNodes() * 541) + 2;
    numWays = _calcNumWays(numNodes, splitVisitor);
    _createWay(++wayID, startNode, startNode + numNodes - 1);
    _map->visitRw(splitVisitor);

    // Sanity checks on split
    _sanityCheckSplit(splitVisitor, startNode, numNodes, numWays);
  }

  void _sanityCheckSplit(SplitLongLinearWaysVisitor& /*splitVisitor*/, const int startNode,
                         const int numNodes, const int numWays)
  {
    // Pull out ways
    WayMap ways = _map->getWays();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(numWays), ways.size() );

    // Pull out nodes
    NodeMap nodes = _map->getNodes();
    CPPUNIT_ASSERT_EQUAL( numNodes, (int)nodes.size() );

    // Make sure no relations
    RelationMap relations = _map->getRelations();
    CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), relations.size() );

    unsigned int nodesLeftToFind = numNodes;
    unsigned int searchId = startNode;
    while ( nodesLeftToFind > 0 )
    {
      LOG_TRACE("Looking for node ID " << searchId);
      bool madeProgress = false;
      bool hitError = false;
      for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
      {
        // Does this way have the node we're looking for?
        WayPtr currWay = it->second;

        if ( currWay->getFirstNodeId() == searchId )
        {
          nodesLeftToFind--;
          LOG_TRACE("Found node ID " << searchId << " at start of way " <<
                     currWay->getId());
          madeProgress = true;
          // Make sure rest of nodes we want exist and are in correct order
          searchId++;
          std::vector<long> wayIds = currWay->getNodeIds();

          // Start at second node, since we already checked first one
          for ( std::vector<long>::const_iterator nodeIt = wayIds.begin() + 1;
            nodeIt != wayIds.end(); ++nodeIt )
          {
             if ( *nodeIt != searchId )
             {
               // Throw a hissy fit
               hitError = true;
               break;
             }

             nodesLeftToFind--;
             searchId++;
          }
          searchId--;
          // Search count is off by one
          if ( nodesLeftToFind > 0 )
          {
            nodesLeftToFind++;
          }

          LOG_TRACE("Found remainder of IDs up to " << searchId << " inside way");
          LOG_TRACE("Nodes left to find: " << nodesLeftToFind);

          // We found what we needed, bail out of looking for more ways
          break;
        }
        else
        {
          LOG_TRACE("Way started with ID " << currWay->getFirstNodeId() << ", skipping");
        }
      }

      CPPUNIT_ASSERT( (madeProgress == true) && (hitError == false) );
    }
  }

  unsigned int _calcNumWays( const int numNodes, SplitLongLinearWaysVisitor& splitVisitor )
  {
    unsigned int nodesRemaining = numNodes;
    unsigned int numWays = 0;
    long maxInWay = std::min(nodesRemaining, splitVisitor.getMaxNumberOfNodes());

    // Take first chunk off, can hold full amount
    nodesRemaining -= maxInWay;
    numWays++;

    // For everything else, can only hold N-1 unique as first has to match last from previous way
    if ( nodesRemaining > 0 )
    {
      maxInWay--;

      numWays += (nodesRemaining / maxInWay);

      // Remainder means we need one more way to hold leftover nodes
      if ( nodesRemaining % maxInWay != 0 )
      {
        numWays++;
      }
    }

    return numWays;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SplitLongLinearWaysVisitorTest, "slow");

}
