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
#include "WorkingMap.h"

// Boost
using namespace boost;

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/manipulators/Manipulation.h>
using namespace hoot::elements;

// Standard
#include <iostream>
#include <limits>

namespace hoot
{

deque< pair< const WorkingMap*, boost::shared_ptr<OsmMap> > > WorkingMap::_mapCache;

WorkingMap::WorkingMap(const WorkingMap& map)
{
  _score = map._score;
  _map.reset(new OsmMap(map.getMap()));
}

WorkingMap::WorkingMap(boost::shared_ptr<const WorkingMap> baseWorking,
                       boost::shared_ptr<const Manipulation> manipulation)
{
  _score = baseWorking->getScore() + manipulation->getScoreEstimate();
  _baseWorking = baseWorking;
  _manipulation = manipulation;
}

WorkingMap::WorkingMap(boost::shared_ptr<OsmMap> map)
{
  _map = map;
  _score = std::numeric_limits<double>::min();
}


WorkingMap::WorkingMap()
{
  _score = std::numeric_limits<double>::min();
}

double WorkingMap::calculatePotential() const
{
  return std::max(_sumWayLengths(Status::Unknown1), _sumWayLengths(Status::Unknown2));
}

boost::shared_ptr<const OsmMap> WorkingMap::getMap() const
{
  boost::shared_ptr<OsmMap> result;

  if (_map)
  {
    result = _map;
  }
  else
  {
    for (size_t i = 0; i < _mapCache.size(); i++)
    {
      if (_mapCache[i].first == this)
      {
        result = _mapCache[i].second;
      }
      else
      {
        set<ElementId> ignored1, ignored2;
        result = _baseWorking->takeMap();
        _manipulation->applyManipulation(result, ignored1, ignored2);

        while (_mapCache.size() > 100)
        {
          _mapCache.pop_back();
        }
        _mapCache.push_front(pair< const WorkingMap*, boost::shared_ptr<OsmMap> >(this, result));
      }
    }
    _map = result;
  }

  return result;
}

double WorkingMap::getScore() const
{
  if (_score != std::numeric_limits<double>::min())
  {
    return _score;
  }

  // score the number of meters of roads
  Meters lengthScore = 0;
//  lengthScore -= _sumWayLengths(Unknown1);
//  lengthScore -= _sumWayLengths(Unknown2);
  lengthScore += _sumWayLengths(Status::Conflated);

//  // score the number of intersections
  int intersectionScore = 0;
//  intersectionScore -= _countIntersections(Unknown1);
//  intersectionScore -= _countIntersections(Unknown2);
//  intersectionScore += _countIntersections(Conflated);

  // create a weighted score.
  _score = lengthScore + 20 * intersectionScore;

  return _score;
}

Meters WorkingMap::_sumWayLengths(Status status) const
{
  ElementConverter ec(_map);
  Meters result = 0;
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator it2 = ways.begin(); it2 != ways.end(); it2++)
  {
    if (it2->second->getStatus() == status)
    {
      result += ec.calculateLength(it2->second);
    }
  }

  return result;
}

int WorkingMap::_countIntersections(Status status) const
{
  QHash<long, int> nodeCounts;

  const WayMap& ways = _map->getWays();
  WayMap::const_iterator it2 = ways.begin();
  while (it2 != ways.end())
  {
    const boost::shared_ptr<Way>& way = it2->second;
    if (way->getStatus() == status)
    {
      for (size_t j = 0; j < way->getNodeCount(); j++)
      {
        nodeCounts[way->getNodeId(j)] = nodeCounts[way->getNodeId(j)] + 1;
      }
    }
    ++it2;
  }

  int result = 0;
  QHash<long, int>::const_iterator i = nodeCounts.constBegin();
  while (i != nodeCounts.constEnd()) {
    if (i.value() > 1)
    {
      result += i.value() - 1;
    }
    ++i;
  }

  return result;
}

boost::shared_ptr<OsmMap> WorkingMap::takeMap() const
{
  boost::shared_ptr<OsmMap> result = _map;
  _map.reset();
  return result;
}

}
