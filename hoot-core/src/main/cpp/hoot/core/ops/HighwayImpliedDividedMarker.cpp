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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "HighwayImpliedDividedMarker.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/elements/NodeToWayMap.h>

// Standard
#include <iostream>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, HighwayImpliedDividedMarker)

bool HighwayImpliedDividedMarker::_dividerSandwich(const std::shared_ptr<Way>& w)
{
  long firstNodeId = w->getNodeId(0);
  long lastNodeId = w->getLastNodeId();

  if (_hasDividerConnected(firstNodeId, w->getId()) &&
      _hasDividerConnected(lastNodeId, w->getId()))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool HighwayImpliedDividedMarker::_hasDividerConnected(long nodeId, long excludedWayId)
{
  const set<long>& wayIds = (*_n2w)[nodeId];

  for (set<long>::iterator it = wayIds.begin(); it != wayIds.end(); ++it)
  {
    if (*it != excludedWayId)
    {
      std::shared_ptr<const Way> w = _result->getWay(*it);
      if (w->getTags()["divider"] == "yes")
      {
        return true;
      }
    }
  }

  return false;
}

std::shared_ptr<OsmMap> HighwayImpliedDividedMarker::markDivided(
  const std::shared_ptr<const OsmMap>& map)
{
  HighwayImpliedDividedMarker t(map);
  return t.markDivided();
}

std::shared_ptr<OsmMap> HighwayImpliedDividedMarker::markDivided()
{
  _numAffected = 0;
  std::shared_ptr<OsmMap> result(new OsmMap(_inputMap));
  _result = result;

  // create a map from nodes to ways
  _n2w.reset(new NodeToWayMap(*_inputMap));

  // find all the tunnels & bridges
  std::shared_ptr<TagCriterion> tunnelCrit(new TagCriterion("tunnel", "yes"));
  std::shared_ptr<TagCriterion> bridgeCrit(new TagCriterion("bridge", "yes"));
  ChainCriterion chain(tunnelCrit, bridgeCrit);
  vector<long> wayIds = ElementIdsVisitor::findElements(_result, ElementType::Way, &chain);

  // go through each way
  for (size_t i = 0; i < wayIds.size(); i++)
  {
    std::shared_ptr<Way> w = _result->getWay(wayIds[i]);
    // if the way has a divided road on both ends
    if (w && _dividerSandwich(w))
    {
      // mark this tunnel/bridge as divided.
      w->setTag("divider", "yes");
      _numAffected++;
    }
  }

  _result.reset();
  return result;
}

void HighwayImpliedDividedMarker::apply(std::shared_ptr<OsmMap>& map)
{
  map = markDivided(map);
}

}
