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
#include "WaySnapMergeManipulation.h"

// geos
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/MaximalNearestSubline.h>
#include <hoot/core/algorithms/MaximalSubline.h>
#include <hoot/core/algorithms/WayAverager.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

WaySnapMergeManipulation::WaySnapMergeManipulation(long leftId, long rightId,
  shared_ptr<const OsmMap> map, Meters minSplitSize) :
  WayMergeManipulation(leftId, rightId, map, minSplitSize)
{
}

void WaySnapMergeManipulation::applyManipulation(shared_ptr<OsmMap> map,
  set<ElementId>& impactedElements, set<ElementId>& newElements) const
{
  shared_ptr<OsmMap> result = map;

  // insert the impacted ways
  impactedElements = getImpactedElementIds(map);
  impactedElements.erase(ElementId::way(_left));
  impactedElements.erase(ElementId::way(_right));

  // remove any ways that spanned the left & right
  _removeSpans(result, impactedElements);

  shared_ptr<Way> w1 = result->getWay(_left);
  shared_ptr<Way> w2 = result->getWay(_right);

  // make sure w1 is the Unknown1
  if (w1->getStatus() != Status::Unknown1)
  {
    assert(w2->getStatus() == Status::Unknown1);
    swap(w1, w2);
  }

  if (!DirectionFinder::isSimilarDirection(map, w1, w2))
  {
    w2->reverseOrder();
  }

  Meters minSplitSize = _minSplitSize;
  minSplitSize = min(minSplitSize, ElementConverter(map).convertToLineString(w1)->getLength() * .7);
  minSplitSize = min(minSplitSize, ElementConverter(map).convertToLineString(w2)->getLength() * .7);

  // split left into its maximal nearest sublines
  MaximalNearestSubline mnser1(result, w1, w2, minSplitSize,
    w1->getCircularError() + w2->getCircularError());
  int mns1Index;
  vector< shared_ptr<Way> > splits1 = mnser1.splitWay(result, mns1Index);
  assert(splits1.size() != 0);
  shared_ptr<Way> mns1 = splits1[mns1Index];

  // split right into its maximal nearest sublines
  MaximalNearestSubline mnser2(result, w2, mns1, minSplitSize,
    w1->getCircularError() + w2->getCircularError());
  int mns2Index;
  vector< shared_ptr<Way> > splits2 = mnser2.splitWay(result, mns2Index);
  assert(splits2.size() != 0);
  shared_ptr<Way> mns2 = splits2[mns2Index];

  for (size_t i = 0; i < splits1.size(); i++)
  {
    newElements.insert(ElementId::way(splits1[i]->getId()));
    result->addWay(splits1[i]);
  }

  for (size_t i = 0; i < splits2.size(); i++)
  {
    if ((int)i != mns2Index)
    {
      newElements.insert(ElementId::way(splits2[i]->getId()));
      result->addWay(splits2[i]);
      // identify and snap the ends of any split ways that should be snapped to the first way
      _snapEnds(splits2[i], splits2[mns2Index], splits1[mns1Index]);
    }
  }

  // merge the attributes appropriately
  Tags newTags = TagMergerFactory::mergeTags(w1->getTags(), w2->getTags(), ElementType::Way);
  splits1[mns1Index]->setTags(newTags);
  splits1[mns1Index]->setStatus(Status::Conflated);

  // remove the old way that was split and snapped
  if (w1 != splits1[mns1Index])
  {
    RemoveWayOp::removeWay(result, w1->getId());
  }
  RemoveWayOp::removeWay(result, w2->getId());

  for (set<ElementId>::iterator it = impactedElements.begin(); it != impactedElements.end(); it++)
  {
    if (result->containsElement(*it) == false)
    {
      LOG_ERROR("" << "Internal error: bad way " << it->toString());
    }
  }
}

const set<long>& WaySnapMergeManipulation::getImpactedWayIds(shared_ptr<const OsmMap> /*map*/) const
{
  _impactedWays.clear();
  _impactedWays.insert(_left);
  _impactedWays.insert(_right);
  return _impactedWays;
}

void WaySnapMergeManipulation::_snapEnds(WayPtr snapee, WayPtr middle, WayPtr snapTo) const
{
  if (snapee->getNodeId(0) == middle->getNodeId(0))
  {
    vector<long> nodes = snapee->getNodeIds();
    nodes[0] = snapTo->getNodeId(0);
    snapee->setNodes(nodes);
  }
  else if (snapee->getNodeId(0) == middle->getLastNodeId())
  {
    vector<long> nodes = snapee->getNodeIds();
    nodes[0] = snapTo->getLastNodeId();
    snapee->setNodes(nodes);
  }

  if (snapee->getLastNodeId() == middle->getNodeId(0))
  {
    vector<long> nodes = snapee->getNodeIds();
    nodes[nodes.size() - 1] = snapTo->getNodeId(0);
    snapee->setNodes(nodes);
  }
  else if (snapee->getLastNodeId() == middle->getLastNodeId())
  {
    vector<long> nodes = snapee->getNodeIds();
    nodes[nodes.size() - 1] = snapTo->getLastNodeId();
    snapee->setNodes(nodes);
  }
}


}
