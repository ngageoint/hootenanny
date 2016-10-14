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

#include "DanglerRemoverManipulation.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

DanglerRemoverManipulation::DanglerRemoverManipulation(long wayId, shared_ptr<const OsmMap> map,
                                                       Meters errorPlus)
{
  _wayId = wayId;
  _errorPlus = errorPlus;
  updateEstimate(map);
}

void DanglerRemoverManipulation::applyManipulation(shared_ptr<OsmMap> map,
  set<ElementId>& impactedElements, set<ElementId>&) const
{
  shared_ptr<OsmMap> result = map;

  // insert the impacted ways
  impactedElements = getImpactedElementIds(map);
  impactedElements.erase(ElementId::way(_wayId));

  RemoveWayOp::removeWay(result, _wayId);
}

double DanglerRemoverManipulation::calculateProbability(shared_ptr<const OsmMap> map) const
{
  shared_ptr<const Way> baseWay = map->getWay(_wayId);

  shared_ptr<LineString> ls = ElementConverter(map).convertToLineString(baseWay);

  const OsmMapIndex& index = map->getIndex();
  const NodeToWayMap& n2w = *index.getNodeToWayMap();

  double score = 0;

  // if the way is short enough
  if (ls->getLength() < baseWay->getCircularError() + _errorPlus)
  {
    long firstId = baseWay->getNodeId(0);
    long lastId = baseWay->getLastNodeId();

    size_t firstCount = n2w.find(firstId)->second.size();
    size_t lastCount = n2w.find(lastId)->second.size();

    // if the way loops back and the front/back touch
    if (firstId == lastId)
    {
      // very good chance that it is not useful.
      score = 0.9;
    }
    // if the way connects two other ways
    else if (firstCount > 1 && lastCount > 1)
    {
      // reasonable chance it is relevant to the network
      score = 0.005;
    }
    // if the way doesn't connect to any other ways
    else if (firstCount == 1 && lastCount == 1)
    {
      // toss that junk
      score = 0.5;
    }
    // if the way connects to only one other way
    else if (firstCount > 1 || lastCount > 1)
    {
      // reasonable chance it is junk
      score = 0.1;
    }
  }

  return score;
}

double DanglerRemoverManipulation::calculateScore(shared_ptr<const OsmMap> map) const
{
  assert(isValid(map));

  _p = calculateProbability(map);

  return _p;
}

bool DanglerRemoverManipulation::isValid(shared_ptr<const OsmMap> map) const
{
  return map->containsWay(_wayId);
}

const set<ElementId>& DanglerRemoverManipulation::getImpactedElementIds(const ConstOsmMapPtr& map)
  const
{
  _impactedElements.clear();
  _impactedElements.insert(ElementId::way(_wayId));

  NodeToWayMap& n2w = *map->getIndex().getNodeToWayMap();

  shared_ptr<const Way> way = map->getWay(_wayId);

  const set<long>& s1 = n2w.at(way->getNodeId(0));
  for (set<long>::const_iterator it = s1.begin(); it != s1.end(); it++)
  {
    _impactedElements.insert(ElementId::way(*it));
  }

  const set<long>& s2 = n2w.at(way->getLastNodeId());
  for (set<long>::const_iterator it = s2.begin(); it != s2.end(); it++)
  {
    _impactedElements.insert(ElementId::way(*it));
  }

  return _impactedElements;
}

QString DanglerRemoverManipulation::toString() const
{
  return QString("DanglerRemoverManipulation wayId: %1 score: %3 p: %4").arg(_wayId).
      arg(getScoreEstimate()).arg(_p);
}

}
