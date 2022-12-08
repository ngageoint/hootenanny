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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "RandomWaySplitter.h"

// boost
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_int.hpp>

// hoot
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/RandomNumberUtils.h>

// Tgs
#include <tgs/System/Time.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RandomWaySplitter)

RandomWaySplitter::RandomWaySplitter()
  : _localRng(std::make_shared<boost::minstd_rand>()),
    _splitRecursionLevel(0)
{
  _rng = _localRng.get();
}

void RandomWaySplitter::setOsmMap(OsmMap* map)
{
  _map = map;
  MapProjector::projectToPlanar(_map->shared_from_this());
}

void RandomWaySplitter::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setWaySplitProbability(configOptions.getRandomWaySplitterProbability());
  setMinNodeSpacing(configOptions.getRandomWaySplitterMinNodeSpacing());
  const int seed = configOptions.getRandomSeed();
  LOG_VARD(seed);
  if (seed == -1)
    _rng->seed(RandomNumberUtils::generateSeed());
  else
    _rng->seed(seed);
}

void RandomWaySplitter::visit(const std::shared_ptr<Element>& e)
{
  if (!_map)
    throw IllegalArgumentException("No map passed to way splitter.");
  else if (_map->getProjection()->IsGeographic())
    throw IllegalArgumentException("Input map must be projected to planar.");

  LOG_TRACE(e->getElementType());
  if (HighwayCriterion().isSatisfied(e))
    _split(e);
}

vector<ElementPtr> RandomWaySplitter::_split(ElementPtr element)
{
  // randomly select elements and split them into two parts

  boost::uniform_real<> randomSplitDistribution(0.0, 1.0);
  const double randomSplitNum = randomSplitDistribution(*_rng);
  if (randomSplitNum <= _waySplitProbability)
  {
    LOG_TRACE("element " << element->getElementId() << " *will* be split based on a split " <<
      "probability of: " <<  _waySplitProbability << " and a randomly generated number: " <<
      randomSplitNum << "\n");

    _splitRecursionLevel++;
    LOG_VART(_splitRecursionLevel);

    const int numNodesBeforeSplit = static_cast<int>(_map->getNodeCount());
    LOG_VART(numNodesBeforeSplit);
    const int numWaysBeforeSplit = static_cast<int>(_map->getWayCount());
    LOG_VART(numWaysBeforeSplit);

    WayLocation waySplitPoint;
    MultiLineStringLocation multiLineSplitPoint;
    QList<long> nodeIdsBeforeSplit;
    int segmentIndex = -1;
    ElementId wayId;
    // determine where to split the element
    if (element->getElementType() == ElementType::Way)
    {
      WayPtr way = std::dynamic_pointer_cast<Way>(element);
      LOG_VART(way->getNodeCount());
      nodeIdsBeforeSplit = QVector<long>::fromStdVector(way->getNodeIds()).toList();
      LOG_VART(nodeIdsBeforeSplit);
      waySplitPoint = _calcSplitPoint(way);
    }
    else
    {
      multiLineSplitPoint = _calcSplitPoint(std::dynamic_pointer_cast<Relation>(element), wayId);
      waySplitPoint = multiLineSplitPoint.getWayLocation();
    }

    const QString distanceMsgStrEnd = QString("a minimum node spacing of %1 meters").arg(QString::number(_minNodeSpacing));
    if (!waySplitPoint.isValid())
    {
      _splitRecursionLevel--;
      LOG_VART(_splitRecursionLevel);

      LOG_TRACE("split point *will not* be used because *it violates* " << distanceMsgStrEnd);
      //if it violates the min node spacing, return an empty element collection, which will end the
      //recursive splitting on the current way
      return vector<ElementPtr>();
    }
    else
    {
      LOG_TRACE("split point *will* be used because it *does not* violate " << distanceMsgStrEnd);
      segmentIndex = waySplitPoint.getSegmentIndex();
      LOG_VART(segmentIndex);
    }

    // split the element

    vector<ElementPtr> newElementsAfterSplit;
    if (element->getElementType() == ElementType::Way)
    {
      vector<WayPtr> newWaysAfterSplit =
        WaySplitter::split(_map->shared_from_this(), std::dynamic_pointer_cast<Way>(element), waySplitPoint);
      for (const auto& way : newWaysAfterSplit)
        newElementsAfterSplit.push_back(way);
    }
    else
    {
      ElementPtr match;
      MultiLineStringSplitter().split(_map->shared_from_this(), multiLineSplitPoint, match);
      newElementsAfterSplit.push_back(match);
    }

    const int numNodesAfterSplit = static_cast<int>(_map->getNodeCount());
    LOG_VART(numNodesAfterSplit);
    const int numNewNodesCreatedBySplit = numNodesAfterSplit - numNodesBeforeSplit;
    LOG_VART(numNewNodesCreatedBySplit);
    LOG_VART(_map->getWayCount());

    if (numNewNodesCreatedBySplit > 0)
    {
      WayPtr way = std::dynamic_pointer_cast<Way>(element);
      //Its possible that the splitting of a relation could generate a new node.  In that case,
      //_updateNewNodeProperties does not need to be called b/c the MultiLineStringSplitter has
      //already properly updated the new node's properties.  when a way is split, however, the
      //new node's properties must be updated by the call to _updateNewNodeProperties.
      if (way != nullptr)
      {
        assert(nodeIdsBeforeSplit.size() > 0);
        //update properties on any nodes added as a result of the way splitting (new ways created as a
        //result of the splitting will already have had their parent's tags added by WaySplitter)
        _updateNewNodeProperties(
          _getNodeAddedBySplit(nodeIdsBeforeSplit, newElementsAfterSplit),
          _map->getNode(way->getNodeId(segmentIndex)),
          _map->getNode(way->getNodeId(segmentIndex + 1)));
      }
    }

    // recursive call
    for (auto it = newElementsAfterSplit.begin(); it != newElementsAfterSplit.end(); ++it)
      _split(*it);

    return newElementsAfterSplit;
  }
  else
  {
    LOG_TRACE("element " << element->getElementId() << " *will not* be split based on a split " <<
      "probability of: " << _waySplitProbability << " and a randomly generated number: " <<
      randomSplitNum << "\n");
  }

  //end the recursive splitting on the current way
  return vector<ElementPtr>();
}

