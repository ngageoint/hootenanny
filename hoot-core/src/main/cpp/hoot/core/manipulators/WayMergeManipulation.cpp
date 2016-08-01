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

#include "WayMergeManipulation.h"

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/MaximalNearestSubline.h>
#include <hoot/core/algorithms/ProbabilityOfMatch.h>
#include <hoot/core/algorithms/WayAverager.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDebug>

// Tgs
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

double epsilon = 1;

WayMergeManipulation::WayMergeManipulation(long leftId, long rightId, shared_ptr<const OsmMap> map,
  Meters minSplitSize)
{
  _left = leftId;
  _right = rightId;
  _minSplitSize = minSplitSize;

  updateEstimate(map);
}

void WayMergeManipulation::addBogusReviewTags(const OsmMapPtr& map) const
{
  assert(isValid(map));

  ElementPtr left = map->getWay(_left);
  ElementPtr right = map->getWay(_right);

  QString note("The review scores on this way are bogus. See #3242.");
  ReviewMarker().mark(map, left, right, note, "Bogus", getBogusReviewScore());
}

void WayMergeManipulation::applyManipulation(shared_ptr<OsmMap> map,
  set<ElementId> &impactedElements, set<ElementId> &newElements) const
{
  shared_ptr<OsmMap> result = map;

  // insert the impacted ways
  impactedElements = getImpactedElementIds(map);
  impactedElements.erase(ElementId::way(_left));
  impactedElements.erase(ElementId::way(_right));

  // remove any ways that spanned the left & right
  _removeSpans(result, impactedElements);

  shared_ptr<Way> left = result->getWay(_left);
  shared_ptr<Way> right = result->getWay(_right);

  Meters minSplitSize = _minSplitSize;
  minSplitSize = min(minSplitSize, ElementConverter(map).convertToLineString(left)->getLength() * .7);
  minSplitSize = min(minSplitSize, ElementConverter(map).convertToLineString(right)->getLength() * .7);

  // split left into its maximal nearest sublines
  MaximalNearestSubline mns1(map, left, right, minSplitSize,
    left->getCircularError() + right->getCircularError());
  int mnsLeftIndex;
  vector< shared_ptr<Way> > splitsLeft = mns1.splitWay(result, mnsLeftIndex);
  assert(splitsLeft.size() != 0);
  shared_ptr<Way> mnsLeft = splitsLeft[mnsLeftIndex];

  // split right into its maximal nearest sublines
  MaximalNearestSubline mns2(map, right, mnsLeft, minSplitSize,
    left->getCircularError() + right->getCircularError());
  int mnsRightIndex;
  vector< shared_ptr<Way> > splitsRight = mns2.splitWay(result, mnsRightIndex);
  assert(splitsRight.size() != 0);
  shared_ptr<Way> mnsRight = splitsRight[mnsRightIndex];

  for (size_t i = 0; i < splitsLeft.size(); i++)
  {
    if ((int)i != mnsLeftIndex)
    {
      newElements.insert(ElementId::way(splitsLeft[i]->getId()));
    }
    result->addWay(splitsLeft[i]);
  }

  for (size_t i = 0; i < splitsRight.size(); i++)
  {
    if ((int)i != mnsRightIndex)
    {
      newElements.insert(ElementId::way(splitsRight[i]->getId()));
    }
    result->addWay(splitsRight[i]);
  }

  // average the two MNSs
  shared_ptr<Way> w = WayAverager::average(map, mnsRight, mnsLeft);
  w->setStatus(Status::Conflated);
  result->addWay(w);

  // insert the new merged way
  newElements.insert(ElementId::way(w->getId()));

  RemoveWayOp::removeWay(result, _left);
  RemoveWayOp::removeWay(result, _right);

  for (set<ElementId>::iterator it = impactedElements.begin(); it != impactedElements.end(); it++)
  {
    if (result->containsElement(*it) == false)
    {
      LOG_ERROR("" << "Internal error: bad way " << it->toString());
    }
  }
}

double WayMergeManipulation::calculateProbability(shared_ptr<const OsmMap> map) const
{
  return _calculateExpertProbability(map);
}

