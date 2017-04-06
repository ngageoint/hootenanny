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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "MergeNearbyNodes.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/index/ClosePointHash.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QTime>

// Standard
#include <iostream>

// TGS
#include <tgs/StreamUtils.h>
#include <tgs/RStarTree/HilbertRTree.h>
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MergeNearbyNodes)

double calcDistance(const boost::shared_ptr<Node>& n1, const boost::shared_ptr<Node>& n2)
{
  double dx = n1->getX() - n2->getX();
  double dy = n1->getY() - n2->getY();
  return sqrt(dx * dx + dy * dy);
}

MergeNearbyNodes::MergeNearbyNodes(Meters distance)
{
  _distance = distance;

  if (_distance < 0.0)
  {
    _distance = ConfigOptions().getMergeNearbyNodesDistance();
  }
}

void MergeNearbyNodes::apply(boost::shared_ptr<OsmMap>& map)
{
  LOG_INFO("MergeNearbyNodes start");

  QTime time;
  time.start();

  boost::shared_ptr<OsmMap> wgs84;
  boost::shared_ptr<OsmMap> planar;

  if (MapProjector::isGeographic(map))
  {
    wgs84 = map;
    planar.reset(new OsmMap(map));
    MapProjector::projectToPlanar(planar);
  }
  else
  {
    planar = map;
    // if we need to check for bounds containment
    if (_bounds.isNull() == false)
    {
      wgs84.reset(new OsmMap(map));
      MapProjector::projectToWgs84(wgs84);
    }
  }

  ClosePointHash cph(_distance);

  const NodeMap& nodes = planar->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); it++)
  {
    const boost::shared_ptr<Node>& n = it->second;
    cph.addPoint(n->getX(), n->getY(), n->getId());
  }

  int mergeCount = 0;
  int count = 0;

  cph.resetIterator();
  while (cph.next())
  {
    const vector<long>& v = cph.getMatch();

    for (size_t i = 0; i < v.size(); i++)
    {
      for (size_t j = 0; j < v.size(); j++)
      {
        if (v[i] != v[j] && map->containsNode(v[i]) && map->containsNode(v[j]))
        {
          const boost::shared_ptr<Node>& n1 = planar->getNode(v[i]);
          const boost::shared_ptr<Node>& n2 = planar->getNode(v[j]);
          double d = calcDistance(n1, n2);
          if (d < _distance && n1->getStatus() == n2->getStatus())
          {
            bool replace = false;
            // if the geographic bounds are not specified.
            if (_bounds.isNull() == true)
            {
              replace = true;
            }
            // if the geographic bounds are specified, then make sure both points are inside.
            else
            {
              const boost::shared_ptr<Node>& g1 = wgs84->getNode(v[i]);
              const boost::shared_ptr<Node>& g2 = wgs84->getNode(v[j]);
              if (_bounds.contains(g1->getX(), g1->getY()) &&
                  _bounds.contains(g2->getX(), g2->getY()))
              {
                replace = true;
              }
            }

            if (replace)
            {
              map->replaceNode(v[j], v[i]);
              mergeCount++;
            }
          }
        }
      }
    }

    if (Log::getInstance().isInfoEnabled() && count % 1000 == 0)
    {
      cout << "MergeNearbyNodes " << count << " " << mergeCount << "    \r";
      cout.flush();
    }
    count++;
  }

  if (Log::getInstance().isInfoEnabled())
  {
    cout << "MergeNearbyNodes " << nodes.size() << " elapsed: " << time.elapsed() << "ms        \n";
  }
}

void MergeNearbyNodes::mergeNodes(boost::shared_ptr<OsmMap> map, Meters distance)
{
  MergeNearbyNodes mnn(distance);
  mnn.apply(map);
}

void MergeNearbyNodes::readObject(QDataStream& is)
{
  is >> _distance;
}

void MergeNearbyNodes::writeObject(QDataStream& os) const
{
  os << _distance;
}


}
