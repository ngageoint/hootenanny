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
#include "PointsToTracksOp.h"

// hoot
#include <hoot/core/Factory.h>

// tgs
#include <tgs/HashMap.h>

using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, PointsToTracksOp)

struct SortFunctor
{
  shared_ptr<OsmMap> map;

  bool operator() (long nid1, long nid2)
  {
    const QString& seq1 = map->getNode(nid1)->getTags()["hoot:track:seq"];
    const QString& seq2 = map->getNode(nid2)->getTags()["hoot:track:seq"];

    return seq1 < seq2;
  }
};

PointsToTracksOp::PointsToTracksOp()
{
}

void PointsToTracksOp::apply(shared_ptr<OsmMap>& map)
{
  HashMap<QString, deque<long> > trackIdToNid;

  const NodeMap& nm = map->getNodeMap();

  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    if (it->second->getTags().contains("hoot:track:id") &&
        it->second->getTags().contains("hoot:track:seq"))
    {
      QString trackId = it->second->getTags()["hoot:track:id"];
      trackIdToNid[trackId].push_back(it->first);
    }
  }

  _sortTracks(map, trackIdToNid);
  _createWays(map, trackIdToNid);
}

void PointsToTracksOp::_createWays(shared_ptr<OsmMap>& map, HashMap<QString, deque<long> >& tracks)
{
  for (HashMap<QString, deque<long> >::const_iterator it = tracks.begin(); it != tracks.end();
    ++it)
  {
    const deque<long>& d = it->second;
    shared_ptr<Node> firstNode = map->getNode(d[0]);

    LOG_INFO("Circular Error: " << firstNode->getCircularError());
    shared_ptr<Way> w(new Way(firstNode->getStatus(), map->createNextWayId(),
      firstNode->getRawCircularError()));

    // check to see if all the nodes have the same non-empty highway tag.
    QString highway = firstNode->getTags()["highway"];
    for (deque<long>::const_iterator it = d.begin(); highway != "" && it != d.end(); ++it)
    {
      if (map->getNode(*it)->getTags()["highway"] != highway)
      {
        highway = "";
      }
    }

    // if all the nodes have the same non-empty highway tag
    if (highway != "")
    {
      // move the highway tag from the nodes into the way.
      w->getTags()["highway"] = highway;
      for (deque<long>::const_iterator it = d.begin(); highway != "" && it != d.end(); ++it)
      {
        map->getNode(*it)->getTags().remove("highway");
      }
    }

    w->addNodes(d.begin(), d.end());

    map->addWay(w);
  }
}

void PointsToTracksOp::_sortTracks(shared_ptr<OsmMap>& map, HashMap<QString, deque<long> >& tracks)
{

  SortFunctor sf;
  sf.map = map;

  for (HashMap<QString, deque<long> >::iterator it = tracks.begin(); it != tracks.end();
    ++it)
  {
    deque<long>& d = it->second;

    sort(d.begin(), d.end(), sf);
  }
}

}