double WayMergeManipulation::calculateScore(shared_ptr<const OsmMap> map) const
{
  assert(isValid(map));

  _p = calculateProbability(map);

  return _p;
}

double WayMergeManipulation::_calculateExpertProbability(shared_ptr<const OsmMap> map) const
{
  OsmMapPtr theMap(new OsmMap());
  CopySubsetOp(map,
               ElementId(ElementType::Way, _left),
               ElementId(ElementType::Way, _right)).apply(theMap);

  shared_ptr<Way> left = theMap->getWay(_left);
  shared_ptr<Way> right = theMap->getWay(_right);

  // use the maximal nearest subline code to find the best subline
  shared_ptr<Way> mnsLeft = MaximalNearestSubline::getMaximalNearestSubline(theMap, left, right,
    _minSplitSize, left->getCircularError() + right->getCircularError());
  if (mnsLeft == 0)
  {
    return 0.0;
  }
  shared_ptr<Way> mnsRight = MaximalNearestSubline::getMaximalNearestSubline(theMap, right, mnsLeft,
    _minSplitSize, left->getCircularError() + right->getCircularError());
  if (mnsRight == 0)
  {
    return 0.0;
  }

  // what portion of the original lines is the MNS
  double pl = ElementConverter(theMap).convertToLineString(mnsLeft)->getLength() /
      ElementConverter(theMap).convertToLineString(left)->getLength();
  double pr = ElementConverter(theMap).convertToLineString(mnsRight)->getLength() /
      ElementConverter(theMap).convertToLineString(right)->getLength();

  // give it a score
  double ps = std::min(pl, pr) / 2.0 + 0.5;

  double p;

  // if either of the lines are zero in length.
  if (pl == 0 || pr == 0)
  {
    p = 0.0;
  }
  else
  {
    p = ps * ProbabilityOfMatch::getInstance().expertProbability(theMap, mnsLeft, mnsRight);
  }

  return p;
}

bool WayMergeManipulation::_directConnect(const OsmMapPtr& map, shared_ptr<Way> w) const
{
  shared_ptr<LineString> ls = ElementConverter(map).convertToLineString(w);

  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->
    create(2, 2);

  cs->setAt(map->getNode(w->getNodeId(0))->toCoordinate(), 0);
  cs->setAt(map->getNode(w->getLastNodeId())->toCoordinate(), 1);

  // create a straight line and buffer it
  shared_ptr<LineString> straight(GeometryFactory::getDefaultInstance()->createLineString(cs));
  shared_ptr<Geometry> g(straight->buffer(w->getCircularError()));

  // is the way in question completely contained in the buffer?
  return g->contains(ls.get());
}

bool WayMergeManipulation::isValid(shared_ptr<const OsmMap> map) const
{
  bool result = false;

  if (map->containsWay(_left) && map->containsWay(_right))
  {
    result = map->getWay(_left)->isUnknown();
    result = result && map->getWay(_right)->isUnknown();
  }

  return result;
}

const set<ElementId>& WayMergeManipulation::getImpactedElementIds(const ConstOsmMapPtr& map) const
{
  _impactedElements.clear();
  const set<long>& wids = getImpactedWayIds(map);

  for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
  {
    _impactedElements.insert(ElementId(ElementType::Way, *it));
  }

  return _impactedElements;
}

const set<long>& WayMergeManipulation::getImpactedWayIds(const ConstOsmMapPtr& map) const
{
  _impactedWays.clear();
  _impactedWays.insert(_left);
  _impactedWays.insert(_right);

  NodeToWayMap& n2w = *map->getIndex().getNodeToWayMap();

  shared_ptr<const Way> left = map->getWay(_left);
  shared_ptr<const Way> right = map->getWay(_right);

  const set<long>& s1 = n2w.at(left->getNodeId(0));
  _impactedWays.insert(s1.begin(), s1.end());
  const set<long>& s2 = n2w.at(left->getLastNodeId());
  _impactedWays.insert(s2.begin(), s2.end());

  const set<long>& s3 = n2w.at(right->getNodeId(0));
  _impactedWays.insert(s3.begin(), s3.end());
  const set<long>& s4 = n2w.at(right->getLastNodeId());
  _impactedWays.insert(s4.begin(), s4.end());

  return _impactedWays;
}

