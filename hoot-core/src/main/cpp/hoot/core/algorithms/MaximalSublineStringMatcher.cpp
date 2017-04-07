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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MaximalSublineStringMatcher.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatch.h>
#include <hoot/core/algorithms/MaximalSublineMatcher.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ExtractWaysVisitor.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/algorithms/SublineMatcher.h>

// Standard
#include <algorithm>

#include "MaximalSubline.h"

namespace hoot
{

unsigned int MaximalSublineStringMatcher::logWarnCount = 0;

HOOT_FACTORY_REGISTER(SublineStringMatcher, MaximalSublineStringMatcher)

MaximalSublineStringMatcher::MaximalSublineStringMatcher()
{
  setConfiguration(conf());
}

vector<WayPtr> MaximalSublineStringMatcher::_changeMap(const vector<ConstWayPtr>& ways,
  OsmMapPtr map) const
{
  vector<WayPtr> result;
  result.reserve(ways.size());
  for (size_t i = 0; i < ways.size(); ++i)
  {
    result.push_back(map->getWay(ways[i]->getId()));
  }

  return result;
}

void MaximalSublineStringMatcher::_configureSublineMatcher()
{
  if (!_sublineMatcher)
  {
    _sublineMatcher.reset(new MaximalSublineMatcher());
  }
  _sublineMatcher->setMaxRelevantAngle(_maxAngle);
  _sublineMatcher->setMinSplitSize(_minSplitsize);
  _sublineMatcher->setHeadingDelta(_headingDelta);
}

WaySublineMatchString MaximalSublineStringMatcher::findMatch(const ConstOsmMapPtr& map,
  const ConstElementPtr& e1, const ConstElementPtr& e2, Meters maxRelevantDistance) const
{
  assert(_maxAngle >= 0);
  if (maxRelevantDistance == -1)
  {
    maxRelevantDistance = e1->getCircularError() + e2->getCircularError();
  }

  // make sure the inputs are legit. If either element isn't legit then throw a NeedsReviewException
  _validateElement(map, e1->getElementId());
  _validateElement(map, e2->getElementId());

  // extract the ways from the elements. In most cases it will return a vector of 1, but
  // multilinestrings may contain multiple ways
  vector<ConstWayPtr> ways1 = ExtractWaysVisitor::extractWays(map, e1);
  vector<ConstWayPtr> ways2 = ExtractWaysVisitor::extractWays(map, e2);

  if ((ways1.size() > 4 && ways2.size() > 4) || (ways1.size() + ways2.size() > 7))
  {
    throw NeedsReviewException("Elements contain too many ways and the computational complexity "
                               "is unreasonable.");
  }

  // Try with all combinations of forward and reversed ways. This is very expensive for
  // multilinestrings with lots of ways in them. Though those shouldn't be common.
  vector<bool> reversed1(ways1.size(), false), reversed2(ways2.size(), false);
  ScoredMatch scoredResult = _findBestMatch(map, maxRelevantDistance, ways1, ways2, reversed1,
    reversed2);

  // convert the best match into a WaySublineStringMatch and return.
  try
  {
    WaySublineMatchString result = scoredResult.matches;
    // this likely shouldn't be necessary. See https://github.com/ngageoint/hootenanny/issues/157.
    result.removeEmptyMatches();
    return result;
  }
  catch(OverlappingMatchesException &e)
  {
    throw NeedsReviewException("Internal Error: Multiple overlapping way matches were found within "
      "one set of ways.  Please report this to hootenanny.help@digitalglobe.com.");
  }
}

MaximalSublineStringMatcher::ScoredMatch MaximalSublineStringMatcher::_evaluateMatch(
  const ConstOsmMapPtr &map, Meters maxDistance, const vector<ConstWayPtr>& ways1,
  const vector<ConstWayPtr> &ways2, const vector<bool>& reversed1,
  const vector<bool> &reversed2) const
{
  vector<WaySublineMatch> matches;

  // make a copy of the map and the ways we need so we can reverse the ways as needed.
  set<ElementId> eids;
  _insertElementIds(ways1, eids);
  _insertElementIds(ways2, eids);
  OsmMapPtr copiedMap(new OsmMap(map->getProjection()));
  CopySubsetOp(map, eids).apply(copiedMap);

  vector<WayPtr> prep1 = _changeMap(ways1, copiedMap);
  vector<WayPtr> prep2 = _changeMap(ways2, copiedMap);

  // reversed ways as appropriate
  _reverseWays(prep1, reversed1);
  _reverseWays(prep2, reversed2);

  double scoreSum = 0;

  // go through and match each way against every other way
  for (size_t i = 0; i < prep1.size(); i++)
  {
    for (size_t j = 0; j < prep2.size(); j++)
    {
      double score;
      WaySublineMatchString m = _sublineMatcher->findMatch(copiedMap, prep1[i], prep2[j], score,
        maxDistance);

      scoreSum += score;
      matches.insert(matches.end(), m.getMatches().begin(), m.getMatches().end());
    }
  }

  HashMap<long, bool> wayIdToReversed1, wayIdToReversed2;
  // create a map from way id to reverse status
  for (size_t i = 0; i < prep1.size(); i++)
  {
    wayIdToReversed1[prep1[i]->getId()] = reversed1[i];
  }
  for (size_t i = 0; i < prep2.size(); i++)
  {
    wayIdToReversed2[prep2[i]->getId()] = reversed2[i];
  }

  // go through all the matches
  for (size_t i = 0; i < matches.size(); i++)
  {
    WaySubline ws1, ws2;
    // if the direction is reversed on one but not both ways then mark the match as reversed.
    long m1Id = matches[i].getSubline1().getStart().getWay()->getId();
    long m2Id = matches[i].getSubline2().getStart().getWay()->getId();

    if (wayIdToReversed1[m1Id])
    {
      // make sure the way subline is pointed to the right way (not reversed)
      ConstWayPtr w = map->getWay(matches[i].getSubline1().getElementId());
      ws1 = matches[i].getSubline1().reverse(w);
    }
    else
    {
      ws1 = WaySubline(matches[i].getSubline1(), map);
    }

    if (wayIdToReversed2[m2Id])
    {
      // make sure the way subline is pointed to the right way (not reversed)
      ConstWayPtr w = map->getWay(matches[i].getSubline2().getElementId());
      ws2 = matches[i].getSubline2().reverse(w);
    }
    else
    {
      ws2 = WaySubline(matches[i].getSubline2(), map);
    }

    if (wayIdToReversed1[m1Id] != wayIdToReversed2[m2Id])
    {
      matches[i] = WaySublineMatch(ws1, ws2, true);
    }
    else
    {
      matches[i] = WaySublineMatch(ws1, ws2, false);
    }
  }

  return ScoredMatch(scoreSum, matches);
}

MaximalSublineStringMatcher::ScoredMatch MaximalSublineStringMatcher::_findBestMatch(
  const ConstOsmMapPtr& map, Meters maxDistance, vector<ConstWayPtr>& ways1,
  vector<ConstWayPtr> &ways2, vector<bool>& reversed1, vector<bool> &reversed2, size_t i, size_t j)
  const
{
  const double epsilon = 1e-2;

  if ((i == ways1.size() || ways1.size() == 1) && j == ways2.size())
  {
    return _evaluateMatch(map, maxDistance, ways1, ways2, reversed1, reversed2);
  }
  else if (j == ways2.size())
  {
    reversed1[i] = true;
    ScoredMatch r1 = _findBestMatch(map, maxDistance, ways1, ways2, reversed1, reversed2, i + 1, j);
    reversed1[i] = false;
    ScoredMatch r2 = _findBestMatch(map, maxDistance, ways1, ways2, reversed1, reversed2, i + 1, j);

    // only keep the reverse if it is significantly better
    if (r1.score - r2.score > epsilon)
    {
      return r1;
    }
    else
    {
      return r2;
    }
  }
  else
  {
    reversed2[j] = true;
    ScoredMatch r1 = _findBestMatch(map, maxDistance, ways1, ways2, reversed1, reversed2, i, j + 1);
    reversed2[j] = false;
    ScoredMatch r2 = _findBestMatch(map, maxDistance, ways1, ways2, reversed1, reversed2, i, j + 1);

    // only keep the reverse if it is significantly better
    if (r1.score - r2.score > epsilon)
    {
      return r1;
    }
    else
    {
      return r2;
    }
  }
}

void MaximalSublineStringMatcher::_insertElementIds(const vector<ConstWayPtr>& ways,
  set<ElementId>& elements) const
{
  for (size_t i = 0; i < ways.size(); ++i)
  {
    elements.insert(ways[i]->getElementId());
  }
}

void MaximalSublineStringMatcher::_reverseWays(const vector<WayPtr> &ways,
  const vector<bool>& reversed) const
{
  for (size_t i = 0; i < ways.size(); i++)
  {
    if (reversed[i])
    {
      ways[i]->reverseOrder();
    }
  }
}

void MaximalSublineStringMatcher::setConfiguration(const Settings& s)
{
  ConfigOptions co(s);
  setMaxRelevantAngle(toRadians(co.getWayMatcherMaxAngle()));
  setMinSplitSize(co.getWayMergerMinSplitSize());
  setHeadingDelta(co.getWayMatcherHeadingDelta());

  _sublineMatcher.reset(
    Factory::getInstance().constructObject<SublineMatcher>(co.getWaySublineMatcher()));
  _configureSublineMatcher();
}

void MaximalSublineStringMatcher::setMaxRelevantAngle(Radians r)
{
  if (r > M_PI)
  {
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Max relevant angle is greaer than PI, did you specify the value in degrees instead "
               "of radians?");
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  _maxAngle = r;
}

void MaximalSublineStringMatcher::setMinSplitSize(Meters minSplitSize)
{
  _minSplitsize = minSplitSize;
  _configureSublineMatcher();
}

void MaximalSublineStringMatcher::setHeadingDelta(Meters headingDelta)
{
  _headingDelta = headingDelta;
  _configureSublineMatcher();
}

void MaximalSublineStringMatcher::setSublineMatcher(boost::shared_ptr<SublineMatcher> sm)
{
  _sublineMatcher = sm;
  _configureSublineMatcher();
}

void MaximalSublineStringMatcher::_validateElement(const ConstOsmMapPtr& map, ElementId eid) const
{
  ConstElementPtr e = map->getElement(eid);

  if (e->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr r = dynamic_pointer_cast<const Relation>(e);

    if (OsmSchema::getInstance().isMultiLineString(*r) == false)
    {
      throw NeedsReviewException("Internal Error: When matching sublines expected a multilinestring "
        "relation not a " + r->getType() + ".  A non-multilinestring should never be found here.  "
        "Please report this to hootenanny.help@digitalglobe.com");
    }

    const vector<RelationData::Entry>& entries = r->getMembers();
    for (size_t i = 0; i < entries.size(); i++)
    {
      if (entries[i].getElementId().getType() != ElementType::Way)
      {
        throw NeedsReviewException("MultiLineString relations can only contain ways when matching "
                                   "sublines.");
      }
    }
  }
  if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr w = dynamic_pointer_cast<const Way>(e);

    if (w->getNodeCount() <= 1)
    {
      throw NeedsReviewException("Internal Error: Attempting to match against a zero length way.");
    }
  }
}

}

