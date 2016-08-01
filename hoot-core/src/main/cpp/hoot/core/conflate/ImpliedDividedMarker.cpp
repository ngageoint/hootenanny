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

#include "ImpliedDividedMarker.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/Factory.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

// Standard
#include <iostream>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

#include "NodeToWayMap.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ImpliedDividedMarker)

ImpliedDividedMarker::ImpliedDividedMarker()
{

}

ImpliedDividedMarker::ImpliedDividedMarker(shared_ptr<const OsmMap> map)
{
  _inputMap = map;
}

bool ImpliedDividedMarker::_dividerSandwhich(shared_ptr<Way> w)
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

bool ImpliedDividedMarker::_hasDividerConnected(long nodeId, long excludedWayId)
{
  const set<long>& wayIds = (*_n2w)[nodeId];

  for (set<long>::iterator it = wayIds.begin(); it != wayIds.end(); it++)
  {
    if (*it != excludedWayId)
    {
      shared_ptr<const Way> w = _result->getWay(*it);
      if (w->getTags()["divider"] == "yes")
      {
        return true;
      }
    }
  }

  return false;
}

shared_ptr<OsmMap> ImpliedDividedMarker::markDivided(shared_ptr<const OsmMap> map)
{
  ImpliedDividedMarker t(map);
  return t.markDivided();
}

shared_ptr<OsmMap> ImpliedDividedMarker::markDivided()
{
  shared_ptr<OsmMap> result(new OsmMap(_inputMap));
  _result = result;

  // create a map from nodes to ways
  _n2w.reset(new NodeToWayMap(*_inputMap));

  // find all the tunnels & bridges
  shared_ptr<TagCriterion> tunnelCrit(new TagCriterion("tunnel", "yes"));
  shared_ptr<TagCriterion> bridgeCrit(new TagCriterion("bridge", "yes"));
  ChainCriterion chain(tunnelCrit, bridgeCrit);
  vector<long> wayIds = FindWaysVisitor::findWays(_result, &chain);

  // go through each way
  for (size_t i = 0; i < wayIds.size(); i++)
  {
    shared_ptr<Way> w = _result->getWay(wayIds[i]);
    // if the way has a divided road on both ends
    if (_dividerSandwhich(w))
    {
      // mark this tunnel/bridge as divided.
      w->setTag("divider", "yes");
    }
  }


  _result.reset();
  return result;
}

void ImpliedDividedMarker::apply(shared_ptr<OsmMap>& map)
{
  map = markDivided(map);
}

}