set<ElementId> WayMergeManipulation::getMatchedElements() const
{
  set<ElementId> result;
  result.insert(ElementId(ElementType::Way, _left));
  result.insert(ElementId(ElementType::Way, _right));
  return result;
}

void WayMergeManipulation::_removeSpans(shared_ptr<OsmMap> map,
  set<ElementId>& impactedElements) const
{
  shared_ptr<Way> left = map->getWay(_left);
  shared_ptr<Way> right = map->getWay(_right);

  set<ElementId> impactedWaysTmp = impactedElements;
  for (set<ElementId>::iterator it = impactedWaysTmp.begin(); it != impactedWaysTmp.end(); it++)
  {
    ElementId eid = *it;
    shared_ptr<Way> w = map->getWay(eid.getId());
    long first = w->getNodeId(0);
    long last = w->getLastNodeId();
    if ((left->hasNode(first) && right->hasNode(last)) ||
        (left->hasNode(last) && right->hasNode(first)))
    {
      if (_directConnect(map, w))
      {
        RemoveWayOp::removeWay(map, eid.getId());
        impactedElements.erase(eid);
      }
    }
  }
}

void WayMergeManipulation::_splitWays(shared_ptr<OsmMap> map, shared_ptr<Way>& left,
  shared_ptr<Way>& right) const
{
  shared_ptr<OsmMap> result = map;

  // insert the impacted ways
  set<ElementId> impactedElements = getImpactedElementIds(map);
  impactedElements.erase(ElementId::way(_left));
  impactedElements.erase(ElementId::way(_right));

  // remove any ways that spanned the left & right
  _removeSpans(result, impactedElements);

  left = result->getWay(_left);
  right = result->getWay(_right);

  Meters minSplitSize = _minSplitSize;
  minSplitSize = min(minSplitSize, ElementConverter(map).convertToLineString(left)->getLength() * .7);
  minSplitSize = min(minSplitSize, ElementConverter(map).convertToLineString(right)->getLength() * .7);

  // split left into its maximal nearest sublines
  MaximalNearestSubline mns1(result, left, right, minSplitSize,
    left->getCircularError() + right->getCircularError());
  int mnsLeftIndex;
  vector< shared_ptr<Way> > splitsLeft = mns1.splitWay(result, mnsLeftIndex);
  assert(splitsLeft.size() != 0);
  shared_ptr<Way> mnsLeft = splitsLeft[mnsLeftIndex];

  // split right into its maximal nearest sublines
  MaximalNearestSubline mns2(result, right, mnsLeft, minSplitSize,
    left->getCircularError() + right->getCircularError());
  int mnsRightIndex;
  vector< shared_ptr<Way> > splitsRight = mns2.splitWay(result, mnsRightIndex);
  assert(splitsRight.size() != 0);
  shared_ptr<Way> mnsRight = splitsRight[mnsRightIndex];

  for (size_t i = 0; i < splitsLeft.size(); i++)
  {
    if (splitsLeft[i] != left)
    {
      result->addWay(splitsLeft[i]);
    }
  }

  for (size_t i = 0; i < splitsRight.size(); i++)
  {
    if (splitsRight[i] != right)
    {
      result->addWay(splitsRight[i]);
    }
  }

  RemoveWayOp::removeWay(result, _left);
  RemoveWayOp::removeWay(result, _right);

  for (set<ElementId>::iterator it = impactedElements.begin(); it != impactedElements.end(); it++)
  {
    if (result->containsElement(*it) == false)
    {
      LOG_ERROR("" << "Internal error: bad way " << it->toString());
    }
  }
  left = mnsLeft;
  right = mnsRight;
}

QString WayMergeManipulation::toString() const
{
  return QString("WayMergeManipulation left: %1 right: %2 score: %3 p: %4").arg(_left).arg(_right).
      arg(getScoreEstimate()).arg(_p);
}

}