WayLocation RandomWaySplitter::_calcSplitPoint(ConstWayPtr way) const
{
  //create a way location that is the minimum node spacing distance from the beginning of the way
  WayLocation splitWayStart(_map->shared_from_this(), way, _minNodeSpacing);
  //create a way location that is the minimum node spacing from the end of the way
  WayLocation splitWayEnd = WayLocation::createAtEndOfWay(_map->shared_from_this(), way).move(-1 * _minNodeSpacing);
  //if the length between the way locations is greater than zero, then a way location can be
  //selected that doesn't violate the min node spacing
  const double splitWayLength = splitWayEnd.calculateDistanceOnWay() - splitWayStart.calculateDistanceOnWay();
  LOG_VART(splitWayLength);
  if (splitWayLength > 0)
  {
    boost::uniform_real<> randomSplitPointDistribution(0.0, splitWayLength);
    const double splitPoint = randomSplitPointDistribution(*_rng);
    LOG_VART(splitPoint);
    return splitWayStart.move(splitPoint);
  }
  else  //  otherwise, return an empty location
    return WayLocation();
}

MultiLineStringLocation RandomWaySplitter::_calcSplitPoint(ConstRelationPtr relation, ElementId& wayId) const
{
  const vector<RelationData::Entry>& members = relation->getMembers();
  LOG_VART(members.size());

  //find the way to split on
  boost::uniform_int<> randomWayIndexDistribution(0, static_cast<int>(members.size()) - 1);
  int wayIndex = randomWayIndexDistribution(*_rng);
  wayId = members.at(wayIndex).getElementId();
  LOG_VART(wayIndex);
  LOG_VART(wayId);
  ElementPtr element = _map->getElement(wayId);
  if (element->getElementType() != ElementType::Way)
  {
    throw HootException(
      "PERTY feature splitting for multi-line string relations may only occur on relations "
      "which contain only ways.");
  }
  WayPtr way = std::dynamic_pointer_cast<Way>(element);
  LOG_VART(way->getNodeCount());

  //calculate the split point
  WayLocation wayLocation = _calcSplitPoint(way);
  //return it, if its valid
  if (wayLocation.isValid())
    return MultiLineStringLocation(_map->shared_from_this(), relation, wayIndex, wayLocation);
  else  //otherwise, return an empty location
    return MultiLineStringLocation();
}

NodePtr RandomWaySplitter::_getNodeAddedBySplit(const QList<long>& nodeIdsBeforeSplit,
                                                const vector<ElementPtr>& newElementsAfterSplit) const
{
  //newElementsAfterSplit is assumed to only contain ways; find the new node created by the way
  //split; it will be the last node in the first way, which is the same as the first node in the
  //last way
  ConstWayPtr firstWay = std::dynamic_pointer_cast<Way>(newElementsAfterSplit.at(0));
  const long lastNodeIdInFirstWay = firstWay->getNodeIds().at(firstWay->getNodeCount() - 1);
  LOG_VART(lastNodeIdInFirstWay);
  ConstWayPtr lastWay = std::dynamic_pointer_cast<Way>(newElementsAfterSplit.at(1));
  const long firstNodeIdInLastWay = lastWay->getNodeIds().at(0);
  LOG_VART(firstNodeIdInLastWay);
  assert(lastNodeIdInFirstWay == firstNodeIdInLastWay);
  assert(!nodeIdsBeforeSplit.contains(lastNodeIdInFirstWay));
  LOG_VART(nodeIdsBeforeSplit);
  return _map->getNode(firstNodeIdInLastWay);
}

void RandomWaySplitter::_updateNewNodeProperties(NodePtr newNode,
                                                 ConstNodePtr firstSplitBetweenNode,
                                                 ConstNodePtr lastSplitBetweenNode) const
{
  //arbitrarily copy the status from one split between node to the new node
  newNode->setStatus(firstSplitBetweenNode->getStatus());
  //add a circular error to the new node that's a weighted average of the circular error on the
  //two split between nodes
  newNode->setCircularError((firstSplitBetweenNode->getCircularError() + lastSplitBetweenNode->getCircularError()) / 2);
  LOG_TRACE("Updated the properties of a node created as a result of a way split: " << newNode->toString());
}

}
